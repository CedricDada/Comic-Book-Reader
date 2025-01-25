#ifndef _CACHEMANAGER_H
#define _CACHEMANAGER_H

#include <QCache>
#include <QList>
#include <QMutex>
#include "Page.h"

class CacheManager {
public:
    explicit CacheManager(int maxSize = 100);
    
    void storePage(int number, const Page& page);
    Page getPage(int number) const;
    void clear();

private:
    int calculateCost(const Page& page) const;
    void enforceCacheLimits();

    QCache<int, Page> m_cache;
    QList<int> m_accessOrder;
    mutable QMutex m_mutex;
    int m_maxSize;
};

#endif //_CACHEMANAGER_H