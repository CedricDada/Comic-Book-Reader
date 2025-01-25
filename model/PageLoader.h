// PageLoader.h
#ifndef _PAGELOADER_H
#define _PAGELOADER_H

#include <QObject>
#include <QQueue>
#include "AbstractBook.h"

class PageLoader : public QObject {
    Q_OBJECT

public:
    explicit PageLoader(QObject* parent = nullptr);
    
    void preloadPages(AbstractBook* book, int start, int count);

signals:
    void pageLoaded(int index, AbstractImage* image);

private:
    QQueue<int> m_loadingQueue;
    
    void processQueue(AbstractBook* book);
};

#endif //_PAGELOADER_H