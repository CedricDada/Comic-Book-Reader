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
        QImage cachedImg = m_cacheManager->getPage(page.number);
        
        if(cachedImg.isNull()) {
            QImage processed = ImageProcessor::processImage(page.image);
            m_cacheManager->storePage(page.number, processed);
            cachedImg = processed;
        }

        QMetaObject::invokeMethod(this, [=](){
            updateDisplay(cachedImg);
        }, Qt::QueuedConnection);
    });
    Q_UNUSED(future);
}

void PageView::updateDisplay(const QImage& image) {
    m_scene->clear();
    QGraphicsPixmapItem* item = m_scene->addPixmap(QPixmap::fromImage(image));
    item->setTransformationMode(Qt::SmoothTransformation);
    fitInView(item, Qt::KeepAspectRatio);
}