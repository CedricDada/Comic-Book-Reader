#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include "AbstractImage.h"
#include "../infrastructure/ImageProcessor.h" 
#include "../infrastructure/Filter.h" 
#include <QVariant>          // Ajouté pour QVariantMap

class AbstractImage;
class ImageProcessor;
class Filter;

class Page {
public:
    int number;
    std::shared_ptr<AbstractImage> image;
    QByteArray rawData; 
    QVariantMap metadata;

    // Constructeur par défaut
    Page() : number(0), image(), rawData(), metadata() {}

    // Constructeur paramétré
    Page(int num, AbstractImage* img, const QVariantMap& meta = QVariantMap()) :
        number(num),
        image(img ? img->clone() : nullptr),
        metadata(meta) {}

    // Constructeur avec rawData
    Page(int num, const QByteArray& data, const QVariantMap& meta = QVariantMap()) 
        : number(num), 
        image(nullptr), // <- Initialisation explicite
        rawData(data), 
        metadata(meta) {}

    // Constructeur de copie
    Page(const Page& other) :
        number(other.number),
        image(other.image ? other.image->clone() : nullptr),
        rawData(other.rawData), // Copie de rawData
        metadata(other.metadata) {}

    // Opérateur d'affectation
    Page& operator=(const Page& other) {
        if (this != &other) {
            number = other.number;
            image.reset(other.image ? other.image->clone() : nullptr);
            rawData = other.rawData; // Copie de rawData
            metadata = other.metadata;
        }
        return *this;
    }

    // Méthodes utilitaires
    void applyFilter(ContentType contentType);
    void resize(int width, int height);
};
#endif // PAGE_H