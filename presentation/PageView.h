#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include "../model/AbstractImage.h"
#include "../model/Page.h"
#include "../model/CacheManager.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtConcurrent>
#include <QImage>
#include <QMetaType>

class CacheManager;
class ImageProcessor;

class PageView : public QGraphicsView {
    Q_OBJECT
public:
    void setCacheManager(CacheManager* manager) { 
        m_cacheManager = manager; 
    }
    explicit PageView(QWidget* parent = nullptr);
    
    int currentPageNumber() const { return m_currentPage; }
    // Méthodes existantes (non modifiées)
    void render(const Page& page);
    void setZoom(float level);
    void toggleDualPageMode();
    
    // Getters existants
    float zoomLevel() const;
    bool isDualPageMode() const;
    int currentPage() const { return m_currentPage; }
    void setCurrentPage(int page) { m_currentPage = page; }


    void displayPage(const Page& page);
    QImage loadImageFromData(const QByteArray& data);

public slots:
    void displayPageAsync(const Page& page);
    void updateDisplay(const Page& page);

private:
    int m_currentPage;
    float m_zoomLevel;
    bool m_dualPageMode;
    QGraphicsScene* m_scene;
    
    // Nouveau membre pour le cache
    CacheManager* m_cacheManager;
};

#endif // PAGEVIEW_H