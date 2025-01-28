#include "JPEGImage.h"
#include <QImage>

JPEGImage::JPEGImage(const std::vector<uint8_t>& data) : m_data(data) {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "JPEG");
    m_width = img.width();
    m_height = img.height();
}

int JPEGImage::width() const { return m_width; }
int JPEGImage::height() const { return m_height; }
std::string JPEGImage::format() const { return "JPEG"; }
const std::vector<uint8_t>& JPEGImage::data() const { return m_data; }
size_t JPEGImage::dataSize() const { return m_data.size(); }

void JPEGImage::resize(int newWidth, int newHeight) {
    QImage img(m_data.data(), m_width, m_height, QImage::Format_RGB888);
    QImage resized = img.scaled(newWidth, newHeight, Qt::KeepAspectRatio);
    m_data.assign(resized.bits(), resized.bits() + resized.sizeInBytes());
    m_width = newWidth;
    m_height = newHeight;
}

void JPEGImage::saveToFile(const std::string& path) const {
    QImage img(m_data.data(), m_width, m_height, QImage::Format_RGB888);
    img.save(QString::fromStdString(path), "JPEG", 85); // Qualité à 85%
}

QImage JPEGImage::toQImage() const {
    return QImage::fromData(m_data.data(), m_data.size(), "JPEG");
}

JPEGImage::JPEGImage(const JPEGImage& other) :
    m_data(other.m_data), // Copie profonde du vecteur
    m_width(other.m_width),
    m_height(other.m_height) {}


AbstractImage::Format JPEGImage::format_for_filter() const { return AbstractImage::JPEG; }

void JPEGImage::setData(const std::vector<unsigned char>& data,
                        int width, int height,
                        const std::string& /* format */) {
    this->m_data = data;
    this->m_width = width;
    this->m_height = height;
}