#ifndef ARCHIVEHANDLER_H
#define ARCHIVEHANDLER_H

#include <string>
#include <vector>
#include <memory>
#include <zip.h>
#include <archive.h>
#include <poppler/cpp/poppler-document.h>

namespace fs = std::filesystem;

class ArchiveHandler {
public:
    virtual ~ArchiveHandler() = default;

    // Factory method
    static std::unique_ptr<ArchiveHandler> create(const std::string& filePath);

    // Interface ToC
    virtual std::string readTableOfContents() = 0;
    virtual void writeTableOfContents(const std::string& tocContent) = 0;
    
    // Méthodes existantes
    virtual void extract(const std::string& outputPath) = 0;
    virtual void compress(const std::vector<std::string>& files) = 0;

protected:
    virtual std::string locateTOCTag() const { return "toc.xml"; }
};

//---------------------------------------------------------
// Implémentation CBZ
//---------------------------------------------------------
class CBZHandler : public ArchiveHandler {
public:
    explicit CBZHandler(const std::string& path);
    ~CBZHandler();

    std::string readTableOfContents() override;
    void writeTableOfContents(const std::string& tocContent) override;
    
    void extract(const std::string& outputPath) override;
    void compress(const std::vector<std::string>& files) override;

private:
    zip_t* m_archive;
    std::string m_path;
};



//---------------------------------------------------------
// Implémentation CBR
//---------------------------------------------------------
class CBRHandler : public ArchiveHandler {
public:
    explicit CBRHandler(const std::string& path);
    ~CBRHandler();

    std::string readTableOfContents() override;
    void writeTableOfContents(const std::string& tocContent) override;
    
    void extract(const std::string& outputPath) override;
    void compress(const std::vector<std::string>& files) override;

private:
    struct archive* m_archive;
    std::string m_path;
};

//---------------------------------------------------------
// Implémentation pdf
//---------------------------------------------------------
class PDFHandler : public ArchiveHandler {
public:
    explicit PDFHandler(const std::string& path);
    ~PDFHandler();

    std::string readTableOfContents() override;
    void writeTableOfContents(const std::string& tocContent) override;
    
    void extract(const std::string& outputPath) override;
    void compress(const std::vector<std::string>& files) override;

private:
    poppler::document* m_doc;
    std::string m_path;
    
    void parsePdfOutline(const poppler::outline_item* item, int level, std::string& toc);
};
#endif // ARCHIVEHANDLER_H