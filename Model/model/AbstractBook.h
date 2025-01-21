/**
 * Project Untitled
 */


#ifndef _ABSTRACTBOOK_H
#define _ABSTRACTBOOK_H

class AbstractBook {
public: 
    void title;
    void author;
    void tags;
    void pages;
    void tableOfContents;
    
/**
 * @param page
 */
void addPage(Page page);
    
/**
 * @param pageNumber
 */
void removePage(int pageNumber);
    
/**
 * @param number
 */
void getPage(int number);
};

#endif //_ABSTRACTBOOK_H