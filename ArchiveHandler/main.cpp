#include <iostream>
#include <filesystem>
#include <string>
#include <cstring>
#include <fstream>
#include <zip.h>         // Pour les fichiers CBZ
#include <archive.h>     // Pour les fichiers CBR, CBT, CB7
#include <archive_entry.h>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-image.h>
#include <poppler/cpp/poppler-page-renderer.h>


namespace fs = std::filesystem;

// Fonction pour extraire un fichier CBZ
void extractCBZ(const std::string& cbzPath, const std::string& outputPath) {
    int error;
    zip_t* zipArchive = zip_open(cbzPath.c_str(), ZIP_RDONLY, &error);

    if (!zipArchive) {
        char errorBuffer[128];
        zip_error_to_str(errorBuffer, sizeof(errorBuffer), error, errno);
        std::cerr << "Erreur : Impossible d'ouvrir l'archive CBZ (" << cbzPath << ") : " << errorBuffer << std::endl;
        return;
    }

    std::cout << "Ouverture réussie de l'archive CBZ : " << cbzPath << std::endl;

    if (!fs::exists(outputPath)) {
        fs::create_directories(outputPath);
        std::cout << "Dossier de sortie créé : " << outputPath << std::endl;
    }

    zip_int64_t numEntries = zip_get_num_entries(zipArchive, 0);

    for (zip_int64_t i = 0; i < numEntries; i++) {
        const char* fileName = zip_get_name(zipArchive, static_cast<zip_uint64_t>(i), 0);
        if (!fileName) {
            std::cerr << "Erreur : Impossible de récupérer le nom du fichier à l'index " << i << std::endl;
            continue;
        }

        std::string outputFilePath = (fs::path(outputPath) / fileName).string();

        zip_stat_t fileStat;
        if (zip_stat(zipArchive, fileName, 0, &fileStat) == 0 && fileStat.size == 0 && fileName[strlen(fileName) - 1] == '/') {
            fs::create_directories(outputFilePath);
            std::cout << "Dossier créé : " << outputFilePath << std::endl;
        } else {
            zip_file_t* zipFile = zip_fopen_index(zipArchive, static_cast<zip_uint64_t>(i), 0);
            if (!zipFile) {
                std::cerr << "Erreur : Impossible d'ouvrir le fichier dans l'archive : " << fileName << std::endl;
                continue;
            }

            std::ofstream outputFile(outputFilePath, std::ios::binary);
            if (!outputFile.is_open()) {
                std::cerr << "Erreur : Impossible de créer le fichier de sortie : " << outputFilePath << std::endl;
                zip_fclose(zipFile);
                continue;
            }

            char buffer[8192];
            zip_int64_t bytesRead;
            while ((bytesRead = zip_fread(zipFile, buffer, sizeof(buffer))) > 0) {
                outputFile.write(buffer, bytesRead);
            }

            if (bytesRead < 0) {
                std::cerr << "Erreur : Impossible de lire le fichier dans l'archive : " << fileName << std::endl;
                zip_fclose(zipFile);
                outputFile.close();
                fs::remove(outputFilePath);
                continue;
            }

            zip_fclose(zipFile);
            outputFile.close();

            std::cout << "Fichier extrait : " << outputFilePath << std::endl;
        }
    }

    zip_close(zipArchive);
    std::cout << "Extraction terminée pour l'archive CBZ : " << cbzPath << std::endl;
}

