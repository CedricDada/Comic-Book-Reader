#ifndef PNGIMAGE_H
#define PNGIMAGE_H

#include "AbstractImage.h"
#include <vector>
#include <string>
#include <QImage>

class PNGImage : public AbstractImage {
public:
    explicit PNGImage(const std::vector<uint8_t>& data, int width, int height);

    int width() const override;
    int height() const override;
    Format format() const override;
    const std::vector<uint8_t>& data() const override;
    size_t dataSize() const override;
    void resize(int newWidth, int newHeight) override;
    void saveToFile(const std::string& path) const override;
    QImage toQImage() const override;
    void setData(const std::vector<unsigned char>& data,
                 int width, int height,
                 const std::string& format) override;

    int bytesPerLine() const;

private:
    Format m_format;
    std::vector<uint8_t> m_data;
    int m_width;
    int m_height;
};

#endif // PNGIMAGE_H
