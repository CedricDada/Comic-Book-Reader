// infrastructure/ImageProcessor.h
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "../model/AbstractImage.h"
#include "../model/Filter.h"  // Inclusion ajoutée

class ImageProcessor {
public:
    static void resize(AbstractImage* image, int width, int height);
    static void applyFilter(AbstractImage* image, const Filter& filter); // Correction
};

#endif // IMAGEPROCESSOR_H