// Fonction pour extraire un fichier CBR, CBT ou CB7
void extractArchive(const std::string& archivePath, const std::string& outputPath) {
    struct archive* archive = archive_read_new();
    archive_read_support_format_all(archive);
    archive_read_support_filter_all(archive);

    if (archive_read_open_filename(archive, archivePath.c_str(), 10240) != ARCHIVE_OK) {
        std::cerr << "Erreur : Impossible d'ouvrir l'archive (" << archivePath << ")" << std::endl;
        return;
    }

    std::cout << "Ouverture réussie de l'archive : " << archivePath << std::endl;

    if (!fs::exists(outputPath)) {
        fs::create_directories(outputPath);
        std::cout << "Dossier de sortie créé : " << outputPath << std::endl;
    }

    struct archive_entry* entry;
    while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
        std::string fileName = archive_entry_pathname(entry);
        std::string outputFilePath = (fs::path(outputPath) / fileName).string();

        if (archive_entry_filetype(entry) == AE_IFDIR) {
            fs::create_directories(outputFilePath);
            std::cout << "Dossier créé : " << outputFilePath << std::endl;
        } else {
            std::ofstream outputFile(outputFilePath, std::ios::binary);
            if (!outputFile.is_open()) {
                std::cerr << "Erreur : Impossible de créer le fichier de sortie : " << outputFilePath << std::endl;
                continue;
            }

            const void* buffer;
            size_t size;
            int64_t offset;
            while (archive_read_data_block(archive, &buffer, &size, &offset) == ARCHIVE_OK) {
                outputFile.write(static_cast<const char*>(buffer), size);
            }

            outputFile.close();
            std::cout << "Fichier extrait : " << outputFilePath << std::endl;
        }
    }

    archive_read_close(archive);
    archive_read_free(archive);
    std::cout << "Extraction terminée pour l'archive : " << archivePath << std::endl;
}

// Fonction pour extraire un fichier PDF
void extractPDF(const std::string& pdfPath, const std::string& outputPath) {
    // Charger le document PDF
    poppler::document* doc = poppler::document::load_from_file(pdfPath);
    if (!doc) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier PDF (" << pdfPath << ")" << std::endl;
        return;
    }

    std::cout << "Ouverture réussie du fichier PDF : " << pdfPath << std::endl;

    // Crée le dossier de sortie s'il n'existe pas
    if (!fs::exists(outputPath)) {
        fs::create_directories(outputPath);
        std::cout << "Dossier de sortie créé : " << outputPath << std::endl;
    }

    // Crée un renderer pour dessiner les pages
    poppler::page_renderer renderer;
    renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
    renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);

    // Parcours toutes les pages du PDF
    for (int i = 0; i < doc->pages(); i++) {
        poppler::page* page = doc->create_page(i);
        if (!page) {
            std::cerr << "Erreur : Impossible de créer la page " << i + 1 << std::endl;
            continue;
        }

        // Rend la page en image avec une résolution de 144 DPI
        poppler::image image = renderer.render_page(page, 144.0, 144.0);
        if (image.is_valid()) {
            std::string outputFilePath = (fs::path(outputPath) / ("page_" + std::to_string(i + 1) + ".png")).string();
            if (image.save(outputFilePath, "png")) {
                std::cout << "Page extraite : " << outputFilePath << std::endl;
            } else {
                std::cerr << "Erreur : Impossible d'enregistrer la page " << i + 1 << std::endl;
            }
        } else {
            std::cerr << "Erreur : Impossible de rendre la page " << i + 1 << std::endl;
        }

        // Libère la mémoire de la page
        delete page;
    }

    // Libère la mémoire du document
    delete doc;
    std::cout << "Extraction terminée pour le fichier PDF : " << pdfPath << std::endl;
}
// Fonction principale pour extraire un comic
void extractComic(const std::string& comicPath, const std::string& outputPath) {
    fs::path path(comicPath);
    std::string extension = path.extension().string();

    if (extension == ".cbz") {
        extractCBZ(comicPath, outputPath);
    } else if (extension == ".cbr" || extension == ".cbt" || extension == ".cb7") {
        extractArchive(comicPath, outputPath);
    } else if (extension == ".pdf") {
        extractPDF(comicPath, outputPath);
    } else {
        std::cerr << "Erreur : Format de fichier non supporté (" << extension << ")" << std::endl;
    }
}

int main() {
    // Chemin du fichier comic (CBZ, CBR, CBT, CB7, PDF)
    std::string comicPath = "../Art.cbr"; //test

    // Chemin du dossier de sortie
    std::string outputPath = "./output";

    // Extrait le comic
    extractComic(comicPath, outputPath);

    return 0;
}