#include "PLoader.h"
#include <QtConcurrent>
#include "../infrastructure/ImageProcessor.h"
#include "../infrastructure/Filter.h"

void PLoader::processImage(AbstractImage* image, ContentType contentType) {
    QFuture<void> future = QtConcurrent::run([=](){
        // Application du filtre adapté au type de contenu
        ImageProcessor::applyFilter(image, contentType);
    });
}

void PLoader::preloadPages(AbstractBook* book, int start, int count, ContentType contentType) {
    for(int i = start; i < start + count; ++i) {
        QFuture<void> future = QtConcurrent::run([=](){ // Capture du résultat
            try {
                Page p = book->getPage(i);
                ContentType finalType = contentType;
                if(contentType == ContentType::AutoDetect) {
                    finalType = detectContentType(p.image.get());
                }
                processImage(p.image.get(), finalType);
                emit pageLoaded(i, p.image.get());
            }
            catch(const std::exception& e) {
                emit loadError(i, QString::fromStdString(e.what()));
            }
        });
        Q_UNUSED(future); // Supprime l'avertissement
    }
}

// Implémentation de la détection de type de contenu
ContentType PLoader::detectContentType(AbstractImage* image) {
    QImage qimg = image->toQImage();
    
    // Analyse simplifiée : ratio de bords détectés
    int edgeCount = 0;
    const int threshold = 50;
    
    for(int y = 1; y < qimg.height()-1; y+=2) {
        for(int x = 1; x < qimg.width()-1; x+=2) {
            QRgb current = qimg.pixel(x, y);
            QRgb right = qimg.pixel(x+1, y);
            QRgb bottom = qimg.pixel(x, y+1);
            
            if(qAbs(qGray(current) - qGray(right)) > threshold ||
               qAbs(qGray(current) - qGray(bottom)) > threshold) {
                edgeCount++;
            }
        }
    }
    
    double edgeRatio = static_cast<double>(edgeCount) / (qimg.width() * qimg.height());
    return edgeRatio > 0.05 ? ContentType::GraphicDominant : ContentType::TextDominant;
}

PLoader::PLoader(QObject* parent) 
    : QObject(parent), m_targetDpi(150) {}

PLoader::~PLoader() = default;