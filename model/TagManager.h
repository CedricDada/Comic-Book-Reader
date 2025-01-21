/**
 * Project Untitled
 */


#ifndef _TAGMANAGER_H
#define _TAGMANAGER_H

class TagManager {
public: 
    void tags;
    void bookTags;
    
/**
 * @param tag
 */
void addTag(Tag tag);
    
/**
 * @param tagName
 */
void removeTag(String tagName);
    
void getTags();
    
/**
 * @param name
 */
void findTagByName(String name);
    
/**
 * @param book
 * @param tag
 */
void assignTagToBook(AbstractBook book, Tag tag);
    
/**
 * @param book
 * @param tag
 */
void removeTagFromBook(AbstractBook book, Tag tag);
    
/**
 * @param book
 */
void getTagsForBook(AbstractBook book);
};

#endif //_TAGMANAGER_H