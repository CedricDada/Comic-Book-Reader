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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_files_triggered();
    void on_actionSave_triggered();
    void on_actionZoom_in_triggered();
    void on_actionZoom_out_triggered();
    void on_actionZoom_100_triggered();
    void addLibraryDirectory(); // Nouveau slot pour l'ajout de bibliothèque

    
private:
    Ui::MainWindow *ui;
    PageView *m_pageView;
    FileHandler m_fileHandler{"chemin/par/default"};
    AbstractImage* m_currentImage = nullptr;

    QDockWidget* m_libraryDock; // Nouveau membre pour la vue bibliothèque
    QTreeWidget* m_libraryTree;
    
    // Méthodes d'initialisation
    void applyLightTheme();
    void applyDarkTheme();
    QGraphicsDropShadowEffect* createHoverEffect();
    void setupInterface();
    void setupConnections();
    void setupToolbar(QToolBar* toolbar);
    void setupDock();
    void setupStatusBar();
    void setupMetadataPanel();
    void setupLibraryView(); // Nouvelle méthode pour la bibliothèque

    // Gestion de l'arborescence
    void populateTree(QTreeWidgetItem* parentItem, const QString& path);

    // Variables membres
    QLabel* m_zoomLabel;
    QLabel* m_fileInfoLabel;
    QPropertyAnimation* m_fadeAnimation;
    bool m_isDarkTheme = false;
    
    // Méthodes utilitaires
    void toggleTheme();
    void updateZoomLabel();
    void animateZoom();
};

#endif // MAINWINDOW_H