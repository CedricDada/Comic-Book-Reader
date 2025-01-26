#ifndef PDFBOOK_H
#define PDFBOOK_H

#include "AbstractBook.h"
#include <poppler-qt6.h>

class PDFBook : public AbstractBook {
public:
    explicit PDFBook(const QString& path);
    
    void addPage(const Page& page) override;
    void removePage(int index) override;
    Page getPage(int index) const override;
    int pageCount() const override;
    
    QVariantMap metadata() const override;
    void setMetadata(const QVariantMap& meta) override;
    
    void loadPages() override;
    void loadRawPages() override;
    void save(const QString& path) override;

private:
    std::unique_ptr<Poppler::Document> m_document;
    void extractPDFTextLayer();
};

#endif // PDFBOOK_H