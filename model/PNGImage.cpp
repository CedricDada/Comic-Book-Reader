#include "PNGImage.h"
#include <QImage>

PNGImage::PNGImage(const std::vector<uint8_t>& data) : m_data(data) {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "PNG");
    m_width = img.width();
    m_height = img.height();
}

// Implémentez TOUTES les méthodes déclarées
int PNGImage::width() const { return m_width; }
int PNGImage::height() const { return m_height; }
std::string PNGImage::format() const { return "PNG"; }
const std::vector<uint8_t>& PNGImage::data() const { return m_data; }
size_t PNGImage::dataSize() const { return m_data.size(); }

void PNGImage::resize(int newWidth, int newHeight) {
    // Logique de redimensionnement (ex: avec QImage)
    QImage img(m_data.data(), m_width, m_height, QImage::Format_ARGB32);
    QImage resized = img.scaled(newWidth, newHeight, Qt::KeepAspectRatio);
    // Mettre à jour m_data, m_width et m_height...
}

void PNGImage::saveToFile(const std::string& path) const {
    QImage img(m_data.data(), m_width, m_height, QImage::Format_ARGB32);
    img.save(QString::fromStdString(path), "PNG");
}
QImage PNGImage::toQImage() const {
    QImage img;
    img.loadFromData(m_data.data(), m_data.size(), "PNG");
    return img;
}