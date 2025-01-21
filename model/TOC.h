/**
 * Project Untitled
 */


#ifndef _TOC_H
#define _TOC_H

class TOC {
public: 
    void entries;
    
/**
 * @param pageNumber
 * @param description
 */
void addEntry(int pageNumber, String description);
    
/**
 * @param pageNumber
 */
void removeEntry(int pageNumber);
    
/**
 * @param pageNumber
 * @param description
 */
void updateEntry(int pageNumber, String description);
    
/**
 * @param pageNumber
 */
void getEntry(int pageNumber);
    
void getAllEntries();
};

#endif //_TOC_H