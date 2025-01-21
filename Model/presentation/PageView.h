/**
 * Project Untitled
 */


#ifndef _PAGEVIEW_H
#define _PAGEVIEW_H

class PageView {
public: 
    void currentPage;
    void zoomLevel;
    
/**
 * @param page
 */
void render(Page page);
    
/**
 * @param level
 */
void setZoom(float level);
    
void toogleDualPageMode();
};

#endif //_PAGEVIEW_H