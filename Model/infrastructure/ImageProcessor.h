/**
 * Project Untitled
 */


#ifndef _IMAGEPROCESSOR_H
#define _IMAGEPROCESSOR_H

class ImageProcessor {
public: 
    void supportedFormats;
    
/**
 * @param data
 */
void decodeImage(bytes data);
    
/**
 * @param image
 * @param width
 * @param height
 */
void resizeImage(Image image, int width, int height);
    
/**
 * @param image
 * @param filter
 */
void applyFilter(Image image, Filter filter);
};

#endif //_IMAGEPROCESSOR_H