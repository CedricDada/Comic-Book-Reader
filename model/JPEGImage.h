#ifndef JPEGIMAGE_H
#define JPEGIMAGE_H

#include "AbstractImage.h"
#include <vector>
#include <string>

class JPEGImage : public AbstractImage {
public:
    explicit JPEGImage(const std::vector<uint8_t>& data);
    JPEGImage(const std::vector<uint8_t>& data, int width, int height);

    // Implémentation des méthodes abstraites
    int width() const override;
    int height() const override;
    Format format() const override;
    QImage toQImage() const override;
    const std::vector<uint8_t>& data() const override;
    size_t dataSize() const override;
    void resize(int newWidth, int newHeight) override;
    void saveToFile(const std::string& path) const override;
    void setData(const std::vector<unsigned char>& data,
                 int width, int height,
                 const std::string& format) override;
    int bytesPerLine() const;

private:
    std::vector<uint8_t> m_data;
    int m_width;
    int m_height;
};

#endif // JPEGIMAGE_H
