#include "mainwindow.h"
#include "ui_mainwindow.h"/*ui_mainwindow.h : Généré automatiquement par Qt à partir du fichier .ui. Contient les déclarations 
des éléments de l'interface utilisateur.*/
#include <QFileDialog>
/*Utilisé pour ouvrir/sauvegarder des fichiers. */
#include <QMessageBox>
/*Utilisé pour afficher des messages d'erreur ou d'information*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),

      ui(new Ui::MainWindow),
      m_pageView(new PageView(this)),
      m_fileHandler("")
{
    ui->setupUi(this);
    setCentralWidget(m_pageView);

    // ===================================================================
    // CONFIGURATION DES ICÔNES DANS LA MENUBAR
    // ===================================================================
    
    // Définir la taille des icônes (32x32 recommandé)
    const QSize iconSize(48, 48);
    
    // Assigner les icônes aux actions
    // Ajouter l'icône à l'action Open_files
    ui->actionOpen_files->setIcon(QIcon(":/icons/comic-open.png"));
    
    // Optionnel : forcer l'affichage si nécessaire
    ui->actionOpen_files->setIconVisibleInMenu(true);

    // Ajouter l'icône au menu Fichier
    ui->menuFile->setIcon(QIcon(":resources/icons/comic-open.png"));
    
    // ui->actionSave->setIcon(QIcon(":/icons/comic-save.png"));
    // ui->actionSave->setIconVisibleInMenu(true);
    
    // ui->actionZoom_in->setIcon(QIcon(":/icons/comic-zoom-in.png"));
    // ui->actionZoom_in->setIconVisibleInMenu(true);
    
    // ui->actionZoom_out->setIcon(QIcon(":/icons/comic-zoom-out.png"));
    // ui->actionZoom_out->setIconVisibleInMenu(true);
    
    // ui->actionZoom_100->setIcon(QIcon(":/icons/comic-zoom-reset.png"));
    // ui->actionZoom_100->setIconVisibleInMenu(true);

    // Style personnalisé pour la menubar
    QMenuBar* menuBar = this->menuBar();
    menuBar->setStyleSheet(R"(
        QMenuBar {
            background-color:rgb(110, 64, 189);
            color: #FFFFFF;
            padding: 2px;
            border-bottom: 1px solid #404040;
        }
        QMenuBar::item#menuFileAction {
            qproperty-icon: url(:/resources/icons/comic-open.png);
            padding: 5px 10px;
        }
        QMenuBar::item:selected {
            background: #404040;
        }
        QMenuBar::item {
            padding: 5px 10px;
            background: transparent;
        }
        QMenuBar::item:selected {
            background: #404040;
        }
        QMenu {
            background-color: #333333;
            color: #FFFFFF;
            border: 1px solid #444444;
        }
        QMenu::item {
            padding: 5px 25px 5px 20px;
        }
        QMenu::item:selected {
            background-color: #404040;
        }
        QMenu::icon {
            padding-left: 5px;
        }
    )");

// ===================================================================
// FIN DES MODIFICATIONS
// ===================================================================

    // Connexions existantes...
    connect(ui->actionOpen_files, &QAction::triggered, 
            this, &MainWindow::on_actionOpen_files_triggered);
}

MainWindow::~MainWindow() {
    delete ui; //Libère la mémoire allouée pour l'interface utilisateur.
    delete m_currentImage;//Libère la mémoire allouée pour l'image actuellement chargée.
}

// Slot : Ouverture de fichier
void MainWindow::on_actionOpen_files_triggered() {
    QString path = QFileDialog::getOpenFileName(this, "Ouvrir une image", 
                  "", "Images (*.png *.jpg *.bmp)");//Ouvre une boîte de dialogue pour sélectionner un fichier
    
    if (!path.isEmpty()) {
        try {
            m_fileHandler = FileHandler(path.toStdString()); //Initialise FileHandler avec le chemin du fichier.
            m_currentImage = m_fileHandler.readFile(); //Lit le fichier et crée une instance de l'image.
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

// Slot : Zoom arrière
void MainWindow::on_actionZoom_100_triggered() {
    m_pageView->setZoom(m_pageView->zoomLevel());
}