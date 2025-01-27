#include "ImageProcessor.h"
#include <QtMath>
#include <vector>
#include "Filter.h"
#include <QBuffer>
#include <QImageReader>


// Conversion sécurisée avec copie indépendante
QImage ImageProcessor::convertToQImage(const AbstractImage* image) {
    QImage::Format format = parseFormat(image->format_for_filter());
    // Calculate bytes per line (assuming 4 bytes per pixel for RGBA8888)
    int bytesPerLine = image->width() * 4;
    return QImage(image->data().data(), 
                  image->width(), 
                  image->height(), 
                  bytesPerLine, 
                  format);
}
// Helper function to convert format string to QImage::Format
QImage::Format ImageProcessor::parseFormat(AbstractImage::Format format) {
    switch(format) {
        case AbstractImage::PNG: return QImage::Format_RGBA8888;
        case AbstractImage::JPEG: return QImage::Format_RGB32;
        case AbstractImage::BMP: return QImage::Format_RGB32;
        default: return QImage::Format_Invalid;
    }
}

// Mise à jour des données de l'image source
void ImageProcessor::updateImageData(AbstractImage* image, const QImage& processed) {
    // Convert QImage to the AbstractImage's native format
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    processed.save(&buffer, AbstractImage::formatToString(image->format_for_filter()).c_str());

    // Update using the abstract interface
    image->setData(
        std::vector<unsigned char>(byteArray.begin(), byteArray.end()),
        processed.width(),
        processed.height(),
        AbstractImage::formatToString(image->format_for_filter())
    );
}

void ImageProcessor::resize(AbstractImage* image, int width, int height, bool keepDetails) {
    QImage qimg = convertToQImage(image);
    
    Qt::TransformationMode transformMode = keepDetails ? 
        Qt::SmoothTransformation : 
        Qt::FastTransformation;
    
    // Calcul du ratio de préservation
    QSize newSize = qimg.size().scaled(width, height, Qt::KeepAspectRatio);
    
    // Algorithme adaptatif
    if(qimg.width() > newSize.width()) { // Downscale
        qimg = qimg.scaled(newSize, Qt::KeepAspectRatio, transformMode);
    } else { // Upscale
        qimg = qimg.scaled(newSize, Qt::KeepAspectRatio, Qt::FastTransformation);
    }
    
    updateImageData(image, qimg);
}

void ImageProcessor::applyFilter(AbstractImage* image, ContentType contentType, float intensity) {
    QImage qimg = convertToQImage(image); // internal conversion
    intensity = qBound(0.0f, intensity, 1.0f);
    
    // Sélection dynamique des filtres
    std::vector<std::unique_ptr<AbstractFilter>> filters;
    
    switch(contentType) {
        case ContentType::TextDominant:
            filters.emplace_back(AbstractFilter::createTextOptimizedFilter());
            break;
            
        case ContentType::GraphicDominant:
            filters.emplace_back(AbstractFilter::createLowPassFilter(0.7f * intensity));
            break;
            
        default: // MixedContent et AutoDetect
            filters.emplace_back(AbstractFilter::createLowPassFilter(0.5f * intensity));
            filters.emplace_back(AbstractFilter::createTextOptimizedFilter());
    }
    
    // Application en chaîne
    for(auto& filter : filters) {
        filter->apply(qimg, contentType);
    }
    
    updateImageData(image, qimg); // write back to the image
}

void ImageProcessor::optimizeForDisplay(AbstractImage* image) {
    QImage qimg = convertToQImage(image);
    
    // 1. Correction gamma pour écrans
    qimg = qimg.convertToFormat(QImage::Format_ARGB32);
    for(int y = 0; y < qimg.height(); ++y) {
        QRgb* line = reinterpret_cast<QRgb*>(qimg.scanLine(y));
        for(int x = 0; x < qimg.width(); ++x) {
            QColor color(line[x]);
            color.setRedF(qPow(color.redF(), 1/2.2f));
            color.setGreenF(qPow(color.greenF(), 1/2.2f));
            color.setBlueF(qPow(color.blueF(), 1/2.2f));
            line[x] = color.rgba();
        }
    }
    
    // 2. Filtrage adaptatif
    applyFilter(image, ContentType::AutoDetect, 0.8f);
    
    // 3. Dernières optimisations
    QImage optimized = qimg.convertToFormat(QImage::Format_RGB888);
    updateImageData(image, optimized);
}