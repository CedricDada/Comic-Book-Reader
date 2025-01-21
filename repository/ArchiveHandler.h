/**
 * Project Untitled
 */


#ifndef _ARCHIVEHANDLER_H
#define _ARCHIVEHANDLER_H

class ArchiveHandler {
public: 
    void archiveType;
    void fileList;
    
/**
 * @param fileName
 */
void extractFile(String fileName);
    
/**
 * @param files
 */
void compressFiles(void files);
};

#endif //_ARCHIVEHANDLER_H