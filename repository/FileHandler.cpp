#include "FileHandler.h"
#include "../model/PNGImage.h"
#include "../model/JPEGImage.h" 
#include "../model/AbstractImage.h"
#include "../infrastructure/ImageProcessor.h"
#include <fstream>

FileHandler::FileHandler(const std::string& path) : m_filePath(path) {}

AbstractImage* FileHandler::readFile() {
    std::ifstream file(m_filePath, std::ios::binary);
    if(!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier");
    }

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    
    // Détection du format et création de l'image appropriée
    // (implémentez la logique concrète ici)
    return new PNGImage(data); // Exemple
}

void FileHandler::writeFile(const AbstractImage* image, const std::string& path) {
    if(!image) return;
    
    std::ofstream file(path, std::ios::binary);
    if(!file.is_open()) {
        throw std::runtime_error("Échec de l'écriture");
    }
    
    const auto& imgData = image->data();
    file.write(reinterpret_cast<const char*>(imgData.data()), imgData.size());
}