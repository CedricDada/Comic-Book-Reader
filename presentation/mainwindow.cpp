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
    QStringList paths = QFileDialog::getOpenFileNames(
        this, 
        "Ouvrir des fichiers",
        "",
        "Tous les formats supportés (*.png *.jpg *.bmp *.pdf *.cbr *.cbz);;"
        "Images (*.png *.jpg *.bmp);;"
        "Archives BD (*.pdf *.cbr *.cbz)"
    );

    if(!paths.isEmpty()) {
        m_bookManager = new BookManager(this);
        m_cacheManager = new CacheManager(1024); // 1GB cache

        // Connexions des signaux
        connect(m_bookManager, &BookManager::bookReady, 
            [this](AbstractBook* book) {
                /*
                Connexion signal-slot

                    Quand un livre est prêt (bookReady) :

                    Stocke le livre

                    Affiche la première page

                    Démarre le préchargement
                */
                m_currentBook.reset(book);
                loadFirstPage();
                startPreloading();
            });

        connect(m_bookManager, &BookManager::pageProcessed, 
            [this](int index) {
                if(index == 0 && m_currentBook && !m_currentBook->pages().empty()) {
                    Page page = m_currentBook->pageAt(0);
                    m_pageView->updateDisplay(page);
                }
            });

        // Démarre le traitement
        QThread::create([this, paths](){
                /*
                 Traitement asynchrone

                Crée un thread séparé pour :

                Ouvrir chaque fichier via BookManager

                Évite de bloquer l'UI
                 */
            for(const auto& path : paths) {
                this->m_bookManager->openBook(path);
            }
        })->start();
    }
}

// Méthodes auxiliaires
void MainWindow::loadFirstPage() {
    if(m_currentBook && !m_currentBook->pages().empty()) {
        m_pageView->displayPageAsync(m_currentBook->pages().first());
    }
}

void MainWindow::startPreloading() {
    if(m_currentBook) {
        QFuture<void> future = QtConcurrent::run([this](){
            for(int i = 1; i < m_currentBook->totalPages(); ++i) {
                Page page = m_currentBook->pageAt(i);
                ImageProcessor::processImage(page.image);
                m_cacheManager->storePage(page.number, page);
                QMetaObject::invokeMethod(m_pageView, "updateDisplay", 
                    Qt::QueuedConnection,
                    Q_ARG(QImage, page.image->toQImage()));
            }
        });
        Q_UNUSED(future); // Supprime l'avertissement
    }
}

// Slot : Sauvegarde
void MainWindow::on_actionSave_triggered() {
    QString path = QFileDialog::getSaveFileName(this, "Enregistrer", 
                  "", "Images (*.png *.jpg *.bmp *.pdf *cbz)");
    
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