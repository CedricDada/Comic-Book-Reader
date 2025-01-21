/**
 * Project Untitled
 */


#ifndef _PAGELOADER_H
#define _PAGELOADER_H

class PageLoader {
public: 
    void cache;
    void loadingQueue;
    
/**
 * @param book
 */
void loadPage(AbstractBook book);
    
/**
 * @param book
 * @param start
 * @param count
 */
void preloadPages(AbstractBook book, int start, int count);
};

#endif //_PAGELOADER_H