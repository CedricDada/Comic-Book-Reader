#include "Filter.h"
#include <QColor>
#include <QtMath>
#include <vector>
#include <algorithm>
#include <omp.h> // Pour la parallélisation

//-----------------------------------------------------------------------------
// LowPassFilter Implementation
//-----------------------------------------------------------------------------

LowPassFilter::LowPassFilter(float cutoffFrequency, int kernelSize)
    : m_cutoffFrequency(qBound(0.1f, cutoffFrequency, 1.0f)),
      m_kernelSize(qBound(3, kernelSize, 15)) // Taille de noyau entre 3 et 15
{
    // Garantit un noyau impair pour symétrie
    if(m_kernelSize % 2 == 0)
        m_kernelSize++;
}

void LowPassFilter::apply(QImage& image, ContentType contentType) const
{
    QImage::Format originalFormat = image.format();
    QImage processed = image.convertToFormat(QImage::Format_ARGB32);

    switch(contentType) {
        case ContentType::TextDominant:
            applyToText(processed); 
            break;
        case ContentType::GraphicDominant:
            applyToGraphics(processed); 
            break;
        default:
            applyAdaptive(processed); 
    }

    image = processed.convertToFormat(originalFormat);
}

void LowPassFilter::applyToText(QImage& image) const
{
    const float sigma = 0.3f + (0.7f * m_cutoffFrequency);
    std::vector<float> kernel(m_kernelSize);
    generateGaussianKernel(kernel, sigma);
    applySeparableFilter(image, kernel);
}

void LowPassFilter::applyToGraphics(QImage& image) const
{
    std::vector<float> kernel(11);
    generateGaussianKernel(kernel, 2.0f);
    applySeparableFilter(image, kernel);
}

void LowPassFilter::applyAdaptive(QImage& image) const
{
    if (image.width() > image.height()) {
        applyToGraphics(image);
    } else {
        applyToText(image);
    }
}

void LowPassFilter::generateGaussianKernel(std::vector<float>& kernel, float sigma) const
{
    /* 
     * Formule : G(x) = (1/(σ√(2π))) * e^(-x²/(2σ²))
     */
    const int radius = kernel.size() / 2;
    float sum = 0.0f;
    
    for(int i = -radius; i <= radius; ++i) {
        float val = qExp(-(i*i)/(2*sigma*sigma));
        kernel[i + radius] = val;
        sum += val;
    }
    
    for(auto& k : kernel)
        k /= sum;
}

void LowPassFilter::applySeparableFilter(QImage& image, const std::vector<float>& kernel) const
{
    QImage temp(image.size(), image.format());
    applyConvolution(image, temp, kernel, true);  // Horizontale
    applyConvolution(temp, image, kernel, false); // Verticale
}

void LowPassFilter::applyConvolution(const QImage& src, QImage& dst, 
                                   const std::vector<float>& kernel, 
                                   bool horizontal) const
{
    const int radius = static_cast<int>(kernel.size()) / 2;
    
    #pragma omp parallel for // Parallélisation des lignes
    for(int y = 0; y < src.height(); ++y) {
        const QRgb* srcLine = reinterpret_cast<const QRgb*>(src.scanLine(y));
        QRgb* dstLine = reinterpret_cast<QRgb*>(dst.scanLine(y));
        
        for(int x = 0; x < src.width(); ++x) {
            float r = 0, g = 0, b = 0;
            
            for(int k = -radius; k <= radius; ++k) {
                // Gestion des bords par clamp
                int px = horizontal ? std::clamp(x + k, 0, src.width()-1) : x;
                int py = horizontal ? y : std::clamp(y + k, 0, src.height()-1);
                
                QRgb pixel = src.pixel(px, py);
                float weight = kernel[k + radius];
                
                r += qRed(pixel) * weight;
                g += qGreen(pixel) * weight;
                b += qBlue(pixel) * weight;
            }
            
            dstLine[x] = qRgb(static_cast<int>(qBound(0.0f, r, 255.0f)),
                             static_cast<int>(qBound(0.0f, g, 255.0f)),
                             static_cast<int>(qBound(0.0f, b, 255.0f)));
        }
    }
}

//-----------------------------------------------------------------------------
// TextEnhancementFilter Implementation
//-----------------------------------------------------------------------------

