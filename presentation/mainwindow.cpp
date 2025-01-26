#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QVBoxLayout>
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

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_pageView(new PageView(this)),
      m_fileHandler(""),
      m_isDarkTheme(false) {
    ui->setupUi(this);
    setCentralWidget(m_pageView);

    setupInterface();
    setupConnections();
    applyLightTheme(); // Par défaut, thème clair

    // initialisation de la vue bibliothèque
    setupLibraryView();
    connect(m_pageView, &PageView::zoomChanged, this, [this](double level) {
        m_zoomLabel->setText(QString("Zoom: %1%").arg(qRound(level)));
    });
}


MainWindow::~MainWindow() {
    delete ui;
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

void MainWindow::on_actionOpen_files_triggered() {
    QString path = QFileDialog::getOpenFileName(this, "Ouvrir une image", 
                "", "Images (*.png *.jpg *.jpeg *.bmp *.webp)");
    
    if (!path.isEmpty()) {
        try {
            m_fileHandler = FileHandler(path.toStdString());
            m_currentImage = m_fileHandler.readFile();
            QImage image = m_currentImage->toQImage();

            // Appliquer le filtre avant de rendre l'image
            applyFilterToImage(image, ContentType::AutoDetect);

            //m_pageView->render(image);
            m_pageView->render(*m_currentImage); // Assuming m_currentImage is a AbstractImage
            // Met à jour les métadonnées
            QFileInfo fileInfo(path);
            QImageReader reader(path);
            
            // Met à jour le nom de fichier
            m_fileNameLabel->setText("Fichier : " + fileInfo.fileName());
            
            // Met à jour la barre de statut
            m_fileInfoLabel->setText(QString(" | Taille: %1 KB - Dimensions: %2x%3 - Modifié: %4")
                .arg(fileInfo.size() / 1024)
                .arg(reader.size().width())
                .arg(reader.size().height())
                .arg(fileInfo.lastModified().toString("dd/MM/yyyy hh:mm")));
            
            // Met à jour les autres informations
            m_titleLabel->setText("Titre : " + fileInfo.baseName());
            m_pagesLabel->setText("Pages : " + QString::number(m_pageView->pageCount()));

            // Essaye de récupérer les métadonnées EXIF
            if (reader.canRead()) {
                QString author = reader.text("Author");
                if (!author.isEmpty()) {
                    m_authorLabel->setText("Auteur : " + author);
                } else {
                    m_authorLabel->setText("Auteur : Inconnu");
            }
            } else {
                m_authorLabel->setText("Auteur : Inconnu");
            }
        } 
        catch (const std::exception& e) {
            QMessageBox::critical(this, "Erreur", e.what());
        }
    }
}

void MainWindow::on_actionSave_triggered() {
    // Implémentez la logique de sauvegarde
}

void MainWindow::on_actionZoom_in_triggered() {
    m_pageView->zoomIn();
    updateZoomLabel();
}

void MainWindow::on_actionZoom_out_triggered() {
    m_pageView->zoomOut();
    updateZoomLabel();
}

void MainWindow::on_actionZoom_100_triggered() {
    m_pageView->resetZoom();
    updateZoomLabel();
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
        m_pageView->render(*m_currentImage);
    }
}

void MainWindow::applyTextEnhancementFilter() {
    if (m_currentImage) {
        ImageProcessor::applyFilter(m_currentImage, ContentType::TextDominant, 1.0f);
        m_pageView->render(*m_currentImage); // Pass AbstractImage reference
    }
}