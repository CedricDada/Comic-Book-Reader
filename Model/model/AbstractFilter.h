/**
 * Project Untitled
 */


#ifndef _ABSTRACTFILTER_H
#define _ABSTRACTFILTER_H

class AbstractFilter {
public: 
    void parameters;
    
/**
 * @param image
 */
void apply(Image image);
    
/**
 * @param key
 * @param value
 */
void setParameter(String key, Object value);
    
/**
 * @param key
 */
void getParameter(String key);
};

#endif //_ABSTRACTFILTER_H