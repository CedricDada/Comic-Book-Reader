#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H 
// BookManager.h
#include <QObject>
#include "AbstractBook.h"
#include "PageLoader.h"
#include "CacheManager.h"

class BookManager : public QObject {
    Q_OBJECT

public:
    explicit BookManager(QObject* parent = nullptr);
    
    void openBook(const QString& path);
    
signals:
    void bookReady(AbstractBook* book);
    void pageProcessed(int index);
    void openFailed(const QString& error);

private:
    PageLoader m_pageLoader;
    CacheManager m_cacheManager;
    
    void setupConnections();
};
#endif 