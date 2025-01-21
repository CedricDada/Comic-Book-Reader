#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "../model/AbstractImage.h"
#include <string>

class FileHandler {
public:
    explicit FileHandler(const std::string& path = ""); // Valeur par défaut
    
    AbstractImage* readFile();
    static void writeFile(const AbstractImage* image, const std::string& path);

private:
    std::string m_filePath;
};

#endif // FILEHANDLER_H