#include "ImageProcessor.h"
#include "../infrastructure/Filter.h"
#include <QImage>

void ImageProcessor::resize(AbstractImage* image, int width, int height) {
    QImage qimg(image->data().data(), 
                image->width(), 
                image->height(), 
                QImage::Format_ARGB32);
    QImage resized = qimg.scaled(width, height, Qt::KeepAspectRatio);
    // Mettre à jour les données de l'image
}
void ImageProcessor::applyFilter(AbstractImage* image, ContentType contentType) {
    QImage qimg = image->toQImage();
    
    // Sélection automatique du filtre
    auto filter = AbstractFilter::createLowPassFilter(
        contentType == ContentType::TextDominant ? 0.4f : 0.6f
    );
    
    filter->apply(qimg, contentType);
}
void ImageProcessor::optimizeForDisplay(AbstractImage* image){

}