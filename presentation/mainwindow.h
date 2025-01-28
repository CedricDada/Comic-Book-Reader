#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <QDockWidget>
#include <QTreeWidget>
#include "../model/AbstractBook.h"  
#include "PageView.h"      // Vue des pages
#include "../repository/FileHandler.h"   // Accès aux fichiers
#include "../infrastructure/ImageProcessor.h"// Traitement d'images
#include "../model/BookManager.h"

#include <QFileIconProvider>
#include <QToolButton>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include "../infrastructure/Filter.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void applyFilterToImage(QImage& image, ContentType contentType);

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
    void addLibraryDirectory(); 
    void applyLowPassFilter();
    void applyTextEnhancementFilter();

private:
    Ui::MainWindow *ui;
    QSlider* m_pageSlider;
    PageView *m_pageView;    
    FileHandler m_fileHandler{"./"}; 
    AbstractImage* m_currentImage = nullptr; 
    BookManager* m_bookManager = nullptr;
    CacheManager* m_cacheManager;
    // std::shared_ptr<AbstractBook> m_currentBook;
    QSharedPointer<AbstractBook> m_currentBook;
    void preloadNextPages(int currentPage);
    void loadFirstPage(); 
    void startPreloading(); 
    void setupInterface();
    void setupConnections();
    void applyLightTheme();
    void setupLibraryView();
    void applyDarkTheme();
    void toggleTheme();
    void updateZoomLabel();
    void animateZoom();
    QLabel* m_fileNameLabel;
    QLabel* m_titleLabel;
    QLabel* m_authorLabel; 
    QLabel* m_pagesLabel;
    QLabel* m_zoomLabel;
    QLabel* m_fileInfoLabel;

    QDockWidget* m_libraryDock; 
    QTreeWidget* m_libraryTree;
    
    QGraphicsDropShadowEffect* createHoverEffect();
    void setupToolbar(QToolBar* toolbar);
    void setupDock();
    void setupStatusBar();
    void setupMetadataPanel();

    // Gestion de l'arborescence
    void populateTree(QTreeWidgetItem* parentItem, const QString& path);

    QPropertyAnimation* m_fadeAnimation;
    bool m_isDarkTheme = false;
};


#endif // MAINWINDOW_H