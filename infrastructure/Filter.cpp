#include "Filter.h"
#include <QColor>
#include <QtMath>
#include <vector>

// LowPassFilter
LowPassFilter::LowPassFilter(float cutoffFrequency, int kernelSize)
    : m_cutoffFrequency(qBound(0.1f, cutoffFrequency, 1.0f)),
      m_kernelSize(kernelSize % 2 == 0 ? kernelSize + 1 : kernelSize) {}

void LowPassFilter::apply(QImage& image, ContentType contentType) const {
    switch(contentType) {
        case ContentType::TextDominant:
            applyToText(image);
            break;
        case ContentType::GraphicDominant:
            applyToGraphics(image);
            break;
        default:
            applyAdaptive(image);
    }
}

void LowPassFilter::applyToText(QImage& image) const {
    // Noyau gaussien adapté au texte
    const int radius = m_kernelSize / 2;
    std::vector<float> kernel(m_kernelSize);
    float sigma = m_cutoffFrequency * radius;
    float sum = 0.0f;

    // Génération du noyau gaussien
    for(int i = -radius; i <= radius; ++i) {
        float val = qExp(-(i*i)/(2*sigma*sigma));
        kernel[i + radius] = val;
        sum += val;
    }

    // Normalisation
    for(auto& k : kernel) k /= sum;

    // Application horizontale et verticale
    QImage temp = image.convertToFormat(QImage::Format_ARGB32);
    
    // Passe horizontale
    for(int y = 0; y < image.height(); ++y) {
        QRgb* line = reinterpret_cast<QRgb*>(temp.scanLine(y));
        for(int x = radius; x < image.width() - radius; ++x) {
            float r = 0, g = 0, b = 0;
            for(int k = -radius; k <= radius; ++k) {
                QRgb pixel = image.pixel(x + k, y);
                float weight = kernel[k + radius];
                r += qRed(pixel) * weight;
                g += qGreen(pixel) * weight;
                b += qBlue(pixel) * weight;
            }
            line[x] = qRgb(r, g, b);
        }
    }
    
    // Passe verticale (similaire)
    // ...
}

// Filtrage plus agressif pour les graphiques
void LowPassFilter::applyToGraphics(QImage& image) const {
    // Implémentation avec noyau binomial
    // ...
}

// TextEnhancementFilter
void TextEnhancementFilter::apply(QImage& image, ContentType) const {
    reduceJPEGArtifacts(image);
    enhanceEdges(image);
}

void TextEnhancementFilter::enhanceEdges(QImage& image) const {
    // Noyau de détection de contours inversé
    const int kernel[3][3] = {{-1, -1, -1}, 
                              {-1,  9, -1}, 
                              {-1, -1, -1}};
    
    QImage temp = image.copy();
    for(int y = 1; y < image.height()-1; ++y) {
        for(int x = 1; x < image.width()-1; ++x) {
            int r = 0, g = 0, b = 0;
            for(int ky = -1; ky <= 1; ++ky) {
                for(int kx = -1; kx <= 1; ++kx) {
                    QRgb pixel = temp.pixel(x + kx, y + ky);
                    int weight = kernel[ky+1][kx+1];
                    r += qRed(pixel) * weight;
                    g += qGreen(pixel) * weight;
                    b += qBlue(pixel) * weight;
                }
            }
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);
            image.setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void TextEnhancementFilter::reduceJPEGArtifacts(QImage& image) const {
    // Réduction des artefacts de compression
    LowPassFilter(0.3f, 3).apply(image, ContentType::TextDominant);
}

std::unique_ptr<AbstractFilter> AbstractFilter::createLowPassFilter(float cutoff) {
    return std::make_unique<LowPassFilter>(cutoff);
}
std::unique_ptr<AbstractFilter> AbstractFilter::createTextOptimizedFilter() {
    return std::make_unique<TextEnhancementFilter>();
}
void LowPassFilter::applyAdaptive(QImage& image) const {
    // Implémentation exemple
    QImage temp = image;
    applyToText(temp);    // 50% de texte
    applyToGraphics(temp);// 50% graphiques
    image = temp;
}

// LowPassFilter::clone
std::unique_ptr<AbstractFilter> LowPassFilter::clone() const {
    return std::make_unique<LowPassFilter>(*this);
}

// TextEnhancementFilter::clone
std::unique_ptr<AbstractFilter> TextEnhancementFilter::clone() const {
    return std::make_unique<TextEnhancementFilter>(*this);
}