#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include "../model/AbstractBook.h"  
#include "PageView.h"      // Vue des pages
#include "../repository/FileHandler.h"   // Accès aux fichiers
#include "../infrastructure/ImageProcessor.h"// Traitement d'images
#include "../model/BookManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots pour les actions du menu
    void on_actionOpen_files_triggered();
    void on_actionSave_in_file_triggered();
    void on_actionZoom_in_triggered();
    void on_actionZoom_out_triggered();
    void on_actionZoom_100_triggered();
    void goToPage(int pageNumber);
    void on_actionNext_page_triggered();
    void on_actionPrevious_page_triggered();

private:
    Ui::MainWindow *ui;
    QSlider* m_pageSlider;
    PageView *m_pageView;    // Composant d'affichage
    FileHandler m_fileHandler{"chemin/par/default"}; // Initialisation directe
    AbstractImage* m_currentImage = nullptr; // Image courante
    BookManager* m_bookManager = nullptr;
    CacheManager* m_cacheManager;
    std::shared_ptr<AbstractBook> m_currentBook;
    void preloadNextPages(int currentPage);
    void loadFirstPage(); 
    void startPreloading(); 
    void setupUI();


};

#endif // MAINWINDOW_H