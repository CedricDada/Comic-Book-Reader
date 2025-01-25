#ifndef CBRBOOK_H
#define CBRBOOK_H

#include "AbstractBook.h"
#include <archive.h>

class CBRBook : public AbstractBook {
public:
    explicit CBRBook(const QString& path);
    ~CBRBook() override; 
    
    void addPage(const Page& page) override;
    void removePage(int index) override;
    Page getPage(int index) const override;
    int pageCount() const override;
    
    QVariantMap metadata() const override;
    void setMetadata(const QVariantMap& meta) override;
    
    void loadPages() override;
    void save(const QString& path) override;

private:
    struct archive* m_archive;
    void extractRarContents();
    bool isImageFile(const QString& filename) const; 
};

#endif // CBRBOOK_H