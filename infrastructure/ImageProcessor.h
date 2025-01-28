#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "../model/AbstractImage.h"
#include "../infrastructure/Filter.h"
#include <QImage>
#include <qimage.h>

class ImageProcessor {
public:

    static void resize(AbstractImage* image, int width, int height, bool keepDetails = true);

    static void applyFilter(AbstractImage* image, ContentType contentType, float intensity = 1.0f);

    static void optimizeForDisplay(AbstractImage* image);

    static QImage processImage(AbstractImage* image) {
        QImage processed = image->toQImage();
        // Exemple de pipeline de base (à personnaliser)
        processed = processed.convertToFormat(QImage::Format_ARGB32);
        return processed;
    }

    static QImage::Format parseFormat(AbstractImage::Format format);



private:
    static QImage convertToQImage(const AbstractImage* image);
    
    // Mise à jour des données de l'image source
    static void updateImageData(AbstractImage* image, const QImage& processed);
};
#endif // IMAGEPROCESSOR_H