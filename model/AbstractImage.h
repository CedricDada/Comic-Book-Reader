#ifndef ABSTRACTIMAGE_H
#define ABSTRACTIMAGE_H

#include <vector>
#include <string>
#include <QImage>

class AbstractImage {
public:
    virtual ~AbstractImage() = default;

    virtual QImage toQImage() const = 0;
    
    // Méthodes abstraites
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual std::string format() const = 0;
    virtual const std::vector<uint8_t>& data() const = 0;
    virtual size_t dataSize() const = 0; 
    virtual void resize(int newWidth, int newHeight) = 0;
    virtual void saveToFile(const std::string& path) const = 0;
    virtual AbstractImage* clone() const = 0;
};

#endif // ABSTRACTIMAGE_H