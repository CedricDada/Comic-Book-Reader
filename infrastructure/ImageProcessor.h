// infrastructure/ImageProcessor.h
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "../model/AbstractImage.h"
#include "../infrastructure/Filter.h" 

class ImageProcessor {
public:
    static void resize(AbstractImage* image, int width, int height);
    static void applyFilter(AbstractImage* image, ContentType contentType);
    static void optimizeForDisplay(AbstractImage* image);
    static QImage processImage(AbstractImage* image) {
        return image->toQImage();
        // Implémentation du traitement
    }
};

#endif // IMAGEPROCESSOR_H