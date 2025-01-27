#include "PDFBook.h"
#include "QImageAdapter.h"

PDFBook::PDFBook(const QString& path) {
    m_filePath = path;
    m_document = Poppler::Document::load(path); // Assignation directe
}

void PDFBook::loadPages() {
    if(!m_document || m_document->isLocked()) {
        throw std::runtime_error("PDF corrompu ou protégé");
    }

    for(int i = 0; i < m_document->numPages(); ++i) {
        std::unique_ptr<Poppler::Page> page(m_document->page(i));
        QImage image = page->renderToImage(200.0, 200.0); // Résolution 200 DPI
        
        m_pages.append(Page(
            i,
            new QImageAdapter(image),
            {{"source", m_filePath}, {"page", i+1}}
        ));
    }
}
void PDFBook::loadRawPages() {
    if(!m_document || m_document->isLocked()) {
        throw std::runtime_error("PDF corrompu ou protégé");
    }

    m_pages.clear();
    for(int i = 0; i < m_document->numPages(); ++i) {
        std::unique_ptr<Poppler::Page> pdfPage(m_document->page(i));
        
        // Rendu en QImage
        QImage image = pdfPage->renderToImage(200.0, 200.0);
        
        // Conversion en données brutes JPEG
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "JPEG", 80);
        
        Page page;
        page.number = i;
        page.rawData = imageData;
        page.metadata.insert("source", m_filePath);
        page.metadata.insert("page", i + 1);
        m_pages.append(page);
    }
    
}

// Implémentations minimales pour les méthodes abstraites
void PDFBook::addPage(const Page& page) { /*...*/ }
void PDFBook::removePage(int index) { /*...*/ }
Page PDFBook::getPage(int index) const {
    if(index < 0 || index >= m_pages.size()) {
        throw std::out_of_range("Index de page invalide");
    }
    return m_pages[index];
}
void PDFBook::save(const QString& path) { /*...*/ }
int PDFBook::pageCount() const {
    return m_pages.size();
}

QVariantMap PDFBook::metadata() const {
    return m_metadata;
}

void PDFBook::setMetadata(const QVariantMap& meta) {
    m_metadata = meta;
}