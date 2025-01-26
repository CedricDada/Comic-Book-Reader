// infrastructure/ImageProcessor.h
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "../model/AbstractImage.h"
#include "../infrastructure/Filter.h" 
#include <QBuffer>
#include <QImageReader>

class ImageProcessor {
public:
    static void resize(AbstractImage* image, int width, int height);
    static void applyFilter(AbstractImage* image, ContentType contentType);
    static void optimizeForDisplay(AbstractImage* image);
    static QImage processImage(AbstractImage* image) {
        return image->toQImage();
        // Implémentation du traitement
    }
    static QByteArray processRawData(const QByteArray& rawData) {
        // Exemple de traitement bas niveau
        QBuffer buffer;
        buffer.setData(rawData);
        buffer.open(QIODevice::ReadOnly);
        
        QImageReader reader(&buffer);
        reader.setAutoTransform(true);
        QImage image = reader.read();
        
        // Appliquer des transformations
        image = image.convertToFormat(QImage::Format_RGB32);
        
        // Recoder en JPEG par exemple
        QByteArray processedData;
        QBuffer outputBuffer(&processedData);
        outputBuffer.open(QIODevice::WriteOnly);
        image.save(&outputBuffer, "JPEG", 85);
        
        return processedData;
    }
};

#endif // IMAGEPROCESSOR_H