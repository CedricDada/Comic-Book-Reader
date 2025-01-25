#include "CacheManager.h"
#include <QMutexLocker>

CacheManager::CacheManager(int maxSize) : 
    m_maxSize(maxSize),
    m_cache(maxSize) // Initialise QCache avec la taille max
{
}

void CacheManager::storePage(int number, const Page& page) {
    QMutexLocker locker(&m_mutex);
    
    if(m_cache.contains(number)) {
        m_cache.remove(number);
    }
    
    Page* pageCopy = new Page(page);
    m_cache.insert(number, pageCopy, calculateCost(page));
    
    // Mise à jour de l'ordre d'accès
    m_accessOrder.removeAll(number);
    m_accessOrder.prepend(number);
}

Page CacheManager::getPage(int number) const {
    QMutexLocker locker(&m_mutex);
    
    if(Page* cached = m_cache.object(number)) {
        // Mise à jour de l'ordre d'accès
        const_cast<QList<int>&>(m_accessOrder).removeAll(number);
        const_cast<QList<int>&>(m_accessOrder).prepend(number);
        return *cached;
    }
    throw std::runtime_error("Page non trouvée dans le cache");
}

void CacheManager::clear() {
    QMutexLocker locker(&m_mutex);
    m_cache.clear();
    m_accessOrder.clear();
}

int CacheManager::calculateCost(const Page& page) const {
    return page.image->dataSize() / (1024 * 1024); // Coût en MB
}

void CacheManager::enforceCacheLimits() {
    while(m_cache.totalCost() > m_maxSize) {
        int lastUsed = m_accessOrder.takeLast();
        m_cache.remove(lastUsed);
    }
}