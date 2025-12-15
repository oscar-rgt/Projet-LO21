#include "tuileitem.h"
#include <cmath>

// Constructeur de TuileItem
TuileItem::TuileItem(Tuile* t, int index, double rayon)
    : modeleTuile(t), indexChantier(index)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setHandlesChildEvents(false); // Désactive la gestion des événements enfants

    // Calcul des dimensions de l'hexagone
    double w = 1.53 * rayon;
    double h = sqrt(3.0) * rayon;

    // Définition des offsets relatifs pour les 3 hexagones de la tuile
    // Ces offsets dépendent de l'inversion de la tuile
    struct Offset { double x; double y; };
    std::vector<Offset> offsets;

    // Hexagone central (toujours au centre)
    offsets.push_back({0.0, 0.0});

    // Hexagone sud (toujours en bas)
    offsets.push_back({0.0, 1.0});

    // Hexagone latéral (gauche ou droite selon l'inversion)
    if (t->getInversion()) {
        offsets.push_back({1.0, 0.5}); // À droite
    } else {
        offsets.push_back({-1.0, 0.5}); // À gauche
    }

    double sideX = offsets[2].x * w;
    double shiftX = -(sideX / 3.0); // Ajout d'un décalage pour centrer la tuile
    // Création des HexagoneItem pour chaque hexagone de la tuile
    for (int i = 0; i < 3; ++i) {
        Hexagone* hexModel = t->getHexagone(i);
        double relX = offsets[i].x;
        double relY = offsets[i].y;

        // Calcul des coordonnées pixel en fonction des offsets
        double pixelX = relX * w;
        double pixelY = relY * h;


        pixelX += shiftX;

        // Récupération de la couleur et du nombre d'étoiles
        QColor couleur = getTypeColor(hexModel->getType());
        int nbEtoiles = hexModel->estPlace() ? hexModel->getEtoiles() : 0;

        // Création et ajout de l'HexagoneItem au groupe
        HexagoneItem* item = new HexagoneItem(pixelX, pixelY, rayon, couleur, 0, 0, 0, nbEtoiles);
        addToGroup(item);
    }
}


// Gestion de l'événement de clic
void TuileItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItemGroup::mousePressEvent(event);
    emit clicked(indexChantier); // Émet le signal avec l'index de la tuile
}

// Méthode statique pour obtenir la couleur associée à un type de quartier
QColor TuileItem::getTypeColor(TypeQuartier t)
{
    switch(t) {
    case Habitation: return Qt::blue;
    case Marche:     return Qt::yellow;
    case Caserne:    return Qt::red;
    case Temple:     return Qt::darkMagenta;
    case Jardin:     return Qt::green;
    case Carriere:   return Qt::gray;
    default:         return Qt::white;
    }
}

void TuileItem::setSelection(bool estSelectionne) {
    // On parcourt les enfants du groupe (les 3 hexagones)
    for (QGraphicsItem* enfant : childItems()) {
        // On vérifie que c'est bien un HexagoneItem
        HexagoneItem* hex = dynamic_cast<HexagoneItem*>(enfant);
        if (hex) {
            hex->setSelection(estSelectionne);
        }
    }
}
