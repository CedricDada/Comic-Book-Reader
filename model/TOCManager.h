/**
 * Project Untitled
 */


#ifndef _TOCMANAGER_H
#define _TOCMANAGER_H

class TOCManager {
public: 
    void bookTOCMap;
    
/**
 * @param book
 * @param pageNumber
 * @param description
 */
void addEntry(AbstractBook book, int pageNumber, String description);
    
/**
 * @param book
 * @param pageNumber
 */
void removeEntry(AbstractBook book, int pageNumber);
    
/**
 * @param book
 * @param pageNumber
 * @param newDescription
 */
void updateEntry(AbstractBook book, int pageNumber, String newDescription);
    
/**
 * @param book
 */
void getTOC(AbstractBook book);
    
/**
 * @param book
 */
void createTOC(AbstractBook book);
    
/**
 * @param book
 */
void deleteTOC(AbstractBook book);
};

#endif //_TOCMANAGER_H