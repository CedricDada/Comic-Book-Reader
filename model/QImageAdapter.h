#ifndef QIMAGEADAPTER_H
#define QIMAGEADAPTER_H

#include "AbstractImage.h"
#include <QImage>
#include <QDebug> // Ajout pour le débogage
#include <iostream> 
#include <QThread> 

class QImageAdapter : public AbstractImage {
    QImage m_image;
    mutable std::vector<uint8_t> m_data;
public:
    AbstractImage* clone() const { 
        qDebug() << "Clonage de QImageAdapter - Thread:" << QThread::currentThreadId();
        return new QImageAdapter(m_image.copy()); 
    }
    explicit QImageAdapter(const QImage& img);
    Format format_for_filter() const override;
    void setData(const std::vector<unsigned char>& data, int width, int height, const std::string& format) override;
    
    QImage toQImage() const override;
    int width() const override;
    int height() const override;
    std::string format() const override;
    const std::vector<uint8_t>& data() const override;
    size_t dataSize() const override;
    void resize(int newWidth, int newHeight) override;
    void saveToFile(const std::string& path) const override;

private:
    void debugPrint() const; // Nouvelle méthode de débogage
};

#endif // QIMAGEADAPTER_H