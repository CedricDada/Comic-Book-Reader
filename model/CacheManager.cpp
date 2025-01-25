#include "CacheManager.h"
#include <QList>
#include <QMutexLocker>

CacheManager::CacheManager(int maxSize) : 
    m_maxSize(maxSize),
    m_currentSize(0) {
    m_cache.setMaxCost(maxSize);
}

void CacheManager::storePage(const QImage& page) {
    QMutexLocker locker(&m_mutex);
    const int key = page.number;
    
    if(m_cache.contains(key)) {
        m_cache.remove(key); // Supprime l'ancienne version
    }
    
    Page* pageCopy = new Page(page);
    pageCopy->image = page.image;/*->clone();*/ // Clone profond de l'image
    
    m_cache.insert(key, pageCopy, calculateCost(*pageCopy));
}

Page CacheManager::getPage(int pageNumber) {
    QMutexLocker locker(&m_mutex);
    
    if(Page* cached = m_cache[pageNumber]) {
        m_accessOrder.removeAll(pageNumber);
        m_accessOrder.prepend(pageNumber);
        return *cached; 
    }
    
    throw std::runtime_error("Page non trouvée dans le cache");
}

void CacheManager::clear() {
    QMutexLocker locker(&m_mutex);
    m_cache.clear();
    m_accessOrder.clear();
}

// Implémentation privée
int CacheManager::calculateCost(const Page& page) const {
    // 1 unité = 1 Mo de mémoire approximative
    const int baseCost = 1;
    const int sizeInMB = (page.image->byteCount() / (1024 * 1024)) + 1;
    return baseCost * sizeInMB;
}

void CacheManager::enforceCacheLimits() {
    while(m_cache.totalCost() > m_maxSize) {
        const int lastUsed = m_accessOrder.takeLast();
        m_cache.remove(lastUsed);
    }
}