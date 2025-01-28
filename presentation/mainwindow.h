#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QFileIconProvider>
#include "PageView.h"
#include "../repository/FileHandler.h"
#include "../infrastructure/ImageProcessor.h"
#include <QToolButton>
#include <QPropertyAnimation>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QDockWidget>
#include <QMainWindow>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QDockWidget>
#include <QTreeWidget>
#include "../infrastructure/Filter.h"
#include "../presentation/PageView.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void applyFilterToImage(QImage& image, ContentType contentType);
    ~MainWindow();

private slots:
    void on_actionOpen_files_triggered();
    void on_actionSave_triggered();
    void on_actionZoom_in_triggered();
    void on_actionZoom_out_triggered();
    void on_actionZoom_100_triggered();
    void addLibraryDirectory();
    void applyLowPassFilter();
    void applyTextEnhancementFilter();
    
private:
    Ui::MainWindow *ui;
    PageView *m_pageView;
    FileHandler m_fileHandler{"./"};
    AbstractImage* m_currentImage = nullptr;

    QDockWidget* m_libraryDock; 
    QTreeWidget* m_libraryTree;

    QLabel* m_fileNameLabel;
    QLabel* m_titleLabel;
    QLabel* m_authorLabel; 
    QLabel* m_pagesLabel;
    QLabel* m_zoomLabel;
    QLabel* m_fileInfoLabel;
    
    
    void applyLightTheme();
    void applyDarkTheme();
    QGraphicsDropShadowEffect* createHoverEffect();
    void setupInterface();
    void setupConnections();
    void setupToolbar(QToolBar* toolbar);
    void setupDock();
    void setupStatusBar();
    void setupMetadataPanel();
    void setupLibraryView();

    // Gestion de l'arborescence
    void populateTree(QTreeWidgetItem* parentItem, const QString& path);

    QPropertyAnimation* m_fadeAnimation;
    bool m_isDarkTheme = false;
    
    void toggleTheme();
    void updateZoomLabel();
    void animateZoom();
};

#endif // MAINWINDOW_H