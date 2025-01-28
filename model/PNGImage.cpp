#include "PNGImage.h"
#include <QImage>
#include <QFile>
#include <QBuffer>
#include <QIODevice>

PNGImage::PNGImage(const std::vector<uint8_t>& data, int width, int height)
    : m_data(data), m_width(width), m_height(height), m_format(AbstractImage::PNG) {
}

int PNGImage::width() const { 
    return m_width; 
}

int PNGImage::height() const { 
    return m_height; 
}

AbstractImage::Format PNGImage::format() const { 
    return m_format; 
}

const std::vector<uint8_t>& PNGImage::data() const { 
    return m_data; 
}

size_t PNGImage::dataSize() const { 
    return m_data.size(); 
}

void PNGImage::resize(int newWidth, int newHeight) {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "PNG");
    if (img.isNull()) {
        throw std::runtime_error("Invalid PNG data");
    }
    QImage resized = img.scaled(newWidth, newHeight, Qt::KeepAspectRatio);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    resized.save(&buffer, "PNG");

    m_data = std::vector<uint8_t>(byteArray.begin(), byteArray.end());
    m_width = resized.width();
    m_height = resized.height();
}

void PNGImage::saveToFile(const std::string& path) const {
    QImage img = QImage::fromData(m_data.data(), m_data.size(), "PNG");
    if (!img.save(QString::fromStdString(path), "PNG")) {
        throw std::runtime_error("Failed to save PNG to file");
    }
}

int PNGImage::bytesPerLine() const {
    return m_width * 4; // RGBA8888 format, 4 bytes par pixel
}

QImage PNGImage::toQImage() const {
    return QImage(
        m_data.data(),
        m_width,
        m_height,
        bytesPerLine(),
        QImage::Format_RGBA8888
    );
}

void PNGImage::setData(const std::vector<unsigned char>& data,
                       int width, int height,
                       const std::string& format) {
    m_data = data;
    m_width = width;
    m_height = height;
    m_format = AbstractImage::PNG;
}
