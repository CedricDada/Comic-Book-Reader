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
    QFuture<void> future = QtConcurrent::run([this, path](){
        try {
            std::unique_ptr<AbstractBook> book = createBook(path);
            book->loadRawPages(); // Nouvelle méthode qui ne crée pas de QImage
            // Debug : vérifier les données avant transfert
            qDebug() << "Données brutes avant transfert :";
            for (const Page& page : book->pages()) {
                qDebug() << "Page" << page.number << " - Taille rawData :" << page.rawData.size();
            }
            
            QMetaObject::invokeMethod(this, [this, book = book.release()]() {
                emit bookReady(book); // Émission dans le thread principal
            }, Qt::QueuedConnection);
        }
        catch (const std::exception& e) {
            qWarning() << "Error opening book:" << e.what();
            emit openFailed(e.what());
        }
    });
    Q_UNUSED(future);
}

std::unique_ptr<AbstractBook> BookManager::createBook(const QString& path) {
    QFileInfo fi(path);
    QString ext = fi.suffix().toLower();
    
    if(ext == "cbr") return std::make_unique<CBRBook>(path);
    if(ext == "cbz") return std::make_unique<CBZBook>(path);
    if(ext == "pdf") return std::make_unique<PDFBook>(path);
    
    throw std::invalid_argument("Format non supporté: " + ext.toStdString());
}