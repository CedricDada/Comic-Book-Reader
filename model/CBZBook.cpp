#include "CBZBook.h"
#include "QImageAdapter.h"
#include <quazipfile.h>
#include <QFileInfo>


#include "CBZBook.h"
#include "QImageAdapter.h"
#include <quazipfile.h>
#include <QFileInfo>
#include <algorithm>
#include <iostream>

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
                        // Afficher les données brutes
            std::cout << "=== Données brutes pour " << fileName.toStdString() << " ===" << std::endl;
            std::cout << "Taille des données : " << data.size() << " octets" << std::endl;

            // Afficher les premiers octets (en hexadécimal)
            const int previewSize = 64; // Nombre d'octets à afficher
            std::cout << "Début des données (hex) : ";
            for (int i = 0; i < qMin(previewSize, data.size()); ++i) {
                printf("%02X ", static_cast<unsigned char>(data[i]));
            }
            std::cout << std::endl;
            //charger l'image
            QImage qImage;
            if(qImage.loadFromData(data)) {
                Page page;
                page.number = m_pages.size();
                page.rawData = data;
                page.image = std::shared_ptr<AbstractImage>(new QImageAdapter(qImage));
                page.metadata.insert("source", fileName);
                m_pages.append(page);
            }
            file.close();
        }
    }
}
void CBZBook::loadRawPages() {
    m_zip.setZipName(m_filePath);
    
    if (!m_zip.open(QuaZip::mdUnzip)) {
        throw std::runtime_error("Échec d'ouverture du CBZ");
    }

    QuaZipFile file(&m_zip);
    QuaZipFileInfo info;

    // Collecter les fichiers image
    QStringList imageFiles;
    for (bool more = m_zip.goToFirstFile(); more; more = m_zip.goToNextFile()) {
        if (m_zip.getCurrentFileInfo(&info) && info.name.endsWith(".jpg", Qt::CaseInsensitive)) {
            imageFiles.append(info.name);
        }
    }

    // Trier les fichiers
    std::sort(imageFiles.begin(), imageFiles.end(), [](const QString& a, const QString& b) {
        return QString::compare(a, b, Qt::CaseInsensitive) < 0;
    });

    // Charger les données brutes
    m_rawPagesData.clear();
    for (const QString& fileName : imageFiles) {
        if (m_zip.setCurrentFile(fileName) && file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            qDebug() << "Taille données :" << data.size();
            m_rawPagesData.append(data); 
            file.close();
        }
    }

    // Créer les pages avec les données brutes
    m_pages.clear();
    for (int i = 0; i < m_rawPagesData.size(); ++i) {
        Page page;
        page.number = i;
        page.rawData = m_rawPagesData[i];
        
        QImage qImg;
        if(qImg.loadFromData(page.rawData)) {
            QImage clonedImg = qImg.copy(); 
            page.image = std::make_shared<QImageAdapter>(clonedImg);
        }
        
        page.metadata["source"] = imageFiles[i];
        m_pages.append(page);
    }
    
    m_zip.close();
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