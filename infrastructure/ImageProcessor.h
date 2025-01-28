#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "../model/AbstractImage.h"
#include "../infrastructure/Filter.h"
#include <QImage>
#include <QBuffer>
#include <QImageReader>

#include <qimage.h>

class ImageProcessor {
public:
    static void resize(AbstractImage* image, int width, int height, bool keepDetails = true);
    static void applyFilter(AbstractImage* image, ContentType contentType, float intensity = 1.0f);

    static void optimizeForDisplay(AbstractImage* image);
    static QImage processImage(AbstractImage* image) {
        QImage processed = image->toQImage();
        processed = processed.convertToFormat(QImage::Format_ARGB32);
        return processed;
    }
    static QByteArray processRawData(const QByteArray& rawData) {
        QBuffer buffer;
        buffer.setData(rawData);
        buffer.open(QIODevice::ReadOnly);
        
        QImageReader reader(&buffer);
        reader.setAutoTransform(true);
        QImage image = reader.read();
        
        image = image.convertToFormat(QImage::Format_RGB32);
        
        QByteArray processedData;
        QBuffer outputBuffer(&processedData);
        outputBuffer.open(QIODevice::WriteOnly);
        image.save(&outputBuffer, "JPEG", 85);
        
        return processedData;
    }
    static QImage::Format parseFormat(AbstractImage::Format format);



private:
    // Conversion sécurisée avec copie des données
    static QImage convertToQImage(const AbstractImage* image);
    
    // Mise à jour des données de l'image source
    static void updateImageData(AbstractImage* image, const QImage& processed);
};
#endif // IMAGEPROCESSOR_H