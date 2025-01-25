// PLoader.h
#ifndef PLOADER_H
#define PLOADER_H

#include <QObject>
#include "AbstractBook.h"
#include "../infrastructure/Filter.h"

class PLoader : public QObject {
    Q_OBJECT

public:
    explicit PLoader(QObject* parent = nullptr);
    ~PLoader() override;

    void preloadPages(AbstractBook* book, int start, int count, ContentType contentType);
    void processImage(AbstractImage* image, ContentType contentType);

signals:
    void pageLoaded(int index, AbstractImage* image);
    void loadError(int pageNumber, const QString& error); // Ajout du signal

private:
    int m_targetDpi; // Déclaration du membre manquant
    ContentType detectContentType(AbstractImage* image); // Déclaration de la méthode
};

#endif // PLOADER_H