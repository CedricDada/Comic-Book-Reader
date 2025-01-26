#include "JPEGImage.h"
#include <QImage>
#include <QBuffer>
#include <QIODevice>

JPEGImage::JPEGImage(const std::vector<uint8_t>& data)
    : m_data(data), m_width(0), m_height(0) {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "JPEG");
    m_width = img.width();
    m_height = img.height();
}

JPEGImage::JPEGImage(const std::vector<uint8_t>& data, int width, int height)
    : m_data(data), m_width(width), m_height(height) {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "JPEG");
    m_width = img.width();
    m_height = img.height();
}

int JPEGImage::width() const { return m_width; }
int JPEGImage::height() const { return m_height; }
AbstractImage::Format JPEGImage::format() const { return AbstractImage::JPEG; }
const std::vector<uint8_t>& JPEGImage::data() const { return m_data; }
size_t JPEGImage::dataSize() const { return m_data.size(); }

void JPEGImage::resize(int newWidth, int newHeight) {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "JPEG");
    QImage resized = img.scaled(newWidth, newHeight, Qt::KeepAspectRatio);
    QByteArray buffer;
    QBuffer qBuffer(&buffer);
    qBuffer.open(QIODevice::WriteOnly);
    resized.save(&qBuffer, "JPEG");

    m_data.assign(buffer.begin(), buffer.end());
    m_width = resized.width();
    m_height = resized.height();
}

void JPEGImage::saveToFile(const std::string& path) const {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "JPEG");
    img.save(QString::fromStdString(path), "JPEG", 85); // Qualité à 85%
}

int JPEGImage::bytesPerLine() const {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "JPEG");
    return img.bytesPerLine();
}

QImage JPEGImage::toQImage() const {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "JPEG");
    return img;
}

void JPEGImage::setData(const std::vector<unsigned char>& data,
                        int width, int height,
                        const std::string& /* format */) {
    this->m_data = data;
    this->m_width = width;
    this->m_height = height;
}
