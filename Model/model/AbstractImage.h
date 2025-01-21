/**
 * Project Untitled
 */


#ifndef _ABSTRACTIMAGE_H
#define _ABSTRACTIMAGE_H

class AbstractImage {
public: 
    void data;
    void format;
    void width;
    void height;
    void Attribute1;
    
void getWidth();
    
void getHeight();
    
void getFormat();
    
/**
 * @param newWidth
 * @param newHeight
 */
void resize(int newWidth, int newHeight);
    
/**
 * @param filter
 */
void applyFilter(Filter filter);
};

#endif //_ABSTRACTIMAGE_H