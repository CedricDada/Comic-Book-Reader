#include "Page.h"
#include "../infrastructure/ImageProcessor.h"
#include "../infrastructure/Filter.h"
#include <QVariant>

void Page::applyFilter(ContentType contentType) {
    if(image) ImageProcessor::applyFilter(image.get(), contentType);
}

void Page::resize(int width, int height) {
    if(image && width > 0 && height > 0) {
        ImageProcessor::resize(image.get(), width, height);
    }
}

// Clone profond pour la gestion du cache
// Page* Page::clone() const {
//     return new Page(
//         number, 
//         image/* ? image->clone() : nullptr*/, 
//         metadata
//     );
// }