cmake_minimum_required(VERSION 3.10)
project(ComicExtractor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Trouver les bibliothèques nécessaires
find_package(ZLIB REQUIRED)
find_package(LibArchive REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(POPPLER REQUIRED poppler-cpp)

# Utiliser pkg-config pour libzip
pkg_check_modules(LIBZIP REQUIRED libzip)

# Ajouter les fichiers sources
set(SOURCES
    main.cpp
)

# Ajouter l'exécutable
add_executable(ComicExtractor ${SOURCES})

# Lier les bibliothèques
target_link_libraries(ComicExtractor
    ZLIB::ZLIB
    LibArchive::LibArchive
    ${POPPLER_LIBRARIES}
    ${LIBZIP_LIBRARIES}
)

# Inclure les répertoires des bibliothèques
target_include_directories(ComicExtractor PRIVATE
    ${ZLIB_INCLUDE_DIRS}
    ${LibArchive_INCLUDE_DIRS}
    ${POPPLER_INCLUDE_DIRS}
    ${LIBZIP_INCLUDE_DIRS}
)