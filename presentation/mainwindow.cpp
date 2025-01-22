#include "mainwindow.h"
#include "ui_mainwindow.h"/*ui_mainwindow.h : Généré automatiquement par Qt à partir du fichier .ui. Contient les déclarations 
des éléments de l'interface utilisateur.*/
#include <QFileDialog>
/*Utilisé pour ouvrir/sauvegarder des fichiers. */
#include <QMessageBox>
/*Utilisé pour afficher des messages d'erreur ou d'information*/

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow), //Crée une instance de l'interface utilisateur.
      m_pageView(new PageView(this)), // Crée une instance de PageView (vue pour afficher l'image).
      m_fileHandler("") // Chemin vide initial
{
    ui->setupUi(this);
    
    // Configuration de la vue
    setCentralWidget(m_pageView);//Définit m_pageView comme widget central de la fenêtre.
    
    // Connexion des signaux/slots
    /*
    ui->actionOpen_files : L'action "Ouvrir un fichier" dans l'interface.

    &QAction::triggered : Le signal émis lorsque l'action est déclenchée.

    this : L'objet qui contient le slot (ici, MainWindow).

    &MainWindow::on_actionOpen_files_triggered : La méthode à appeler (slot).
    */
    connect(ui->actionOpen_files, &QAction::triggered, 
            this, &MainWindow::on_actionOpen_files_triggered);
    // connect(ui->actionSave, &QAction::triggered, 
    //         this, &MainWindow::on_actionSave_triggered);
    // connect(ui->actionZoom_in, &QAction::triggered, 
    //         this, &MainWindow::on_actionZoom_in_triggered);
    // connect(ui->actionZoom_out, &QAction::triggered, 
    //         this, &MainWindow::on_actionZoom_out_triggered);
    /*
    Important : Automatisation avec on_<objectName>_<signal>
    Qt permet de connecter automatiquement les signaux et slots si la méthode suit la nomenclature on_<objectName>_<signal>.

    Par exemple, on_actionOpen_files_triggered() est automatiquement connecté à actionOpen_files->triggered().
    */
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