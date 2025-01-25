#include "CBZBook.h"
#include <quazipfile.h>
#include <QFileInfo>

CBZBook::CBZBook(const QString& path) {
    m_filePath = path;
    m_zip.setZipName(path);
}

void CBZBook::loadPages() {
    if(!m_zip.open(QuaZip::mdUnzip)) {
        throw std::runtime_error("Failed to open CBZ file");
    }

    QuaZipFile file(&m_zip);
    for(bool more=m_zip.goToFirstFile(); more; more=m_zip.goToNextFile()) {
        if(file.open(QIODevice::ReadOnly)) {
            Page page;
            page.number = m_pages.size();
            page.image.loadFromData(file.readAll(), QFileInfo(file.getActualFileName()).suffix().toUtf8());
            m_pages.append(page);
            file.close();
        }
    }
    sortPagesByNaturalOrder();
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