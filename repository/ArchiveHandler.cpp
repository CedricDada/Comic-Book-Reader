#include "ArchiveHandler.h"
#include <fstream>
#include <sstream>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-image.h>
#include <poppler/cpp/poppler-document.h>
#include <tinyxml2.h>

using namespace std;

// Méthodes de base ArchiveHandler
unique_ptr<ArchiveHandler> ArchiveHandler::create(const string& filePath) {
    fs::path path(filePath);
    string ext = path.extension().string();
    
    if(ext == ".cbz") return make_unique<CBZHandler>(filePath);
    if(ext == ".cbr" || ext == ".cbt" || ext == ".cb7") return make_unique<CBRHandler>(filePath);
    if(ext == ".pdf") return make_unique<PDFHandler>(filePath);
    
    throw runtime_error("Format non supporté: " + ext);
}

void ArchiveHandler::ensureOutputDirectory(const fs::path& path) {
    if(!fs::exists(path)) {
        fs::create_directories(path);
        cout << "Dossier créé: " << path << endl;
    }
}

void ArchiveHandler::handleLibzipError(int errorCode) {
    char buffer[128];
    zip_error_to_str(buffer, sizeof(buffer), errorCode, errno);
    cerr << "Erreur libzip: " << buffer << endl;
}

void ArchiveHandler::handleLibarchiveError(struct archive* arch) {
    cerr << "Erreur libarchive: " << archive_error_string(arch) << endl;
}

// Implémentation CBZHandler
CBZHandler::CBZHandler(const string& path) : comicPath(path) {
    int error;
    zipArchive = zip_open(path.c_str(), ZIP_RDONLY, &error);
    if(!zipArchive) handleLibzipError(error);
}

void CBZHandler::extract(const string& outputPath) {
    fs::path outPath(outputPath);
    ensureOutputDirectory(outPath);
    
    zip_int64_t numEntries = zip_get_num_entries(zipArchive, 0);
    
    for(zip_int64_t i = 0; i < numEntries; ++i) {
        const char* fileName = zip_get_name(zipArchive, i, 0);
        fs::path filePath = outPath / fileName;
        
        if(zip_stat_t fileStat; zip_stat(zipArchive, fileName, 0, &fileStat) == 0) {
            if(fileStat.size == 0 && fileName[strlen(fileName)-1] == '/') {
                fs::create_directories(filePath);
                continue;
            }
        }
        
        zip_file_t* file = zip_fopen_index(zipArchive, i, 0);
        if(!file) {
            handleLibzipError(zip_get_error(zipArchive)->zip_err);
            continue;
        }
        
        ofstream outFile(filePath, ios::binary);
        char buffer[8192];
        zip_int64_t bytesRead;
        
        while((bytesRead = zip_fread(file, buffer, sizeof(buffer))) > 0) {
            outFile.write(buffer, bytesRead);
        }
        
        zip_fclose(file);
        cout << "Fichier extrait: " << filePath << endl;
    }
}

string CBZHandler::readTableOfContents() {
    zip_int64_t index = zip_name_locate(zipArchive, "toc.xml", 0);
    if(index < 0) return "";
    
    zip_file_t* file = zip_fopen_index(zipArchive, index, 0);
    stringstream ss;
    char buffer[4096];
    zip_int64_t bytesRead;
    
    while((bytesRead = zip_fread(file, buffer, sizeof(buffer))) > 0) {
        ss.write(buffer, bytesRead);
    }
    
    zip_fclose(file);
    return ss.str();
}

void CBZHandler::writeTableOfContents(const string& tocContent) {
    zip_source_t* src = zip_source_buffer(zipArchive, tocContent.c_str(), tocContent.size(), 0);
    zip_add(zipArchive, "toc.xml", src);
}
// CBZHandler.cpp
std::string CBZHandler::readTableOfContents() {
    zip_int64_t index = zip_name_locate(m_archive, "toc.xml", 0);
    if(index < 0) return "";
    
    zip_file_t* file = zip_fopen_index(m_archive, index, 0);
    std::string content;
    char buffer[4096];
    zip_int64_t bytesRead;
    
    while((bytesRead = zip_fread(file, buffer, sizeof(buffer))) > 0) {
        content.append(buffer, bytesRead);
    }
    
    zip_fclose(file);
    return content;
}

void CBZHandler::writeTableOfContents(const std::string& tocContent) {
    zip_source_t* src = zip_source_buffer(m_archive, tocContent.c_str(), tocContent.size(), 0);
    zip_int64_t index = zip_name_locate(m_archive, "toc.xml", 0);
    
    if(index >= 0) {
        zip_replace(m_archive, index, src);
    } else {
        zip_add(m_archive, "toc.xml", src);
    }
}
// Implémentations similaires pour CBRHandler et PDFHandler...

// Implémentation PDFHandler
PDFHandler::PDFHandler(const std::string& path) : m_path(path) {
    m_doc = poppler::document::load_from_file(path);
    if(!m_doc) throw std::runtime_error("Failed to open PDF document");
}

PDFHandler::~PDFHandler() {
    delete m_doc;
}

std::string PDFHandler::readTableOfContents() {
    // Lire le TOC depuis les métadonnées PDF
    std::string tocContent;
    
    // Méthode 1 : Outline du PDF
    if(poppler::outline* outline = m_doc->create_outline()) {
        for(const auto& item : *outline) {
            parsePdfOutline(item, 0, tocContent);
        }
        delete outline;
    }
    // Méthode 2 : Fichier toc.xml externe (si existant)
    std::string tocPath = fs::path(m_path).parent_path().string() + "/toc.xml";
    if(fs::exists(tocPath)) {
        std::ifstream file(tocPath);
        tocContent.assign((std::istreambuf_iterator<char>(file)), 
                         std::istreambuf_iterator<char>());
    }
    
    return tocContent;
}

void PDFHandler::parsePdfOutline(const poppler::outline_item* item, int level, std::string& toc) {
    if(!item) return;
    
    toc += std::string(level*2, ' ') + "- " + item->title().to_utf8() + "\n";
    
    for(const auto& child : item->children()) {
        parsePdfOutline(child, level + 1, toc);
    }
}

void PDFHandler::writeTableOfContents(const std::string& tocContent) {
    // Écrire dans un fichier externe
    std::string tocPath = fs::path(m_path).parent_path().string() + "/toc.xml";
    std::ofstream file(tocPath);
    file << tocContent;
}

void PDFHandler::extract(const std::string& outputPath) {
    poppler::page_renderer renderer;
    renderer.set_render_hints(
        poppler::page_renderer::antialiasing | 
        poppler::page_renderer::text_antialiasing
    );
    
    fs::create_directories(outputPath);
    
    for(int i = 0; i < m_doc->pages(); ++i) {
        poppler::page* page = m_doc->create_page(i);
        poppler::image img = renderer.render_page(page, 144.0, 144.0);
        
        std::string filename = outputPath + "/page_" + std::to_string(i+1) + ".png";
        img.save(filename, "png");
        
        delete page;
    }
}


void PDFHandler::compress(const std::vector<std::string>& files) {
    throw std::runtime_error("PDF compression not supported");
}