void TextEnhancementFilter::apply(QImage& image, ContentType contentType) const
{
    if(contentType != ContentType::GraphicDominant) {
        reduceJPEGArtifacts(image);
    }
    enhanceEdges(image);
}

void TextEnhancementFilter::enhanceEdges(QImage& image) const
{
    /* Noyau de convolution "unsharp masking" adapté aux BD :
     * [ -0.5, -1.0, -0.5 ]
     * [ -1.0,  7.0, -1.0 ]
     * [ -0.5, -1.0, -0.5 ]
     * Renforce les contours sans sur-accentuation
     */
    const float kernel[3][3] = {{-0.5f, -1.0f, -0.5f}, 
                               {-1.0f,  7.0f, -1.0f}, 
                               {-0.5f, -1.0f, -0.5f}};
    
    QImage temp = image.copy();
    
    #pragma omp parallel for // Parallélisation par ligne
    for(int y = 1; y < image.height()-1; ++y) {
        QRgb* line = reinterpret_cast<QRgb*>(image.scanLine(y));
        
        for(int x = 1; x < image.width()-1; ++x) {
            float r = 0, g = 0, b = 0;
            
            // Convolution 3x3
            for(int ky = -1; ky <= 1; ++ky) {
                const QRgb* srcLine = reinterpret_cast<const QRgb*>(temp.scanLine(y + ky));
                
                for(int kx = -1; kx <= 1; ++kx) {
                    QRgb pixel = srcLine[x + kx];
                    float weight = kernel[ky+1][kx+1];
                    
                    r += qRed(pixel) * weight;
                    g += qGreen(pixel) * weight;
                    b += qBlue(pixel) * weight;
                }
            }
            
            line[x] = qRgb(static_cast<int>(qBound(0.0f, r, 255.0f)),
                          static_cast<int>(qBound(0.0f, g, 255.0f)),
                          static_cast<int>(qBound(0.0f, b, 255.0f)));
        }
    }
}

void TextEnhancementFilter::reduceJPEGArtifacts(QImage& image) const
{
    const int radius = 2;
    const float spatialSigma = 1.5f;
    const float rangeSigma = 25.0f;
    
    QImage temp = image.copy();
    
    #pragma omp parallel for
    for(int y = radius; y < image.height()-radius; ++y) {
        QRgb* line = reinterpret_cast<QRgb*>(image.scanLine(y));
        
        for(int x = radius; x < image.width()-radius; ++x) {
            float sumWeights = 0.0f;
            float r = 0, g = 0, b = 0;
            QRgb centerPixel = temp.pixel(x, y);
            
            // Parcours du voisinage
            for(int ky = -radius; ky <= radius; ++ky) {
                const QRgb* srcLine = reinterpret_cast<const QRgb*>(temp.scanLine(y + ky));
                
                for(int kx = -radius; kx <= radius; ++kx) {
                    QRgb pixel = srcLine[x + kx];
                    
                    float spatial = qExp(-(kx*kx + ky*ky)/(2*spatialSigma*spatialSigma));
                    
                    float range = qExp(-pow(qRed(pixel) - qRed(centerPixel), 2) / 
                                      (2*rangeSigma*rangeSigma));
                    
                    float weight = spatial * range;
                    
                    r += qRed(pixel) * weight;
                    g += qGreen(pixel) * weight;
                    b += qBlue(pixel) * weight;
                    sumWeights += weight;
                }
            }
            
            line[x] = qRgb(static_cast<int>(std::clamp(r/sumWeights, 0.0f, 255.0f)),
                          static_cast<int>(std::clamp(g/sumWeights, 0.0f, 255.0f)),
                          static_cast<int>(std::clamp(b/sumWeights, 0.0f, 255.0f)));
        }
    }
}


std::unique_ptr<AbstractFilter> AbstractFilter::createLowPassFilter(float cutoffFrequency) {
    return std::make_unique<LowPassFilter>(cutoffFrequency);
}

std::unique_ptr<AbstractFilter> AbstractFilter::createTextOptimizedFilter() {
    return std::make_unique<TextEnhancementFilter>();
}


std::unique_ptr<AbstractFilter> LowPassFilter::clone() const {
    return std::make_unique<LowPassFilter>(*this);
}

std::unique_ptr<AbstractFilter> TextEnhancementFilter::clone() const {
    return std::make_unique<TextEnhancementFilter>(*this);
}