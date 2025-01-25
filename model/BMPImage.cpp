#include "BMPImage.h"
#include <QImage>

BMPImage::BMPImage(const std::vector<uint8_t>& data) : m_data(data) {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "BMP");
    m_width = img.width();
    m_height = img.height();
}

// Getters standards
int BMPImage::width() const { return m_width; }
int BMPImage::height() const { return m_height; }
std::string BMPImage::format() const { return "BMP"; }
const std::vector<uint8_t>& BMPImage::data() const { return m_data; }
size_t BMPImage::dataSize() const { return m_data.size(); }

// Redimensionnement générique
void BMPImage::resize(int newWidth, int newHeight) {
    QImage img(m_data.data(), m_width, m_height, QImage::Format_RGB32);
    QImage resized = img.scaled(newWidth, newHeight, Qt::KeepAspectRatio);
    m_data.assign(resized.bits(), resized.bits() + resized.sizeInBytes());
    m_width = newWidth;
    m_height = newHeight;
}

// Sauvegarde unifiée
void BMPImage::saveToFile(const std::string& path) const {
    QImage img(m_data.data(), m_width, m_height, QImage::Format_RGB32);
    if (!img.save(QString::fromStdString(path), "BMP")) {
        throw std::runtime_error("Échec de la sauvegarde BMP");
    }
}

QImage BMPImage::toQImage() const {
    return QImage::fromData(m_data.data(), m_data.size(), "BMP");
}