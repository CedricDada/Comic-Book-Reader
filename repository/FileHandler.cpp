#include "FileHandler.h"
#include "../model/PNGImage.h"
#include "../model/JPEGImage.h"
#include "../model/BMPImage.h"
#include <fstream>
#include <cstring> // Pour memcmp

FileHandler::FileHandler(const std::string& path) : m_filePath(path) {}

AbstractImage* FileHandler::readFile() {
    std::ifstream file(m_filePath, std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("Erreur d'ouverture");
    
    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    
    std::string format = detectFormat(data);
    
    if (format == "PNG") return new PNGImage(data);
    if (format == "JPEG") return new JPEGImage(data);
    if (format == "BMP") return new BMPImage(data);
    
    throw std::runtime_error("Format non supporté");
}

std::string FileHandler::detectFormat(const std::vector<uint8_t>& data) {
    // Détection par signature binaire
    if (data.size() > 8 && !memcmp(data.data(), "\x89PNG\r\n\x1a\n", 8)) return "PNG";
    if (data.size() > 2 && data[0] == 0xFF && data[1] == 0xD8) return "JPEG";
    if (data.size() > 2 && !memcmp(data.data(), "BM", 2)) return "BMP";
    
    return "UNKNOWN";
}

void FileHandler::writeFile(const AbstractImage* image, const std::string& path) {
    if (!image) return;
    image->saveToFile(path); // Polymorphisme
}