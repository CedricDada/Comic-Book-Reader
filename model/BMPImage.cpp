#include "BMPImage.h"
#include <QImage>
#include <QBuffer>
#include <stdexcept>

BMPImage::BMPImage(const std::vector<uint8_t>& data, int width, int height)
    : m_data(data), m_width(width), m_height(height) {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "BMP");
    if (img.isNull()) {
        throw std::runtime_error("Invalid BMP data");
    }
    m_width = img.width();
    m_height = img.height();
}

int BMPImage::width() const {
    return m_width;
}

int BMPImage::height() const {
    return m_height;
}

AbstractImage::Format BMPImage::format() const {
    return AbstractImage::BMP;
}

const std::vector<uint8_t>& BMPImage::data() const {
    return m_data;
}

size_t BMPImage::dataSize() const {
    return m_data.size();
}

void BMPImage::resize(int newWidth, int newHeight) {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "BMP");
    if (img.isNull()) {
        throw std::runtime_error("Invalid BMP data for resizing");
    }
    QImage resized = img.scaled(newWidth, newHeight, Qt::KeepAspectRatio);
    
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    if (!resized.save(&buffer, "BMP")) {
        throw std::runtime_error("Failed to resize BMP image");
    }

    m_data = std::vector<uint8_t>(byteArray.begin(), byteArray.end());
    m_width = resized.width();
    m_height = resized.height();
}

void BMPImage::saveToFile(const std::string& path) const {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "BMP");
    if (img.isNull() || !img.save(QString::fromStdString(path), "BMP")) {
        throw std::runtime_error("Failed to save BMP to file");
    }
}

QImage BMPImage::toQImage() const {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "BMP");
    if (img.isNull()) {
        throw std::runtime_error("Invalid BMP data for conversion to QImage");
    }
    return img;
}

void BMPImage::setData(
    const std::vector<unsigned char>& data,
    int width,
    int height,
    const std::string& format
) {
    if (format != "BMP") {
        throw std::invalid_argument("Invalid format for BMPImage");
    }
    
    // Convert vector<unsigned char> to vector<uint8_t>
    m_data.assign(data.begin(), data.end());
    m_width = width;
    m_height = height;
}