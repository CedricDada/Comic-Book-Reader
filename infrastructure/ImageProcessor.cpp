#include "ImageProcessor.h"
#include "../model/Filter.h"
#include <QImage>

void ImageProcessor::resize(AbstractImage* image, int width, int height) {
    QImage qimg(image->data().data(), 
                image->width(), 
                image->height(), 
                QImage::Format_ARGB32);
    QImage resized = qimg.scaled(width, height, Qt::KeepAspectRatio);
    // Mettre à jour les données de l'image
}
void ImageProcessor::applyFilter(AbstractImage* image, const Filter& filter) {
}
