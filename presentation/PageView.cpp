#include "PageView.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QDebug> // Pour la gestion des avertissements et logs

PageView::PageView(QWidget* parent)
    : QGraphicsView(parent), 
      m_currentImage(0),
      m_zoomLevel(1.0),
      m_dualPageMode(false),
      m_scene(new QGraphicsScene(this))
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

void PageView::render(const AbstractImage& page) {
    m_scene->clear();

    // Fixed line: Convert enum to string first
    QByteArray format = QByteArray::fromStdString(
        AbstractImage::formatToString(page.format())
    );

    QByteArray imageData = QByteArray::fromRawData(
        reinterpret_cast<const char*>(page.data().data()), 
        static_cast<int>(page.data().size())
    );

    QPixmap pixmap;
    if (pixmap.loadFromData(imageData, format.constData())) {
        m_scene->addPixmap(pixmap);
    } else {
        qWarning() << "Échec du chargement de l'image:"
                   << format;
    }
}

float PageView::zoomLevel() const {
    return m_zoomLevel;
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
