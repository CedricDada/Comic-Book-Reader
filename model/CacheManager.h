// CacheManager.h
#ifndef _CACHEMANAGER_H
#define _CACHEMANAGER_H

#include <QCache>
#include "Page.h"

class CacheManager {
public:
    explicit CacheManager(int maxSize = 100);
    
    void storePage(int number, const QImage& image);
    QImage getPage(int number) const;

private:
    QCache<int, QImage> m_cache;
};

#endif //_CACHEMANAGER_H