#ifndef ABSTRACTBOOK_H
#define ABSTRACTBOOK_H

#include "Page.h"
#include <QVector>
#include <QString>
#include <QVariantMap>

class AbstractBook {
public:
    virtual ~AbstractBook() = default;

    // Gestion des pages
    virtual void addPage(const Page& page) = 0;
    virtual void removePage(int index) = 0;
    virtual Page getPage(int index) const = 0;
    virtual int pageCount() const = 0;

    // Métadonnées
    virtual QVariantMap metadata() const = 0;
    virtual void setMetadata(const QVariantMap& meta) = 0;

    // Persistance
    virtual void loadPages() = 0;
    virtual void save(const QString& path) = 0;

    int totalPages() const { return m_pages.size(); }
    virtual const QVector<Page>& pages() const { return m_pages; }
    virtual Page pageAt(int index) const { return m_pages.at(index); } 

protected:
    QVector<Page> m_pages;
    QVariantMap m_metadata;
    QString m_filePath;
};

#endif // ABSTRACTBOOK_H