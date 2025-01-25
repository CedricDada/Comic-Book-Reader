// Page.h
#ifndef _PAGE_H
#define _PAGE_H

#include "AbstractImage.h"
#include "../infrastructure/ImageProcessor.h"  // Ajouté
#include <QVariant>          // Ajouté pour QVariantMap

class Page {
public:
    int number;              // Correction du type
    AbstractImage* image;
    QVariantMap metadata;    // Dépend de QVariant
    
    Page(int num, AbstractImage* img, const QVariantMap& meta = {});
    
    void applyFilter(const Filter& filter);
    void resize(int width, int height);
    Page* clone() const;
};

#endif //_PAGE_H