#include "BookManager.h"
#include "CBRBook.h"
#include "CBZBook.h"
#include "PDFBook.h"
#include <QFileInfo>
#include <QtConcurrent>

BookManager::BookManager(QObject* parent) 
    : QObject(parent), 
      m_cacheManager(1024) // 1GB cache
{}

void BookManager::openBook(const QString& path) {
    QtConcurrent::run([this, path](){
        try {
            std::unique_ptr<AbstractBook> book = createBook(path);
            book->loadPages();
            
            // Récupérer les 5 premières pages
            QVector<Page> initialPages;
            for(int i = 0; i < qMin(5, book->pageCount()); ++i) {
                initialPages.append(book->getPage(i));
            }
            
            // Préchargement asynchrone des pages restantes
            m_PLoader.preloadPages(
                book.get(), 
                initialPages.size(),
                book->pageCount() - initialPages.size(),
                ContentType::AutoDetect
            );
            
            // Émettre le signal avec ownership transféré
            emit bookReady(book.release());
        }
        catch (const std::exception& e) {
            qWarning() << "Error opening book:" << e.what();
            emit openFailed(e.what());
        }
    });
}

std::unique_ptr<AbstractBook> BookManager::createBook(const QString& path) {
    QFileInfo fi(path);
    QString ext = fi.suffix().toLower();
    
    if(ext == "cbr") return std::make_unique<CBRBook>(path);
    if(ext == "cbz") return std::make_unique<CBZBook>(path);
    if(ext == "pdf") return std::make_unique<PDFBook>(path);
    
    throw std::invalid_argument("Format non supporté: " + ext.toStdString());
}