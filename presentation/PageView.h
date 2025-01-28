#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include "../model/AbstractImage.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <qtmetamacros.h>

class PageView : public QGraphicsView {
    Q_OBJECT
public:
    explicit PageView(QWidget* parent = nullptr);
    void render(const AbstractImage& page);
    void setZoom(float level);
    void toggleDualPageMode();
    void setZoomLevel(float zoomLevel);
    
    int currentPage() const;
    float zoomLevel() const;
    bool isDualPageMode() const;
    void resetZoom();
    void zoomIn();
    void zoomOut();
    int getZoomLevel() const;
    int pageCount() const { return m_currentImage ? 1 : 0; } 

signals:
    void zoomChanged(double newZoomLevel);
    
private:
    int m_currentImage;
    float m_zoomLevel=1.0;
    bool m_dualPageMode;
    QGraphicsScene* m_scene;
    const double ZOOM_STEP = 0.1; 

protected:
    void wheelEvent(QWheelEvent* event) override;
};

#endif // PAGEVIEW_H