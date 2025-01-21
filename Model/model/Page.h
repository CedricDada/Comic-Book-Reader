/**
 * Project Untitled
 */


#ifndef _PAGE_H
#define _PAGE_H

class Page {
public: 
    void number;
    void image;
    void metadata;
    void Attribute3;
    
/**
 * @param filter
 */
void applyFilter(Filter filter);
    
/**
 * @param width 
 * @param height
 */
void resize(int width , int height);
};

#endif //_PAGE_H