// Page.h
#ifndef _PAGE_H
#define _PAGE_H

#include "AbstractImage.h"
#include "../infrastructure/ImageProcessor.h" 
#include "../infrastructure/Filter.h" 
#include <QVariant>          // Ajouté pour QVariantMap

class Page {
public:
    int number;              // Correction du type
    AbstractImage* image;
    QVariantMap metadata;    // Dépend de QVariant
    Page(): number(0), image(), metadata(){};
    Page(int num, AbstractImage* img, const QVariantMap& meta = {});
    
    void applyFilter(ContentType contentType);
    void resize(int width, int height);
    Page* clone() const;
    ~Page() { delete image; } // Destructeur
    
    Page(const Page& other) : // Constructeur de copie
        number(other.number),
        image(other.image ? other.image->clone() : nullptr),
        metadata(other.metadata) {}
        
    Page& operator=(const Page& other) { // Opérateur d'affectation
        if(this != &other) {
            delete image;
            number = other.number;
            image = other.image->clone();
            metadata = other.metadata;
        }
        return *this;
    }
};
#endif //_PAGE_H