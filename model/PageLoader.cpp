/**
 * Project Untitled
 */


#include "PageLoader.h"

void PageLoader::processImage(AbstractImage* image) {
    QFuture<void> future = QtConcurrent::run([image](){
        ImageProcessor::applyFilters(image);
        ImageProcessor::optimizeForDisplay(image);
    });
}

void PageLoader::preloadPages(AbstractBook* book, int start, int count) {
    for(int i = start; i < start + count; ++i) {
        QtConcurrent::run([=](){
            Page p = book->getPage(i);
            ImageProcessor::processImage(p.image);
            emit pageLoaded(i, p.image);
        });
    }
}