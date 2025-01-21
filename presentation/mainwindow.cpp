#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_pageView(new PageView(this)), // Maintenant compatible
      m_fileHandler("") // Chemin vide initial
{
    ui->setupUi(this);
    
    // Configuration de la vue
    setCentralWidget(m_pageView);
    
    // Connexion des signaux/slots
    connect(ui->actionOpen_files, &QAction::triggered, 
            this, &MainWindow::on_actionOpen_files_triggered);
    // connect(ui->actionSave, &QAction::triggered, 
    //         this, &MainWindow::on_actionSave_triggered);
    // connect(ui->actionZoom_in, &QAction::triggered, 
    //         this, &MainWindow::on_actionZoom_in_triggered);
    // connect(ui->actionZoom_out, &QAction::triggered, 
    //         this, &MainWindow::on_actionZoom_out_triggered);
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_currentImage;
}

// Slot : Ouverture de fichier
void MainWindow::on_actionOpen_files_triggered() {
    QString path = QFileDialog::getOpenFileName(this, "Ouvrir une image", 
                  "", "Images (*.png *.jpg *.bmp)");
    
    if (!path.isEmpty()) {
        try {
            m_fileHandler = FileHandler(path.toStdString());
            m_currentImage = m_fileHandler.readFile();
            m_pageView->render(*m_currentImage);
        } 
        catch (const std::exception& e) {
            QMessageBox::critical(this, "Erreur", e.what());
        }
    }
}

// Slot : Sauvegarde
void MainWindow::on_actionSave_triggered() {
    QString path = QFileDialog::getSaveFileName(this, "Enregistrer", 
                  "", "Images (*.png *.jpg *.bmp)");
    
    if (!path.isEmpty() && m_currentImage) {
        FileHandler::writeFile(m_currentImage, path.toStdString());
    }
}

// Slot : Zoom avant
void MainWindow::on_actionZoom_in_triggered() {
    m_pageView->setZoom(m_pageView->zoomLevel() * 1.1);
}

// Slot : Zoom arrière
void MainWindow::on_actionZoom_out_triggered() {
    m_pageView->setZoom(m_pageView->zoomLevel() * 0.9);
}