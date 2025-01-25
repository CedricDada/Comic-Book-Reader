#include "ArchiveHandler.h"
#include <QFileInfo>
#include <QDir>
#include <stdexcept>
#include <poppler-qt6.h>
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#include <archive.h>
#include <archive_entry.h>

using namespace std;

ArchiveHandler::ArchiveHandler(const string& path) : m_archivePath(path) {
    loadArchiveStructure();
}

QVector<Page> ArchiveHandler::getInitialPages(int count) {
    QVector<Page> pages;
    const int limit = min(count, totalPages());
    
    for(int i = 0; i < limit; ++i) {
        try {
            Page page = extractPage(i);
            AbstractImage* img = page.image;
            pages.append(Page(i, img, {{"source", QString::fromStdString(m_archivePath)}}));
        } catch (const exception& e) {
            throw runtime_error("Erreur extraction page " + to_string(i) + ": " + e.what());
        }
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
        throw runtime_error("Format non supporté");
    }

    if(m_pageList.empty()) {
        throw runtime_error("Archive vide ou corrompue");
    }
}

AbstractImage* ArchiveHandler::extractPage(int index) const {
    QFileInfo file(QString::fromStdString(m_archivePath));
    QString ext = file.suffix().toLower();

    if(index < 0 || index >= m_pageList.size()) {
        throw out_of_range("Index de page invalide");
    }

    if(ext == "pdf") {
        return extractPdfPage(index);
    } else if(ext == "cbz") {
        return extractCbzPage(index);
    } else if(ext == "cbr") {
        return extractCbrPage(index);
    }
    return nullptr;
}

// PDF Implementation
void ArchiveHandler::loadPdfStructure() {
    unique_ptr<Poppler::Document> doc(Poppler::Document::load(QString::fromStdString(m_archivePath)));
    
    if(!doc || doc->isLocked()) {
        throw runtime_error("PDF inaccessible ou corrompu");
    }

    const int pageCount = doc->numPages();
    for(int i = 0; i < pageCount; ++i) {
        m_pageList.push_back(to_string(i));
    }
}

AbstractImage* ArchiveHandler::extractPdfPage(int index) const {
    unique_ptr<Poppler::Document> doc(Poppler::Document::load(QString::fromStdString(m_archivePath)));
    Poppler::Page* pdfPage = doc->page(index);
    
    if(!pdfPage) {
        throw runtime_error("Page PDF introuvable");
    }

    QImage image = pdfPage->renderToImage();
    delete pdfPage;
    
    return new QImage(image); // Conversion vers AbstractImage
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
    
    sort(m_pageList.begin(), m_pageList.end()); // Tri alphabétique
}

AbstractImage* ArchiveHandler::extractCbzPage(int index) const {
    QuaZip zip(QString::fromStdString(m_archivePath));
    if(!zip.open(QuaZip::mdUnzip)) {
        throw runtime_error("Ouverture CBZ échouée");
    }

    if(zip.setCurrentFile(QString::fromStdString(m_pageList[index]))) {
        QuaZipFile file(&zip);
        if(file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QImage image;
            if(image.loadFromData(data)) {
                return new QImage(image);
            }
        }
    }
    throw runtime_error("Extraction CBZ échouée");
}

// CBR Implementation
void ArchiveHandler::loadCbrStructure() {
    struct archive* a = archive_read_new();
    archive_read_support_format_all(a);
    
    if(archive_read_open_filename(a, m_archivePath.c_str(), 10240) != ARCHIVE_OK) {
        throw runtime_error("Ouverture CBR échouée");
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

AbstractImage* ArchiveHandler::extractCbrPage(int index) const {
    struct archive* a = archive_read_new();
    archive_read_support_format_all(a);
    
    if(archive_read_open_filename(a, m_archivePath.c_str(), 10240) != ARCHIVE_OK) {
        throw runtime_error("Ouverture CBR échouée");
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
                archive_read_close(a);
                archive_read_free(a);
                return new QImage(image);
            }
        }
    }
    
    archive_read_close(a);
    archive_read_free(a);
    throw runtime_error("Page CBR introuvable");
}


bool ArchiveHandler::isImageFile(const QString& filename) const {
    static const vector<string> extensions = {".jpg", ".jpeg", ".png", ".bmp"};
    QString qname = QString::fromStdString(filename).toLower();
    return any_of(extensions.begin(), extensions.end(),
        [&qname](const string& ext) { return qname.endsWith(QString::fromStdString(ext)); });
}