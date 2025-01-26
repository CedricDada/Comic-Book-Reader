#ifndef ABSTRACTIMAGE_H
#define ABSTRACTIMAGE_H

#include <vector>
#include <string>
#include <QImage>

class AbstractImage {
public:
    virtual ~AbstractImage() = default;

    virtual QImage toQImage() const = 0;
    
    enum Format { PNG, JPEG, BMP, UNKNOWN };
    virtual Format format() const = 0;
    
    // Méthodes abstraites
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual const std::vector<uint8_t>& data() const = 0;
    virtual size_t dataSize() const = 0; 
    virtual void resize(int newWidth, int newHeight) = 0;
    virtual void saveToFile(const std::string& path) const = 0;
    virtual void setData(const std::vector<unsigned char>& data,
                        int width, int height,
                        const std::string& format) = 0;
    
    static std::string formatToString(Format format) {
        switch (format) {
            case PNG: return "PNG";
            case JPEG: return "JPEG";
            case BMP: return "BMP";
            default: return "UNKNOWN";
        }
    }
};

#endif // ABSTRACTIMAGE_H