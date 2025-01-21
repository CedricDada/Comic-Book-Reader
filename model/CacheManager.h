/**
 * Project Untitled
 */


#ifndef _CACHEMANAGER_H
#define _CACHEMANAGER_H

class CacheManager {
public: 
    void cacheSize;
    void cachedPages;
    void Attribute1;
    
/**
 * @param page
 */
void storePage(Page page);
    
/**
 * @param pageNumber
 */
void getPage(int pageNumber);
};

#endif //_CACHEMANAGER_H