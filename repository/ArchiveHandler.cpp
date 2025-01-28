#include "ArchiveHandler.h"
#include "../model/QImageAdapter.h"
#include <QFileInfo>
#include <QDir>
#include <stdexcept>
#include <poppler-qt6.h>
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#include <archive.h>
#include <archive_entry.h>

ArchiveHandler::ArchiveHandler(const std::string& path) : m_archivePath(path) {
    loadArchiveStructure();
}

QVector<Page> ArchiveHandler::getInitialPages(int count) {
    QVector<Page> pages;
    for(int i = 0; i < qMin(count, totalPages()); ++i) {
        pages.append(extractPage(i)); 
    }
    return pages;
}

int ArchiveHandler::totalPages() const {
    return m_pageList.size();
}

// Implementation privée
void ArchiveHandler::loadArchiveStructure() {
    QFileInfo file(QString::fromStdString(m_archivePath));
    QString ext = file.suffix().toLower();

    if(ext == "pdf") {
        loadPdfStructure();
    } else if(ext == "cbz") {
        loadCbzStructure();
    } else if(ext == "cbr") {
        loadCbrStructure();
    } else {
        throw std::runtime_error("Format non supporté");
    }

    if(m_pageList.empty()) {
        throw std::runtime_error("Archive vide ou corrompue");
    }
}

Page ArchiveHandler::extractPage(int index) const {
    QFileInfo file(QString::fromStdString(m_archivePath));
    QString ext = file.suffix().toLower();

    if(index < 0 || index >= m_pageList.size()) {
        throw std::out_of_range("Index de page invalide");
    }

    if(ext == "pdf") return extractPdfPage(index);
    if(ext == "cbz") return extractCbzPage(index);
    if(ext == "cbr") return extractCbrPage(index);
    throw std::runtime_error("Format non supporté");
}

// PDF Implementation
void ArchiveHandler::loadPdfStructure() {
    std::unique_ptr<Poppler::Document> doc(Poppler::Document::load(QString::fromStdString(m_archivePath)));
    
    if(!doc || doc->isLocked()) {
        throw std::runtime_error("PDF inaccessible ou corrompu");
    }

    const int pageCount = doc->numPages();
    for(int i = 0; i < pageCount; ++i) {
        m_pageList.push_back(std::to_string(i));
    }
}

Page ArchiveHandler::extractPdfPage(int index) const {
    auto doc = Poppler::Document::load(QString::fromStdString(m_archivePath));
    if (!doc || doc->isLocked()) throw std::runtime_error("PDF inaccessible");

    auto pdfPage = std::unique_ptr<Poppler::Page>(doc->page(index)); // Gestion mémoire
    QImage image = pdfPage->renderToImage();
    
    return Page(
        index,
        new QImageAdapter(image), // Supprimer la ligne avec 'adapter' redondant
        {{"source", QString::fromStdString(m_archivePath)}, {"type", "PDF"}}
    );
}

// CBZ Implementation
void ArchiveHandler::loadCbzStructure() {
    QuaZip zip(QString::fromStdString(m_archivePath));
    if(!zip.open(QuaZip::mdUnzip)) return;

    QuaZipFileInfo info;
    for(bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
        if(zip.getCurrentFileInfo(&info) && isImageFile(info.name)) {
            m_pageList.push_back(info.name.toStdString());
        }
    }
    zip.close();
    
    sort(m_pageList.begin(), m_pageList.end());
}

Page ArchiveHandler::extractCbzPage(int index) const {
    QuaZip zip(QString::fromStdString(m_archivePath));
    if(!zip.open(QuaZip::mdUnzip)) throw std::runtime_error("Ouverture CBZ échouée");

    if(zip.setCurrentFile(QString::fromStdString(m_pageList[index]))) {
        QuaZipFile file(&zip);
        if(file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QImage image;
            if(image.loadFromData(data)) {
                return Page(
                    index,
                    new QImageAdapter(image),
                    {{"source", QString::fromStdString(m_pageList[index])}, {"type", "CBZ"}}
                );
            }
        }
    }
    throw std::runtime_error("Erreur CBZ");
}

// CBR Implementation
void ArchiveHandler::loadCbrStructure() {
    struct archive* a = archive_read_new();
    archive_read_support_format_all(a);
    
    if(archive_read_open_filename(a, m_archivePath.c_str(), 10240) != ARCHIVE_OK) {
        throw std::runtime_error("Ouverture CBR échouée");
    }

    struct archive_entry* entry;
    while(archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        const char* filename = archive_entry_pathname(entry);
        if(isImageFile(filename)) {
            m_pageList.push_back(filename);
        }
    }
    archive_read_close(a);
    archive_read_free(a);
    
    sort(m_pageList.begin(), m_pageList.end());
}

Page ArchiveHandler::extractCbrPage(int index) const {
    struct archive* a = archive_read_new();
    archive_read_support_format_all(a);
    
    if(archive_read_open_filename(a, m_archivePath.c_str(), 10240) != ARCHIVE_OK) {
        throw std::runtime_error("Ouverture CBR échouée");
    }

    struct archive_entry* entry;
    int currentIndex = 0;
    while(archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        if(currentIndex++ == index) {
            const void* buff;
            size_t size;
            int64_t offset;
            
            QByteArray data;
            while(archive_read_data_block(a, &buff, &size, &offset) == ARCHIVE_OK) {
                data.append(static_cast<const char*>(buff), size);
            }
            
            QImage image;
            if(image.loadFromData(data)) {
                return Page(
                    index,
                    new QImageAdapter(image),
                    {{"source", QString::fromStdString(m_pageList[index])}, {"type", "CBR"}}
                );
            }
        }
    }
    
    archive_read_close(a);
    archive_read_free(a);
    throw std::runtime_error("Page CBR introuvable");
}


bool ArchiveHandler::isImageFile(const QString& filename) const {
    QString qname = filename.toLower();
    return qname.endsWith(".jpg") || qname.endsWith(".jpeg") || 
           qname.endsWith(".png") || qname.endsWith(".bmp");
}