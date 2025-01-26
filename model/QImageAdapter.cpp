#include "QImageAdapter.h"

QImageAdapter::QImageAdapter(const QImage& img) : m_image(img) {
    // Affiche les infos à la création
    std::cout << "\n=== Création QImageAdapter ===" << std::endl;
    debugPrint();
}

QImage QImageAdapter::toQImage() const {
    qDebug() << "Conversion vers QImage - Format:" << m_image.format();
    if(m_image.isNull()) qWarning() << "QImage est null!";
    return m_image; 
}

int QImageAdapter::width() const { 
    return m_image.width(); 
}

int QImageAdapter::height() const { 
    return m_image.height(); 
}

std::string QImageAdapter::format() const {
    switch(m_image.format()) {
        case QImage::Format_Invalid: return "Invalid";
        case QImage::Format_Mono: return "Mono";
        case QImage::Format_RGB32: return "RGB32";
        case QImage::Format_ARGB32: return "ARGB32";
        case QImage::Format_RGB16: return "RGB16";
        case QImage::Format_RGB888: return "RGB888";
        case QImage::Format_RGBA8888: return "RGBA8888";
        case QImage::Format_Grayscale8: return "Grayscale8";
        default: return QString("Format 0x%1").arg(m_image.format(), 0, 16).toStdString();
    }
}

const std::vector<uint8_t>& QImageAdapter::data() const { 
    static std::vector<uint8_t> vec;
    vec.assign(m_image.constBits(), m_image.constBits() + m_image.sizeInBytes());
    
    // Affiche les infos des données
    std::cout << "\n=== Données brutes ===" << std::endl;
    std::cout << "Taille: " << vec.size() << " octets" << std::endl;
    std::cout << "Début (hex): ";
    for(size_t i = 0; i < std::min<size_t>(10, vec.size()); ++i) {
        printf("%02X ", vec[i]);
    }
    std::cout << std::endl;
    
    return vec;
}

size_t QImageAdapter::dataSize() const { 
    return m_image.sizeInBytes(); 
}

void QImageAdapter::resize(int w, int h) { 
    std::cout << "\nRedimensionnement de " 
              << m_image.size().width() << "x" << m_image.size().height()
              << " vers " << w << "x" << h << std::endl;
    
    m_image = m_image.scaled(w, h);
    
    // Affiche les nouvelles infos
    debugPrint();
}

void QImageAdapter::saveToFile(const std::string& path) const { 
    bool success = m_image.save(QString::fromStdString(path));
    std::cout << "\nSauvegarde vers " << path 
              << " : " << (success ? "RÉUSSIE" : "ÉCHEC") << std::endl;
}

void QImageAdapter::debugPrint() const {
    std::cout << "Dimensions: " << width() << "x" << height() << std::endl;
    std::cout << "Format: " << format() << std::endl;
    std::cout << "Taille mémoire: " << dataSize() << " octets" << std::endl;
    std::cout << "Profondeur de couleur: " << m_image.depth() << " bits" << std::endl;
}