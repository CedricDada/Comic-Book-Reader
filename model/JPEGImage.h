#ifndef JPEGIMAGE_H
#define JPEGIMAGE_H

#include "AbstractImage.h"
#include <vector>
#include <string>

class JPEGImage : public AbstractImage {
public:
    explicit JPEGImage(const std::vector<uint8_t>& data);
    
    // Implémentation des méthodes abstraites
    int width() const override;
    int height() const override;
    std::string format() const override;
    const std::vector<uint8_t>& data() const override;
    size_t dataSize() const override;
    void resize(int newWidth, int newHeight) override;
    void saveToFile(const std::string& path) const override;

    QImage toQImage() const override;

private:
    std::vector<uint8_t> m_data;
    int m_width;
    int m_height;
};

#endif // JPEGIMAGE_H