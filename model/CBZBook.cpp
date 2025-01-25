#include "CBZBook.h"
#include "QImageAdapter.h"
#include <quazipfile.h>
#include <QFileInfo>


#include "CBZBook.h"
#include "QImageAdapter.h" // Ajout important
#include <quazipfile.h>
#include <QFileInfo>
#include <algorithm>

CBZBook::CBZBook(const QString& path) {
    m_filePath = path;
    m_zip.setZipName(path);
}

void CBZBook::loadPages() {
    if(!m_zip.open(QuaZip::mdUnzip)) {
        throw std::runtime_error("Échec d'ouverture du fichier CBZ");
    }

    QuaZipFileInfo info;
    QuaZipFile file(&m_zip);
    
    // Première passe : collecter les noms de fichiers
    QList<QString> imageFiles;
    for(bool more = m_zip.goToFirstFile(); more; more = m_zip.goToNextFile()) {
        if(m_zip.getCurrentFileInfo(&info) && info.name.endsWith(".jpg", Qt::CaseInsensitive)) {
            imageFiles.append(info.name);
        }
    }
    
    // Tri naturel des noms de fichiers
    std::sort(imageFiles.begin(), imageFiles.end(), [](const QString& a, const QString& b) {
        return QString::compare(a, b, Qt::CaseInsensitive) < 0;
    });

    // Deuxième passe : charger les images triées
    for(const QString& fileName : imageFiles) {
        if(m_zip.setCurrentFile(fileName) && file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QImage qImage;
            if(qImage.loadFromData(data)) {
                Page page;
                page.number = m_pages.size();
                page.image = new QImageAdapter(qImage); // Utilisation de l'adaptateur
                page.metadata.insert("source", fileName);
                m_pages.append(page);
            }
            file.close();
        }
    }
}

void CBZBook::addPage(const Page& page) {
    m_pages.append(page);
}

void CBZBook::removePage(int index) {
    if(index < 0 || index >= m_pages.size()) {
        throw std::out_of_range("Invalid page index");
    }
    m_pages.remove(index);
}

Page CBZBook::getPage(int index) const {
    if(index < 0 || index >= m_pages.size()) {
        throw std::out_of_range("Page index out of range");
    }
    return m_pages.at(index);
}

void CBZBook::save(const QString& path) {
    // Implémentation de la sauvegarde en format CBZ
    // Utiliser QuaZip pour recréer l'archive
}
int CBZBook::pageCount() const {
    return m_pages.size();
}

QVariantMap CBZBook::metadata() const {
    return m_metadata;
}

void CBZBook::setMetadata(const QVariantMap& meta) {
    m_metadata = meta;
}