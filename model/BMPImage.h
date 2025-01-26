#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include "AbstractImage.h"
#include <vector>
#include <string>

class BMPImage : public AbstractImage {
public:
    explicit BMPImage(const std::vector<uint8_t>& data, int width, int height);

    // Implémentation des méthodes abstraites
    int width() const override;
    int height() const override;
    Format format() const override;
    const std::vector<uint8_t>& data() const override;
    size_t dataSize() const override;

    // Méthodes spécifiques
    void resize(int newWidth, int newHeight) override;
    void saveToFile(const std::string& path) const override;
    QImage toQImage() const override;
    void setData(const std::vector<unsigned char>& data,
                 int width, int height,
                 const std::string& format) override;

private:
    std::vector<uint8_t> m_data;
    int m_width = 0;
    int m_height = 0;
};

#endif // BMPIMAGE_H
