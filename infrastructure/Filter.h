#ifndef FILTER_H
#define FILTER_H

#include <QImage>
#include <memory>

enum class ContentType {
    AutoDetect,
    TextDominant,    // Filtrage optimisé pour le texte
    GraphicDominant, // Filtrage optimisé pour les images
    MixedContent     // Compromis texte/images
};

class AbstractFilter {
public:
    virtual ~AbstractFilter() = default;
    
    // Applique le filtre en fonction du type de contenu
    virtual void apply(QImage& image, ContentType contentType) const = 0;
    
    // Crée une copie polymorphique
    virtual std::unique_ptr<AbstractFilter> clone() const = 0;
    
    // Méthode statique pour créer des filtres prédéfinis
    static std::unique_ptr<AbstractFilter> createLowPassFilter(float cutoffFrequency);
    static std::unique_ptr<AbstractFilter> createTextOptimizedFilter();
};

// Filtre passe-bas paramétrable
class LowPassFilter : public AbstractFilter {
public:
    explicit LowPassFilter(float cutoffFrequency = 0.5f, int kernelSize = 5);
    LowPassFilter(const LowPassFilter&) = default; // Constructeur de copie
    
    void apply(QImage& image, ContentType contentType) const override;
    std::unique_ptr<AbstractFilter> clone() const override;

private:
    float m_cutoffFrequency; 
    int m_kernelSize;        
    void applyToText(QImage& image) const;
    void applyToGraphics(QImage& image) const;
    void applyAdaptive(QImage& image) const;
};

class TextEnhancementFilter : public AbstractFilter {
public:
    TextEnhancementFilter(){};
    TextEnhancementFilter(const TextEnhancementFilter&) = default; // Constructeur de copie
    void apply(QImage& image, ContentType contentType) const override;
    std::unique_ptr<AbstractFilter> clone() const override;

private:
    void enhanceEdges(QImage& image) const;
    void reduceJPEGArtifacts(QImage& image) const;
};
#endif // FILTER_H

