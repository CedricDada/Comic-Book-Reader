#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "PageView.h"      // Vue des pages
#include "../repository/FileHandler.h"   // Accès aux fichiers
#include "../infrastructure/ImageProcessor.h"// Traitement d'images

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
    void on_actionSave_triggered();
    void on_actionZoom_in_triggered();
    void on_actionZoom_out_triggered();
    void on_actionZoom_100_triggered();

private:
    Ui::MainWindow *ui;
    PageView *m_pageView;    // Composant d'affichage
    FileHandler m_fileHandler{"chemin/par/default"}; // Initialisation directe
    AbstractImage* m_currentImage = nullptr; // Image courante
};

#endif // MAINWINDOW_H