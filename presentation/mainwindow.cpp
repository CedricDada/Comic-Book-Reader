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
#include <QListWidget>
#include <QPushButton>

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
}

MainWindow::~MainWindow() {
    delete ui;
}

// Ajout de la configuration de la vue bibliothèque
void MainWindow::setupLibraryView() {
    m_libraryDock = new QDockWidget("Bibliothèques", this);
    m_libraryDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    
    QWidget* libraryContent = new QWidget(m_libraryDock);
    QVBoxLayout* layout = new QVBoxLayout(libraryContent);
    
    // Widgets pour la gestion des bibliothèques
    QListWidget* libraryList = new QListWidget(this);
    QPushButton* btnAddLibrary = new QPushButton("Ajouter une bibliothèque", this);
    
    // Style similaire aux autres panneaux
    m_libraryDock->setStyleSheet(R"(
        QDockWidget {
            background: rgb(230, 230, 230);
            border-radius: 4px;
            margin: 1px;
        }
        QListWidget {
            background: white;
            border: 1px solid #ddd;
        }
    )");
    
    layout->addWidget(libraryList);
    layout->addWidget(btnAddLibrary);
    libraryContent->setLayout(layout);
    m_libraryDock->setWidget(libraryContent);
    
    addDockWidget(Qt::LeftDockWidgetArea, m_libraryDock);
    m_libraryDock->hide(); // Caché par défaut
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
    ui->actionOpen_files->setIcon(QIcon(":/icons/open_file.png"));
    ui->actionZoom_in->setIcon(QIcon(":/icons/zoom_in.png"));
    ui->actionZoom_out->setIcon(QIcon(":/icons/zoom_out.png"));
    ui->actionZoom_100->setIcon(QIcon(":/icons/zoom_reset.png"));

    toolbar->addAction(ui->actionOpen_files);
    toolbar->addAction(ui->actionZoom_in);
    toolbar->addAction(ui->actionZoom_out);
    toolbar->addAction(ui->actionZoom_100);
        // Nouvel action pour les bibliothèques
    QAction* actionManageLib = new QAction(QIcon(":/icons/library.png"), "Bibliothèques", this);
        toolbar->addAction(ui->actionOpen_files);
    toolbar->addAction(actionManageLib); // Ajout du nouveau bouton

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
        btn->setIconSize(QSize(24, 24));
        btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn->setStyleSheet(R"(
            QToolButton {
                background: #ffffff;
                border: 2px solid #e0e0e0;
                border-radius: 8px;
                padding: 15px;
                margin: 5px;
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

    shortcutLayout->addWidget(createShortcutButton("Édition", ":resources/icons/comic-open.png"));
    shortcutLayout->addWidget(createShortcutButton("Filtres", ":resources/icons/comic-open.png"));
    shortcutLayout->addWidget(createShortcutButton("Export", ":resources/icons/comic-open.png"));
    shortcutLayout->addStretch();

    shortcutContent->setLayout(shortcutLayout);
    shortcutBar->setWidget(shortcutContent);
    addDockWidget(Qt::RightDockWidgetArea, shortcutBar);
}

void MainWindow::setupMetadataPanel() {
    QDockWidget* metadataPanel = new QDockWidget("Métadonnées", this);
    metadataPanel->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    QWidget* metadataContent = new QWidget(metadataPanel);
    QVBoxLayout* layout = new QVBoxLayout(metadataContent);

    QLabel* titleLabel = new QLabel("Titre : Inconnu", this);
    QLabel* authorLabel = new QLabel("Auteur : Inconnu", this);
    QLabel* pagesLabel = new QLabel("Pages : 0", this);

    layout->addWidget(titleLabel);
    layout->addWidget(authorLabel);
    layout->addWidget(pagesLabel);
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

        // Nouvelle connexion pour les bibliothèques
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
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this, 
        "Ouvrir des fichiers", 
        "", 
        "Images (*.png *.jpg *.jpeg *.bmp)"
    );
    
    if (!fileNames.isEmpty()) {
        // Ajoutez ici la logique de traitement des fichiers
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