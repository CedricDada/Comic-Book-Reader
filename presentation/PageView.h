#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include "../model/AbstractImage.h"
#include "../model/Page.h"
#include "../model/CacheManager.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtConcurrent>

class CacheManager;
class ImageProcessor;

class PageView : public QGraphicsView {
    Q_OBJECT
public:
    explicit PageView(QWidget* parent = nullptr);
    
    // Méthodes existantes (non modifiées)
    void render(const AbstractImage& page);
    void setZoom(float level);
    void toggleDualPageMode();
    
    // Getters existants
    int currentPage() const;
    float zoomLevel() const;
    bool isDualPageMode() const;

    // Nouvelle méthode de mise à jour
    void updateDisplay(const QImage& image);

public slots:
    void displayPageAsync(const Page& page);

private:
    int m_currentPage;
    float m_zoomLevel;
    bool m_dualPageMode;
    QGraphicsScene* m_scene;
    
    // Nouveau membre pour le cache
    CacheManager* m_cacheManager;
};

#endif // PAGEVIEW_H