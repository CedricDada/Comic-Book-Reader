// ArchiveHandler.h
#ifndef _ARCHIVEHANDLER_H
#define _ARCHIVEHANDLER_H

#include "IFileHandler.h"
#include <QString>
#include <vector>

class ArchiveHandler : public IFileHandler {
public:
    explicit ArchiveHandler(const std::string& path);
    QVector<Page> getInitialPages(int count) override;
    int totalPages() const override;

private:
    std::string m_archivePath;
    std::vector<std::string> m_pageList;
    
    void loadArchiveStructure();

    void loadPdfStructure();
    void loadCbzStructure();
    void loadCbrStructure();
    
    AbstractImage* extractPage(int index) const override;
    AbstractImage* extractPdfPage(int index) const;
    AbstractImage* extractCbzPage(int index) const;
    AbstractImage* extractCbrPage(int index) const;
    
    bool isImageFile(const QString& filename) const override;
};

#endif //_ARCHIVEHANDLER_H