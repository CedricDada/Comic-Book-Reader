#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "../model/AbstractImage.h"
#include "../infrastructure/Filter.h"
#include <QImage>
#include <qimage.h>

class ImageProcessor {
public:
    /* Redimensionnement intelligent avec préservation des proportions
     * Paramètres :
     * - image : Image à traiter (modifiée directement)
     * - width : Largeur cible
     * - height : Hauteur cible
     * - keepDetails : Conserve les détails textuels (défaut=true)
     */
    static void resize(AbstractImage* image, int width, int height, bool keepDetails = true);

    /* Application de filtres adaptatifs selon le type de contenu
     * Paramètres :
     * - image : Image à filtrer
     * - contentType : Type de contenu dominant
     * - intensity : Force d'application des filtres [0.0-1.0]
     */
    static void applyFilter(AbstractImage* image, ContentType contentType, float intensity = 1.0f);

    /* Optimisation pour l'affichage écran
     * Applique une chaîne de prétraitements pour :
     * - Réduction du bruit
     * - Amélioration des contours
     * - Ajustement des couleurs
     */
    static void optimizeForDisplay(AbstractImage* image);

    /* Pipeline principal de traitement d'image
     * Combine redimensionnement + filtrage + optimisation
     * Retourne l'image traitée pour affichage immédiat
     */
    static QImage processImage(AbstractImage* image) {
        QImage processed = image->toQImage();
        // Exemple de pipeline de base (à personnaliser)
        processed = processed.convertToFormat(QImage::Format_ARGB32);
        return processed;
    }

    static QImage::Format parseFormat(AbstractImage::Format format);



private:
    // Conversion sécurisée avec copie des données
    static QImage convertToQImage(const AbstractImage* image);
    
    // Mise à jour des données de l'image source
    static void updateImageData(AbstractImage* image, const QImage& processed);
};
#endif // IMAGEPROCESSOR_H