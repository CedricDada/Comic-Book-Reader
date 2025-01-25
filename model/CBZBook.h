#ifndef CBZBOOK_H
#define CBZBOOK_H

#include "AbstractBook.h"
#include <quazip.h>

class CBZBook : public AbstractBook {
public:
    explicit CBZBook(const QString& path);
    
    void addPage(const Page& page) override;
    void removePage(int index) override;
    Page getPage(int index) const override;
    int pageCount() const override;
    QVariantMap metadata() const override;
    void setMetadata(const QVariantMap& meta) override;
    
    void loadPages() override;
    void save(const QString& path) override;

private:
    QuaZip m_zip;
    void sortPagesByNaturalOrder();
};

#endif // CBZBOOK_H