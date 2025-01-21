#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "../model/AbstractImage.h"
#include <string>

class FileHandler {
public:
    explicit FileHandler(const std::string& path);
    
    AbstractImage* readFile();
    static void writeFile(const AbstractImage* image, const std::string& path);

private:
    std::string m_filePath;
    
    // Détection de format basée sur l'entête
    static std::string detectFormat(const std::vector<uint8_t>& data);
};

#endif // FILEHANDLER_H