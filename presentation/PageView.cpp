#include "PageView.h"
#include "../model/CacheManager.h"
#include <QGraphicsPixmapItem>

PageView::PageView(QWidget* parent) // Modification ici
    : QGraphicsView(parent), 
      m_currentPage(0),
      m_zoomLevel(1.0),
      m_dualPageMode(false),
      m_scene(new QGraphicsScene(this)) // Initialisation correcte
{
    setScene(m_scene);
}

void PageView::render(const AbstractImage& page) {
    m_scene->clear();
    
    // Conversion du vector<uint8_t> vers QByteArray
    QByteArray imageData = QByteArray::fromRawData(
        reinterpret_cast<const char*>(page.data().data()), 
        static_cast<int>(page.data().size())
    );
    
    QPixmap pixmap;
    if (pixmap.loadFromData(imageData, page.format().c_str())) {
        m_scene->addPixmap(pixmap);
    } else {
        qWarning() << "Échec du chargement de l'image";
    }
}
float PageView::zoomLevel() const {
    return m_zoomLevel;
}

void PageView::setZoom(float level) {
    m_zoomLevel = level;
    resetTransform();
    scale(level, level);
}

void PageView::displayPageAsync(const Page& page) {
    QFuture<void> future = QtConcurrent::run([=](){
        try {
            // 1. Tentative de récupération depuis le cache
            Page cachedPage = m_cacheManager->getPage(page.number);
            
            // 2. Si non trouvé ou besoin de traitement
            if(cachedPage.image == nullptr) {
                // Clone profond pour ne pas modifier l'original
                AbstractImage* processedImage = page.image->clone();
                ImageProcessor::processImage(processedImage);
                
                // Création d'une nouvelle page avec l'image traitée
                Page processedPage(page.number, processedImage, page.metadata);
                
                // Stockage dans le cache
                m_cacheManager->storePage(page.number, processedPage);
                cachedPage = processedPage;
            }

            // 3. Mise à jour UI avec métadonnées
            QMetaObject::invokeMethod(this, [=](){
                updateDisplay(cachedPage); // Maintenant avec Page complète
                //updateMetadataDisplay(cachedPage.metadata); // Nouvelle méthode
            }, Qt::QueuedConnection);
        }
        catch(const std::exception& e) {
            qWarning() << "Erreur chargement page:" << e.what();
        }
    });
    Q_UNUSED(future);
}

void PageView::updateDisplay(const Page& page) {
    QImage image = page.image->toQImage();
    QGraphicsPixmapItem* item = m_scene->addPixmap(QPixmap::fromImage(image));
    item->setTransformationMode(Qt::SmoothTransformation);
    fitInView(item, Qt::KeepAspectRatio);
}