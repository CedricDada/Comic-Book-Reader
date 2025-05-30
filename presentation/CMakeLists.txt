cmake_minimum_required(VERSION 3.19)
project(BookReaderView LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

# Liste tous les fichiers sources
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
    ../model/Filter.h 
    
    # LibraryView.cpp
    # LibraryView.h
    # SettingView.cpp
    # SettingView.h
    # UIManager.cpp
    # UIManager.h
)

qt_add_executable(BookReaderView
    WIN32 MACOSX_BUNDLE
    ${SOURCES}
)

qt_add_translations(BookReaderView
    TS_FILES BookReaderView_en_US.ts
)

target_link_libraries(BookReaderView
    PRIVATE
        Qt6::Core
        Qt6::Widgets
)

# Configurer l'installation
include(GNUInstallDirs)

install(TARGETS BookReaderView
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Générer le script de déploiement
qt_generate_deploy_app_script(
    TARGET BookReaderView
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
