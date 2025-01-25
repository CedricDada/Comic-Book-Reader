#include "TocManager.h"
#include <tinyxml2.h>

using namespace tinyxml2;

TocManager::TocManager(std::unique_ptr<ArchiveHandler> handler) 
    : m_archiveHandler(std::move(handler)) {}

void TocManager::loadTOC() {
    std::string tocContent = m_archiveHandler->readTableOfContents();
    parseTOC(tocContent);
}

void TocManager::saveTOC() {
    std::string tocContent = serializeTOC();
    m_archiveHandler->writeTableOfContents(tocContent);
}

// Sérialisation XML
std::string TocManager::serializeTOC() const {
    XMLDocument doc;
    XMLElement* root = doc.NewElement("toc");
    
    for(const auto& page : m_pages) {
        XMLElement* pageElem = doc.NewElement("page");
        pageElem->SetAttribute("number", page.number);
        pageElem->SetAttribute("title", page.title.c_str());
        pageElem->SetAttribute("path", page.imagePath.c_str());
        root->InsertEndChild(pageElem);
    }
    
    doc.InsertFirstChild(root);
    
    XMLPrinter printer;
    doc.Print(&printer);
    return printer.CStr();
}

// Parsing XML
void TocManager::parseTOC(const std::string& xmlContent) {
    XMLDocument doc;
    doc.Parse(xmlContent.c_str());
    
    XMLElement* root = doc.FirstChildElement("toc");
    if(!root) return;
    
    m_pages.clear();
    
    for(XMLElement* elem = root->FirstChildElement("page"); elem; elem = elem->NextSiblingElement()) {
        PageEntry entry;
        elem->QueryIntAttribute("number", &entry.number);
        entry.title = elem->Attribute("title");
        entry.imagePath = elem->Attribute("path");
        m_pages.push_back(entry);
    }
}