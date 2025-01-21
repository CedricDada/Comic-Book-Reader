/**
 * Project Untitled
 */


#ifndef _LIBRARY_H
#define _LIBRARY_H

class Library {
public: 
    void books;
    void tags;
    void storagePath;
    void Attribute1;
    
/**
 * @param book
 */
void addBook(Book book);
    
/**
 * @param book
 */
void removeBook(Book book);
    
/**
 * @param title
 */
void searchBooksByTitle(String title);
    
/**
 * @param book
 * @param tag
 */
void assignTag(Book book, String tag);
    
/**
 * @param book
 * @param tag
 */
void removeTag(Book book, String tag);
    
/**
 * @param tag
 */
void getBooksByTag(String tag);
    
void listAllBooks();
    
void saveLibrary();
    
void loadLibrary();
};

#endif //_LIBRARY_H