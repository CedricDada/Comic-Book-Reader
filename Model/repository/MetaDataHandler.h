/**
 * Project Untitled
 */


#ifndef _METADATAHANDLER_H
#define _METADATAHANDLER_H

class MetaDataHandler {
public: 
    void metaDataMap;
    void filePath;
    
void readMetaData();
    
void writeMetaData();
    
/**
 * @param key
 */
void getMetaData(String key);
    
/**
 * @param key
 * @param value
 */
void setMetaData(String key, String value);
    
/**
 * @param key
 */
void removeMetaData(String key);
};

#endif //_METADATAHANDLER_H