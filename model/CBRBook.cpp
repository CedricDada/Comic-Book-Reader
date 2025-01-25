#include "CBRBook.h"
#include "QImageAdapter.h"
#include "Page.h"
#include <archive.h>
#include <archive_entry.h>
#include <QImage>
#include <QFileInfo>
#include <stdexcept>
#include <algorithm>

CBRBook::CBRBook(const QString& path) {
    m_filePath = path;
    m_archive = archive_read_new();
    archive_read_support_format_all(m_archive);
}

CBRBook::~CBRBook() {
    archive_read_close(m_archive);
    archive_read_free(m_archive);
}

void CBRBook::loadPages() {
    if(archive_read_open_filename(m_archive, m_filePath.toUtf8().constData(), 10240) != ARCHIVE_OK) {
        throw std::runtime_error("Failed to open CBR file: " + m_filePath.toStdString());
    }

    struct archive_entry* entry;
    QVector<QString> imagePaths;
    
    // First pass: collect image entries
    while(archive_read_next_header(m_archive, &entry) == ARCHIVE_OK) {
        const char* filename = archive_entry_pathname(entry);
        QString qFilename = QString::fromUtf8(filename);
        
        if(isImageFile(qFilename)) {
            imagePaths.append(qFilename);
        }
        archive_read_data_skip(m_archive);
    }

    // Natural sort for comic book page order
    std::sort(imagePaths.begin(), imagePaths.end(), [](const QString& a, const QString& b) {
        return QString::compare(a, b, Qt::CaseInsensitive) < 0;
    });

    // Second pass: extract and load images
    for(const QString& filename : imagePaths) {
        archive_read_free(m_archive);
        m_archive = archive_read_new();
        archive_read_support_format_all(m_archive);
        archive_read_open_filename(m_archive, m_filePath.toUtf8().constData(), 10240);

        while(archive_read_next_header(m_archive, &entry) == ARCHIVE_OK) {
            if(QString::fromUtf8(archive_entry_pathname(entry)) == filename) {
                QByteArray data;
                const void* buff;
                size_t size;
                int64_t offset;

                while(archive_read_data_block(m_archive, &buff, &size, &offset) == ARCHIVE_OK) {
                    data.append(static_cast<const char*>(buff), size);
                }

                QImage img;
                if(img.loadFromData(data)) {
                    Page page;
                    page.number = m_pages.size();
                    page.image = new QImageAdapter(img);
                    page.metadata.insert("source", filename);
                    m_pages.append(page);
                }
                break;
            }
        }
    }
}

// Basic implementation for abstract methods
void CBRBook::addPage(const Page& page) {
    m_pages.append(page);
}

void CBRBook::removePage(int index) {
    if(index < 0 || index >= m_pages.size()) {
        throw std::out_of_range("Invalid page index");
    }
    m_pages.remove(index);
}

Page CBRBook::getPage(int index) const {
    if(index < 0 || index >= m_pages.size()) {
        throw std::out_of_range("Page index out of range");
    }
    return m_pages.at(index);
}

QVariantMap CBRBook::metadata() const {
    // Implement metadata extraction from archive if needed
    return m_metadata;
}

void CBRBook::setMetadata(const QVariantMap& meta) {
    m_metadata = meta;
}

void CBRBook::save(const QString& path) {
    // Implementation would require RAR archiving which is non-trivial
    throw std::runtime_error("CBR saving not implemented");
}

// Helper method
bool CBRBook::isImageFile(const QString& filename) const {
    QString ext = QFileInfo(filename).suffix().toLower();
    return ext == "jpg" || ext == "jpeg" || ext == "png" || ext == "bmp";
}
int CBRBook::pageCount() const {
    return m_pages.size();
}