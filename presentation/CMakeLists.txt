cmake_minimum_required(VERSION 3.19)
project(BookReaderView LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Configuration Qt6
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Gui Concurrent LinguistTools)
qt_standard_project_setup()

# 2. Configuration des dépendances
find_package(PkgConfig REQUIRED)
pkg_check_modules(POPPLER_QT6 REQUIRED poppler-qt6)
pkg_check_modules(LIBARCHIVE REQUIRED libarchive)
find_package(QuaZip-Qt6 REQUIRED)
find_package(ZLIB REQUIRED)

# 3. Liste des fichiers sources (conservé inchangé)
set(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    PageView.cpp
    PageView.h
    ../model/PNGImage.cpp
    ../model/PNGImage.h
    ../model/JPEGImage.cpp 
    ../model/JPEGImage.h 
    ../model/BMPImage.cpp 
    ../model/BMPImage.h 
    ../repository/FileHandler.cpp
    ../repository/FileHandler.h
    ../model/AbstractImage.h
    ../infrastructure/ImageProcessor.cpp
    ../infrastructure/ImageProcessor.h  
    ../model/Filter.h 
    ../model/Page.cpp                   
    ../model/Page.h 
    ../repository/ArchiveHandler.cpp    
    ../repository/ArchiveHandler.h 
    ../model/CacheManager.cpp  
    ../model/CacheManager.h 
    ../model/BookManager.cpp
    ../model/BookManager.h
    ../model/AbstractBook.cpp
    ../model/AbstractBook.h
    ../model/CBRBook.cpp
    ../model/CBZBook.cpp 
    ../model/PDFBook.cpp
    
    # LibraryView.cpp
    # LibraryView.h
    # SettingView.cpp
    # SettingView.h
    # UIManager.cpp
    # UIManager.h
)

# 4. Création de l'exécutable
qt_add_executable(BookReaderView
    WIN32 MACOSX_BUNDLE
    ${SOURCES}
)

# 5. Configuration des inclusions
target_include_directories(BookReaderView PRIVATE
    ${POPPLER_QT6_INCLUDE_DIRS}
    ${LIBARCHIVE_INCLUDE_DIRS}
    ${QUAZIP_QT6_INCLUDE_DIRS}
    /usr/local/include/QuaZip-Qt6-1.4  # Chemin explicite
    ${Qt6Core_INCLUDE_DIRS}
)

# 6. Lien des bibliothèques
target_link_libraries(BookReaderView PRIVATE
    Qt6::Core
    QuaZip::QuaZip
    Qt6::Widgets
    Qt6::Gui
    Qt6::Concurrent
    ${POPPLER_QT6_LIBRARIES}
    ${LIBARCHIVE_LIBRARIES}
    ${QUAZIP_QT6_LIBRARIES} 
    ZLIB::ZLIB
)