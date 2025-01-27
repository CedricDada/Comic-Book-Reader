#include "PageView.h"
#include "../model/CacheManager.h"
#include "../model/QImageAdapter.h"
#include <QGraphicsPixmapItem>
#include <QDebug> // Ajout pour le débogage
#include <QWheelEvent>

PageView::PageView(QWidget* parent) // Modification ici
    : QGraphicsView(parent), 
      m_currentPage(0),
      m_zoomLevel(1.0),
      m_dualPageMode(false),
      m_cacheManager(new CacheManager(1024)),
      m_scene(new QGraphicsScene(this)) // Initialisation correcte
{
    setScene(m_scene);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void PageView::wheelEvent(QWheelEvent* event) {
    if(event->modifiers() & Qt::ControlModifier) { // Zoom uniquement si Ctrl enfoncé
        if(event->angleDelta().y() > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
    } else {
        QGraphicsView::wheelEvent(event); // Comportement normal sinon
    }
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

// void PageView::setZoom(float level) {
//     m_zoomLevel = level;
//     resetTransform();
//     scale(level, level);
// }

void PageView::displayPageAsync(const Page& page) {
    if (!m_cacheManager) {
        qCritical() << "CacheManager non initialisé !";
        return;
    }
    QFuture<void> future = QtConcurrent::run([=](){
        try {
            Page cachedPage = m_cacheManager->getPage(page.number);
            
            // Si rawData existe mais image est null
            if(cachedPage.rawData.isEmpty() || !cachedPage.image) {
                if(!page.rawData.isEmpty()) {
                    // Convertir rawData en image
                    QImage qimg = loadImageFromData(page.rawData);
                    if(!qimg.isNull()) {
                        cachedPage.image.reset(new QImageAdapter(qimg));
                    }
                }
                m_cacheManager->storePage(page.number, cachedPage);
            }

            QMetaObject::invokeMethod(this, [=](){
                updateDisplay(cachedPage); // Utiliser la version sécurisée
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
    m_currentPage = page.number;
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
        m_currentPage = page.number; // Mettre à jour la page courante
    }
}
QImage PageView::loadImageFromData(const QByteArray& data) {
    QImage img;
    img.loadFromData(data);
    return img;
}
void PageView::setZoom(float zoom) {
    double newZoom = zoom / 100.0;
    double factor = newZoom / m_zoomLevel;

    // Appliquer la mise à l'échelle et mettre à jour le niveau de zoom
    scale(factor, factor);
    m_zoomLevel = newZoom;

    emit zoomChanged(m_zoomLevel * 100); // Émettre un signal avec le zoom en pourcentage
}

void PageView::zoomIn() {
    if(m_zoomLevel < 5.0) {
        scale(1.0 + ZOOM_STEP, 1.0 + ZOOM_STEP);
        m_zoomLevel *= (1.0 + ZOOM_STEP);
        emit zoomChanged(m_zoomLevel * 100); // Convertir en pourcentage
    }
}

void PageView::zoomOut() {
    if(m_zoomLevel > 0.2) {
        scale(1.0 - ZOOM_STEP, 1.0 - ZOOM_STEP);
        m_zoomLevel *= (1.0 - ZOOM_STEP);
        emit zoomChanged(m_zoomLevel * 100); // Convertir en pourcentage
    }
}

void PageView::resetZoom() {
    setZoom(100.0);
    emit zoomChanged(100);
}

int PageView::getZoomLevel() const {
    return static_cast<int>(m_zoomLevel * 100); // Retourne un entier en pourcentage
}