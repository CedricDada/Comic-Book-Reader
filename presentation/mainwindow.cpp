#include "mainwindow.h"
#include "ui_mainwindow.h"/*ui_mainwindow.h : Généré automatiquement par Qt à partir du fichier .ui. Contient les déclarations 
des éléments de l'interface utilisateur.*/
#include <QFileDialog>
#include <iostream>
#include <memory>
/*Utilisé pour ouvrir/sauvegarder des fichiers. */
#include <QMessageBox>
#include <QToolBar>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QMenuBar>
#include <QDockWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QToolButton>
#include <QFileInfo>
#include <QTreeWidget>
#include <QPushButton>
#include <QDir>
#include <QFileIconProvider>
#include <QImageReader>
#include <qimage.h>
#include "../infrastructure/Filter.h"



/*Utilisé pour afficher des messages d'erreur ou d'information*/

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow), //Crée une instance de l'interface utilisateur.
      m_cacheManager(new CacheManager(1024 * 100)),
      m_pageView(new PageView(this)), // Crée une instance de PageView (vue pour afficher l'image).
      m_fileHandler(""), // Chemin vide initial
      m_isDarkTheme(false) 
{
    m_pageView->setCacheManager(m_cacheManager);
    ui->setupUi(this);
    
    // Configuration de la vue
    setCentralWidget(m_pageView);//Définit m_pageView comme widget central de la fenêtre.

    setupInterface();
    setupConnections();
    applyLightTheme(); // Par défaut, thème clair

    // initialisation de la vue bibliothèque
    setupLibraryView();
    connect(m_pageView, &PageView::zoomChanged, this, [this](double level) {
        m_zoomLabel->setText(QString("Zoom: %1%").arg(qRound(level)));
    });
    
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
            for (int i = 0; i < 3; ++i) {
                 try {
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
                catch (const std::exception& e) {
                    qWarning() << "Cache miss for page" << ":" << e.what();
                }
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
    
    preloadNextPages(pageNumber);
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
        
        for(int i = 0; i <= 3; ++i) {
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

void MainWindow::applyLightTheme() {
    // Styles pour le thème clair
    setStyleSheet(R"(
        QMainWindow {
            background-color: #f5f5f5;
        }
        QMenuBar {
            background-color: #ffffff;
        }
    )");
}

void MainWindow::applyDarkTheme() {
    // Styles pour le thème sombre
    setStyleSheet(R"(
        QMainWindow {
            background-color: #2d2d2d;
        }
        QMenuBar {
            background-color: #333333;
        }
    )");
}

void MainWindow::toggleTheme() {
    m_isDarkTheme = !m_isDarkTheme;
    m_isDarkTheme ? applyDarkTheme() : applyLightTheme();
}

void MainWindow::updateZoomLabel() {
    m_zoomLabel->setText(QString("Zoom: %1%").arg(m_pageView->getZoomLevel()));
}

void MainWindow::animateZoom() {
    QPropertyAnimation* animation = new QPropertyAnimation(m_pageView, "zoomLevel");
    animation->setDuration(300);
    animation->setStartValue(m_pageView->getZoomLevel());
    animation->setEndValue(100);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

// After: Work through AbstractImage interface
void MainWindow::applyLowPassFilter() {
    if (m_currentImage) {
        // Let ImageProcessor handle conversions internally
        ImageProcessor::applyFilter(m_currentImage, ContentType::GraphicDominant, 0.7f);
        Page render_page(0, m_currentImage);
        m_pageView->render(render_page);
    }
}

void MainWindow::applyTextEnhancementFilter() {
    if (m_currentImage) {
        ImageProcessor::applyFilter(m_currentImage, ContentType::TextDominant, 1.0f);
        Page render_page(0, m_currentImage);
        m_pageView->render(render_page);
    }
}

void MainWindow::applyFilterToImage(QImage &image, ContentType contentType) {
    auto filter = AbstractFilter::createLowPassFilter(0.5f); // Exemple de filtre passe-bas
    filter->apply(image, contentType);
}

// Ajout de la configuration de la vue bibliothèque
void MainWindow::setupLibraryView() {
    m_libraryDock = new QDockWidget("Bibliothèques", this);
    m_libraryDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    
    QWidget* libraryContent = new QWidget(m_libraryDock);
    QVBoxLayout* layout = new QVBoxLayout(libraryContent);
    
    m_libraryTree = new QTreeWidget(this);
    m_libraryTree->setHeaderHidden(true);
    
    QPushButton* btnAddLibrary = new QPushButton("Add a library", this);
    
    // Style
    m_libraryDock->setStyleSheet(R"(
        QDockWidget {
            background: rgb(230, 230, 230);
            border-radius: 4px;
            margin: 1px;
        }
        QTreeWidget {
            background: white;
            border: 1px solid #ddd;
        }
    )");
    
    layout->addWidget(m_libraryTree);
    layout->addWidget(btnAddLibrary);
    libraryContent->setLayout(layout);
    m_libraryDock->setWidget(libraryContent);
    
    addDockWidget(Qt::LeftDockWidgetArea, m_libraryDock);
    m_libraryDock->hide();
    
    // Connexion directe pour simplifier
    connect(btnAddLibrary, &QPushButton::clicked, this, &MainWindow::addLibraryDirectory);
}

void MainWindow::addLibraryDirectory() {
    QString directory = QFileDialog::getExistingDirectory(
        this,
        "Sélectionner un répertoire de bibliothèque",
        QDir::homePath(),
        QFileDialog::ShowDirsOnly
    );
    
    if (!directory.isEmpty()) {
        QTreeWidgetItem* rootItem = new QTreeWidgetItem(m_libraryTree);
        rootItem->setText(0, QDir(directory).dirName());
        rootItem->setData(0, Qt::UserRole, directory);
        rootItem->setIcon(0, QFileIconProvider().icon(QFileIconProvider::Folder));
        
        populateTree(rootItem, directory);
        m_libraryTree->expandItem(rootItem);
    }
}

void MainWindow::populateTree(QTreeWidgetItem* parentItem, const QString& path) {
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    
    foreach (const QFileInfo& entry, entries) {
        QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
        item->setText(0, entry.fileName());
        item->setData(0, Qt::UserRole, entry.filePath());
        
        if (entry.isDir()) {
            item->setIcon(0, QFileIconProvider().icon(QFileIconProvider::Folder));
            populateTree(item, entry.filePath()); // Recursion pour les sous-répertoires
        }
        else {
            item->setIcon(0, QFileIconProvider().icon(entry));
        }
    }
}

void MainWindow::setupInterface() {
    // Ombre portée pour la fenêtre principale
    auto* windowShadow = new QGraphicsDropShadowEffect(this);
    windowShadow->setBlurRadius(20);
    windowShadow->setColor(QColor(5, 5, 5, 30));
    windowShadow->setOffset(2, 2);
    this->setGraphicsEffect(windowShadow);

    // Barre de menu
    QMenuBar* menuBar = this->menuBar();
    menuBar->setStyleSheet("QMenuBar::item { transition: all 0.3s ease; }");

    // Barre d'outils principale
    QToolBar* toolbar = this->addToolBar("Main Toolbar");
    toolbar->setMovable(true);
    setupToolbar(toolbar);

    // Barre déplaçable (dock)
    setupDock();

    // Barre de statut
    setupStatusBar();

    // Panneau des métadonnées
    setupMetadataPanel();
}

void MainWindow::setupToolbar(QToolBar* toolbar) {
    ui->actionOpen_files->setIcon(QIcon(":/icons/comic_open.png"));
    ui->actionZoom_in->setIcon(QIcon(":/icons/comic_open.png"));
    ui->actionZoom_out->setIcon(QIcon(":/icons/comic_open.png"));
    ui->actionZoom_100->setIcon(QIcon(":/icons/comic_open.png"));

    toolbar->addAction(ui->actionOpen_files);
    toolbar->addAction(ui->actionZoom_in);
    toolbar->addAction(ui->actionZoom_out);
    toolbar->addAction(ui->actionZoom_100);
        // Nouvel action pour les bibliothèques
    QAction* actionManageLib = new QAction(QIcon(":/icons/library.png"), "Bibliothèques", this);
        toolbar->addAction(ui->actionOpen_files);
    toolbar->addAction(actionManageLib); // Ajout du nouveau bouton

    QAction* actionLowPassFilter = new QAction(QIcon(":/icons/lowpass.png"), "Low Pass Filter", this);
    QAction* actionTextEnhancementFilter = new QAction(QIcon(":/icons/textenhance.png"), "Text Enhancement Filter", this);

    toolbar->addAction(actionLowPassFilter);
    toolbar->addAction(actionTextEnhancementFilter);

    connect(actionLowPassFilter, &QAction::triggered, this, &MainWindow::applyLowPassFilter);
    connect(actionTextEnhancementFilter, &QAction::triggered, this, &MainWindow::applyTextEnhancementFilter);
    
    for (QAction* action : toolbar->actions()) {
        QToolButton* btn = qobject_cast<QToolButton*>(toolbar->widgetForAction(action));
        if (btn) {
            btn->setGraphicsEffect(createHoverEffect());
            btn->setStyleSheet(R"(
                QToolButton {
                    border: 2px solid #e0e0e0;
                    transition: all 0.3s ease;
                }
                QToolButton:hover {
                    border: 2px solid #2196F3;
                    transform: scale(1.1);
                }
            )");
        }
    }
}

void MainWindow::setupDock() {
    QDockWidget* shortcutBar = new QDockWidget("Outils rapides", this);
    shortcutBar->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    QWidget* shortcutContent = new QWidget(shortcutBar);
    QVBoxLayout* shortcutLayout = new QVBoxLayout(shortcutContent);

    shortcutBar->setStyleSheet(R"(
        QDockWidget {
            background:rgb(199, 203, 204);
            border-radius: 4px;
            margin: 1px;
        }
    )");

    auto createShortcutButton = [](const QString& text, const QString& iconPath) {
        QToolButton* btn = new QToolButton();
        btn->setText(text);
        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(14, 14));
        btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn->setStyleSheet(R"(
            QToolButton {
                background: #ffffff;
                border: 2px solid #e0e0e0;
                border-radius: 8px;
                color: #333;
                transition: all 0.3s ease;
            }
            QToolButton:hover {
                background: #2196F3;
                color: white;
                border-color: #1976D2;
            }
        )");
        return btn;
    };

    shortcutLayout->addWidget(createShortcutButton("Édition", ":/icons/comic-open.png"));
    shortcutLayout->addWidget(createShortcutButton("Filtres", ":/icons/comic-open.png"));
    shortcutLayout->addStretch();

    shortcutContent->setLayout(shortcutLayout);
    shortcutBar->setWidget(shortcutContent);
    addDockWidget(Qt::RightDockWidgetArea, shortcutBar);
}

void MainWindow::setupMetadataPanel() {
    QDockWidget* metadataPanel = new QDockWidget("Métadonnées", this);
    metadataPanel->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    metadataPanel->setStyleSheet(R"(
        QDockWidget {
            background: rgb(230, 230, 230);
            border-radius: 4px;
            margin: 1px;
        }
        QLabel {
            margin: 5px;
            padding: 2px;
            color: #555;
        }
        QLabel[objectName^="metadata_"] {
            font-size: 12px;
            border-bottom: 1px solid #eee;
        }
    )");

    QWidget* metadataContent = new QWidget(metadataPanel);
    QVBoxLayout* layout = new QVBoxLayout(metadataContent);

    m_fileNameLabel = new QLabel("Fichier : Aucun", this);
    m_fileNameLabel->setObjectName("metadata_title");
    m_fileNameLabel->setStyleSheet("font-weight: bold; color: #2196F3; font-size: 14px;");

    m_titleLabel = new QLabel("Titre : Inconnu", this);
    m_authorLabel = new QLabel("Auteur : Inconnu", this);
    m_pagesLabel = new QLabel("Pages : 0", this);

    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet("color: #ddd;");

    layout->addWidget(m_fileNameLabel);
    layout->addWidget(separator);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_authorLabel);
    layout->addWidget(m_pagesLabel);
    layout->addStretch();

    metadataContent->setLayout(layout);
    metadataPanel->setWidget(metadataContent);
    addDockWidget(Qt::LeftDockWidgetArea, metadataPanel);
}


void MainWindow::setupStatusBar() {
    statusBar()->setStyleSheet("background: #ffffff; color: #333; border-top: 5px solid #e0e0e0;");
    m_zoomLabel = new QLabel("Zoom: 100%", this);
    m_fileInfoLabel = new QLabel(" | Fichier: Aucun", this);
    statusBar()->addPermanentWidget(m_zoomLabel);
    statusBar()->addPermanentWidget(m_fileInfoLabel);
}

// Nouvelles implémentations ajoutées
void MainWindow::setupConnections() {
    connect(ui->actionOpen_files, &QAction::triggered, this, &MainWindow::on_actionOpen_files_triggered);
    connect(ui->actionZoom_in, &QAction::triggered, this, &MainWindow::on_actionZoom_in_triggered);
    connect(ui->actionZoom_out, &QAction::triggered, this, &MainWindow::on_actionZoom_out_triggered);
    connect(ui->actionZoom_100, &QAction::triggered, this, &MainWindow::on_actionZoom_100_triggered);

    QList<QAction*> actions = findChildren<QAction*>();
    foreach(QAction* action, actions) {
        if(action->text() == "Bibliothèques") {
            connect(action, &QAction::triggered, [this]() {
                m_libraryDock->setVisible(!m_libraryDock->isVisible());
            });
        }
    }
}

QGraphicsDropShadowEffect* MainWindow::createHoverEffect() {
    QGraphicsDropShadowEffect* hoverEffect = new QGraphicsDropShadowEffect(this);
    hoverEffect->setBlurRadius(15);
    hoverEffect->setColor(QColor(0, 0, 0, 30));
    hoverEffect->setOffset(3, 3);
    return hoverEffect;
}