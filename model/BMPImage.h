#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include "AbstractImage.h"
#include <vector>
#include <string>

class BMPImage : public AbstractImage {
public:
    explicit BMPImage(const std::vector<uint8_t>& data);
    
    // Méthodes communes standardisées
    int width() const override;
    int height() const override;
    std::string format() const override;
    const std::vector<uint8_t>& data() const override;
    size_t dataSize() const override;
    
    // Méthodes spécifiques
    void resize(int newWidth, int newHeight) override;
    void saveToFile(const std::string& path) const override;

    QImage toQImage() const override;
    AbstractImage* clone() const override {
        return new BMPImage(*this);
    }
    BMPImage(const BMPImage& other);
    void setData(const std::vector<unsigned char>& data,
                 int width, int height,
                 const std::string& format) override;

    // Méthode supplémentaire
    int bytesPerLine() const;
    Format format_for_filter() const override;
private:
    std::vector<uint8_t> m_data;
    int m_width;
    int m_height;
};

#endif // BMPIMAGE_H