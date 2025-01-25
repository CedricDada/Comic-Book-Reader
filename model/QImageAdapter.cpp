// QImageAdapter.cpp
#include "QImageAdapter.h"

QImageAdapter::QImageAdapter(const QImage& img) : m_image(img) {}

QImage QImageAdapter::toQImage() const { return m_image; }
int QImageAdapter::width() const { return m_image.width(); }
int QImageAdapter::height() const { return m_image.height(); }
std::string QImageAdapter::format() const { return "QImage"; }
const std::vector<uint8_t>& QImageAdapter::data() const { 
    static std::vector<uint8_t> vec(m_image.constBits(), m_image.constBits() + m_image.sizeInBytes());
    return vec;
}
size_t QImageAdapter::dataSize() const { return m_image.sizeInBytes(); }
void QImageAdapter::resize(int w, int h) { m_image = m_image.scaled(w, h); }
void QImageAdapter::saveToFile(const std::string& path) const { m_image.save(QString::fromStdString(path)); }