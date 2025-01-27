#include "mainwindow.h"
#include "ui_mainwindow.h"/*ui_mainwindow.h : Généré automatiquement par Qt à partir du fichier .ui. Contient les déclarations 
des éléments de l'interface utilisateur.*/
#include <QFileDialog>
#include <iostream>
#include <memory>
/*Utilisé pour ouvrir/sauvegarder des fichiers. */
#include <QMessageBox>
#include <QToolBar>

/*Utilisé pour afficher des messages d'erreur ou d'information*/

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow), //Crée une instance de l'interface utilisateur.
      m_cacheManager(new CacheManager(1024)),
      m_pageView(new PageView(this)), // Crée une instance de PageView (vue pour afficher l'image).
      m_fileHandler("") // Chemin vide initial
{
    m_pageView->setCacheManager(m_cacheManager);
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
    connect(ui->actionSave_in_file, &QAction::triggered, 
             this, &MainWindow::on_actionSave_in_file_triggered);
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
    // connect(m_bookManager, &BookManager::bookReady, 
    // [this](AbstractBook* book) {
    //     m_currentBook.reset(book);
        
    //     // Debug 1: Vérification basique du livre
    //     qDebug() << "Livre reçu. Valide ?" << (m_currentBook ? "Oui" : "Non");
        
    //     if(m_currentBook) {
    //         // Debug 2: Informations générales du livre
    //         qDebug() << "Nombre de pages:" << m_currentBook->pageCount()
    //                  << "| Format:" << m_currentBook->metadata()["format"].toString() 
    //                  << "| Taille du cache:" << m_currentBook->pages().size();
    //         // Debug : vérifier les données reçues
    //         qDebug() << "Données brutes reçues :";
    //         for (const Page& page : book->pages()) {
    //             qDebug() << "Page" << page.number << " - Taille rawData :" << page.rawData.size();
    //         }

    //         if(m_currentBook->pageCount() > 0) {
    //             // Accéder au thread principal pour l'UI
    //             QMetaObject::invokeMethod(m_pageView, [this]() {
    //                 m_pageView->render(m_currentBook->pageAt(0));
    //             }, Qt::QueuedConnection); // Ne PAS utiliser BlockingQueuedConnection
    //         }
    //         else {
    //             qWarning() << "Livre valide mais sans pages !";
    //         }
    //     }
    // });

        connect(m_bookManager, &BookManager::pageProcessed, 
            [this](int index) {
                if(index == 0 && m_currentBook && !m_currentBook->pages().empty()) {
                    Page page = m_currentBook->pageAt(0);
                    m_pageView->render(page);
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

        m_pageView->setCurrentPage(0);
    }
}

void MainWindow::startPreloading() {
    auto currentBook = m_currentBook;
    if (currentBook) {
        QFuture<void> future = QtConcurrent::run([this, currentBook]() {
            for (int i = 0; i < currentBook->totalPages(); ++i) {
                Page page = currentBook->pageAt(i);
                if (!page.image) {
                    qWarning() << "Image non initialisée pour la page" << i;
                    continue;
                }
                qDebug() << "Préchargement page" << i;
                if(!page.image) {
                    qWarning() << "Image null pour page" << i;
                    continue;
                }
                ImageProcessor::processImage(page.image.get());
                m_cacheManager->storePage(page.number, page);
                QMetaObject::invokeMethod(m_pageView, "updateDisplay", 
                    Qt::QueuedConnection,
                    Q_ARG(Page, page));
            }
        });
        Q_UNUSED(future);
    }
}

// Slot : Sauvegarde
void MainWindow::on_actionSave_in_file_triggered() {
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

// Navigation vers une page spécifique
void MainWindow::goToPage(int pageNumber) {
    if(!m_currentBook || pageNumber < 0 || pageNumber >= m_currentBook->pageCount()) {
        qWarning() << "Invalid page number:" << pageNumber;
        return;
    }
    
    try {
        Page page = m_cacheManager->getPage(pageNumber);
        if (page.image == nullptr || page.rawData.isEmpty()) {
            throw std::runtime_error("Page invalide");
        }
        m_pageView->displayPage(page);
    }
    catch (const std::exception& e) {
        qWarning() << "Cache miss for page" << pageNumber << ":" << e.what();
        auto future = QtConcurrent::run([this, pageNumber](){
            Page page = m_currentBook->pageAt(pageNumber);
            m_cacheManager->storePage(pageNumber, page);
            QMetaObject::invokeMethod(m_pageView, [this, page](){
                m_pageView->displayPage(page);
            }, Qt::QueuedConnection);
        });
        Q_UNUSED(future);
    }
    
    //preloadNextPages(pageNumber);
}

void MainWindow::on_actionNext_page_triggered() {
    if (!m_currentBook) return;
    
    int current = m_pageView->currentPage();
    int nextPage = current + 1;
    
    if (nextPage < m_currentBook->pageCount()) {
        goToPage(nextPage);
    }
}

void MainWindow::on_actionPrevious_page_triggered() {
    int current = m_pageView->currentPage();
    int previousPage = current - 1;
    
    if (previousPage >= 0) {
        goToPage(previousPage);
    }
}
void MainWindow::preloadNextPages(int currentPage) {
    if(!m_currentBook) return;

    QFuture<void> future = QtConcurrent::run([this, currentPage]() {
        const int totalPages = m_currentBook->totalPages();
        
        for(int i = 1; i <= 3; ++i) {
            const int targetPage = currentPage + i;
            if(targetPage >= totalPages) break;

            try {
                // Vérification existence dans cache
                m_cacheManager->getPage(targetPage);
            }
            catch (const std::exception&) {
                // Chargement et stockage asynchrone
                Page page = m_currentBook->pageAt(targetPage);
                ImageProcessor::processRawData(page.rawData);
                m_cacheManager->storePage(targetPage, page);
            }
        }
    });
    Q_UNUSED(future);
}