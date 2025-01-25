#ifndef TOCMANAGER_H
#define TOCMANAGER_H

#include <string>
#include <vector>
#include "ArchiveHandler.h"

struct PageEntry {
    int number;
    std::string title;
    std::string imagePath;
};

class TocManager {
public:
    explicit TocManager(std::unique_ptr<ArchiveHandler> handler);

    void loadTOC();
    void saveTOC();
    
    void addPage(const PageEntry& page);
    void updatePage(int index, const PageEntry& newData);
    
    const std::vector<PageEntry>& getPages() const { return m_pages; }

private:
    std::string serializeTOC() const;
    void parseTOC(const std::string& xmlContent);

    std::unique_ptr<ArchiveHandler> m_archiveHandler;
    std::vector<PageEntry> m_pages;
};

#endif // TOCMANAGER_H