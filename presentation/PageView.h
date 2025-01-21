#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include "../model/AbstractImage.h"
#include <QGraphicsScene>
#include <QGraphicsView>

class PageView : public QGraphicsView {
    Q_OBJECT
public:
    explicit PageView(QWidget* parent = nullptr);
    void render(const AbstractImage& page);
    void setZoom(float level);
    void toggleDualPageMode();
    
    // Getters
    int currentPage() const;
    float zoomLevel() const;
    bool isDualPageMode() const;

private:
    int m_currentPage;
    float m_zoomLevel;
    bool m_dualPageMode;
    QGraphicsScene* m_scene;
};

#endif // PAGEVIEW_H