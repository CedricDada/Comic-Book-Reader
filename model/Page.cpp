#include "Page.h"
#include "../infrastructure/ImageProcessor.h"
#include <QVariant>

Page::Page(int num, AbstractImage* img, const QVariantMap& meta)
    : number(num), image(img), metadata(meta) {}

void Page::applyFilter(const Filter& filter) {
    if(image) ImageProcessor::applyFilter(image, filter);
}

void Page::resize(int width, int height) {
    if(image && width > 0 && height > 0) {
        ImageProcessor::resize(image, width, height);
    }
}

// Clone profond pour la gestion du cache
Page* Page::clone() const {
    return new Page(
        number, 
        image/* ? image->clone() : nullptr*/, 
        metadata
    );
}