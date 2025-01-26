#include "PageView.h"
#include <QGraphicsPixmapItem>
#include <qgraphicsview.h>
#include <QWheelEvent>

PageView::PageView(QWidget* parent) // Modification ici
    : QGraphicsView(parent), 
      m_currentImage(0),
      m_zoomLevel(1.0),
      m_dualPageMode(false),
      m_scene(new QGraphicsScene(this)) // Initialisation correcte
{
    setScene(m_scene);
    setDragMode(QGraphicsView::ScrollHandDrag); // Mode de déplacement à la souris CLIC DROIT
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse); // Zoom centré sur la souris
    setRenderHint(QPainter::SmoothPixmapTransform); // Lissage de l'image
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

void PageView::setZoom(float zoom) {
    double newZoom = zoom / 100.0;
    double factor = newZoom / m_zoomLevel;
    scale(factor, factor);
    m_zoomLevel = newZoom;
}


void PageView::zoomIn() {
    if(m_zoomLevel < 3.0) {
        scale(1.0 + ZOOM_STEP, 1.0 + ZOOM_STEP);
        m_zoomLevel *= (1.0 + ZOOM_STEP);
        emit zoomChanged(m_zoomLevel * 100); // Convertir en pourcentage
    }
}

// void PageView::zoomOut() {
//     setZoom(m_zoomLevel*0.9);
// }

void PageView::zoomOut() {
    if(m_zoomLevel > 0.2) {
        scale(1.0 - ZOOM_STEP, 1.0 - ZOOM_STEP);
        m_zoomLevel *= (1.0 - ZOOM_STEP);
        emit zoomChanged(m_zoomLevel * 100);
    }
}


void PageView::resetZoom() {
    setZoom(1);
}

int PageView::getZoomLevel() const {
    return m_zoomLevel*100;
}
