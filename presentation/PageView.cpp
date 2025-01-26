#include "PageView.h"
#include "../model/CacheManager.h"
#include <QGraphicsPixmapItem>
#include <QDebug> // Ajout pour le débogage

PageView::PageView(QWidget* parent) // Modification ici
    : QGraphicsView(parent), 
      m_currentPage(0),
      m_zoomLevel(1.0),
      m_dualPageMode(false),
      m_cacheManager(new CacheManager(1024)),
      m_scene(new QGraphicsScene(this)) // Initialisation correcte
{
    setScene(m_scene);
}

// void PageView::render(const AbstractImage& page) {
//     m_scene->clear();
    
//     // Conversion du vector<uint8_t> vers QByteArray
//     QByteArray imageData = QByteArray::fromRawData(
//         reinterpret_cast<const char*>(page.data().data()), 
//         static_cast<int>(page.data().size())
//     );
    
//     QPixmap pixmap;
//     if (pixmap.loadFromData(imageData, page.format().c_str())) {
//         m_scene->addPixmap(pixmap);
//     } else {
//         qWarning() << "Échec du chargement de l'image";
//     }
float PageView::zoomLevel() const {
    return m_zoomLevel;
}

void PageView::setZoom(float level) {
    m_zoomLevel = level;
    resetTransform();
    scale(level, level);
}

void PageView::displayPageAsync(const Page& page) {
    if (!m_cacheManager) {
        qCritical() << "CacheManager non initialisé !";
        return;
    }
    QFuture<void> future = QtConcurrent::run([=](){
        try {
            Page cachedPage = m_cacheManager->getPage(page.number);
            
            // Si la page du cache est vide ou nécessite un traitement
            if (cachedPage.rawData.isEmpty() && cachedPage.image == nullptr) {
                QByteArray processedData = ImageProcessor::processRawData(page.rawData);
                Page processedPage(page.number, processedData, page.metadata);
                m_cacheManager->storePage(page.number, processedPage);
                cachedPage = processedPage;
            }

            qDebug() << "CachedPage" << cachedPage.number << " - Taille rawData :" << cachedPage.rawData.size();

            QMetaObject::invokeMethod(this, [=](){
                updateDisplay(cachedPage); // Utilisez updateDisplay avec vérifications
            }, Qt::QueuedConnection);
        }
        catch(const std::exception& e) {
            qWarning() << "Erreur chargement page:" << e.what();
        }
    });
    Q_UNUSED(future);
}
void PageView::updateDisplay(const Page& page) {
    if (page.rawData.isEmpty() && !page.image) {
        qWarning() << "Données manquantes pour la page" << page.number;
        return;
    }

    QImage qimg;
    if (!page.rawData.isEmpty()) {
        qimg.loadFromData(page.rawData);
    } else if (page.image) {
        qimg = page.image->toQImage();
    }

    if (qimg.isNull()) {
        qCritical() << "Échec du chargement de l'image";
        return;
    }

    m_scene->clear();
    QGraphicsPixmapItem* item = m_scene->addPixmap(QPixmap::fromImage(qimg));
    item->setTransformationMode(Qt::SmoothTransformation);
    fitInView(item, Qt::KeepAspectRatio);
}
// }
void PageView::render(const Page& page) {
    if(page.rawData.isEmpty()) {
        qCritical() << "Données brutes vides !";
        return;
    }

    // Conversion EXCLUSIVE dans le thread principal
    QImage qimg;
    if(!qimg.loadFromData(page.rawData)) {
        qCritical() << "Échec du chargement QImage";
        return;
    }

    m_scene->clear();
    QGraphicsPixmapItem* item = m_scene->addPixmap(QPixmap::fromImage(qimg));
    item->setTransformationMode(Qt::SmoothTransformation);
}
void PageView::displayPage(const Page& page) {
    if(page.rawData.isEmpty()) {
        qWarning() << "Page" << page.number << "non disponible";
        return;
    }

    QImage qimg;
    if(qimg.loadFromData(page.rawData)) {
        m_scene->clear();
        QGraphicsPixmapItem* item = m_scene->addPixmap(QPixmap::fromImage(qimg));
        item->setTransformationMode(Qt::SmoothTransformation);
    }
}