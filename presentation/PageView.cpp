#include "PageView.h"
#include <QGraphicsPixmapItem>

PageView::PageView(QWidget* parent)
    : QGraphicsView(parent), 
      m_currentPage(0),
      m_zoomLevel(1.0),
      m_dualPageMode(false),
      m_scene(new QGraphicsScene(this))
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