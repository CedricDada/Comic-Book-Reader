#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "PageView.h"
#include "../repository/FileHandler.h"
#include "../infrastructure/ImageProcessor.h"
#include <QToolButton>
#include <QPropertyAnimation>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

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
    

private:
    Ui::MainWindow *ui;
    PageView *m_pageView;
    FileHandler m_fileHandler{"chemin/par/default"};
    AbstractImage* m_currentImage = nullptr;
        void applyLightTheme();
    void applyDarkTheme();
    QGraphicsDropShadowEffect* createHoverEffect();

    // Variables membres
    QLabel* m_zoomLabel;
    QLabel* m_fileInfoLabel;
    QPropertyAnimation* m_fadeAnimation;
    bool m_isDarkTheme = false;
    void toggleTheme();
    void updateZoomLabel();
    void animateZoom();
    void setupInterface();      // Déclaration
    void setupConnections();    // Déclaration
    void setupToolbar(QToolBar* toolbar);  // Déclaration
    void setupDock();           // Déclaration
    void setupStatusBar();      // Déclaration
    void setupMetadataPanel();
};

#endif // MAINWINDOW_H