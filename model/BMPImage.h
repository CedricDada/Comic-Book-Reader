#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include "AbstractImage.h"
#include <vector>
#include <string>

class BMPImage : public AbstractImage {
public:
    explicit BMPImage(const std::vector<uint8_t>& data);
    
    int width() const override;
    int height() const override;
    std::string format() const override;
    const std::vector<uint8_t>& data() const override;
    size_t dataSize() const override;
    
    void resize(int newWidth, int newHeight) override;
    void saveToFile(const std::string& path) const override;

private:
    std::vector<uint8_t> m_data;
    int m_width;
    int m_height;
};

#endif // BMPIMAGE_H