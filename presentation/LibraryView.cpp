/**
 * Project Untitled
 */


#include "LibraryView.h"

/**
 * LibraryView implementation
 */


void LibraryView::displayLibrary() {

}

/**
 * @param querry
 */
void LibraryView::searchBooks(String querry) {

}

// LibraryView.cpp
void LibraryView::refreshTOC() {
    TocManager tocManager(ArchiveHandler::create(m_currentBookPath));
    tocManager.loadTOC();
    
    m_treeModel->clear();
    
    for(const auto& page : tocManager.getPages()) {
        QStandardItem* item = new QStandardItem(
            QString("%1 - %2").arg(page.number).arg(QString::fromStdString(page.title))
        );
        item->setData(QString::fromStdString(page.imagePath), Qt::UserRole);
        m_treeModel->appendRow(item);
    }
}