#include "tuileitem.h"
#include <Qcolor>
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

    int nbHex = t->getNbHexagones();
    // Définition des offsets relatifs pour les 3 hexagones de la tuile
    // Ces offsets dépendent de l'inversion de la tuile
    struct Offset { double x; double y; };
    std::vector<Offset> offsets;

    // --- LOGIQUE DE PLACEMENT ---
    if (nbHex == 4) {
        // CAS TUILE DE DÉPART (4 Hexagones)
        // Forme en "trèfle" ou étoile : 1 centre + 3 autour

        // 0. Centre (Habitation)
        offsets.push_back({0.0, 0.0});

        // 1. Haut (Carrière)
        offsets.push_back({0.0, -1.0});

        // 2. Bas-Gauche (Carrière)
        offsets.push_back({-1.0, 0.5});

        // 3. Bas-Droite (Carrière)
        offsets.push_back({1.0, 0.5});

    } else {
        // CAS TUILE STANDARD (3 Hexagones)
        // 0. Centre
        offsets.push_back({0.0, 0.0});

        // 1. Sud
        offsets.push_back({0.0, 1.0});

        // 2. Latéral (Gauche ou Droite selon inversion)
        if (t->getInversion()) {
            offsets.push_back({1.0, 0.5}); // À droite
        } else {
            offsets.push_back({-1.0, 0.5}); // À gauche
        }
    }

    // Centrage approximatif
    // Pour la tuile départ (centrée en 0,0 avec des voisins autour), le shift est 0.
    // Pour les tuiles standards, on garde votre logique de décalage.
    double shiftX = 0;
    if (nbHex == 3) {
        double sideX = offsets[2].x * w; 
        shiftX = -(sideX / 3.0); // Ajout d'un décalage pour centrer la tuile
    }

    // --- CRÉATION DES ITEMS ---
    // On boucle sur nbHex (3 ou 4) au lieu de 3 fixe
    for (int i = 0; i < nbHex; ++i) {
        Hexagone* hexModel = t->getHexagone(i);

        // Sécurité : si on a défini moins d'offsets que d'hexagones (ne devrait pas arriver)
        if (i >= offsets.size()) break;

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
    case Habitation: return QColor(37, 150, 190);
    case Marche:     return QColor(255, 214, 61);
    case Caserne:    return QColor(233, 73, 53);
    case Temple:     return QColor(106, 77, 152);
    case Jardin:     return QColor(65, 179, 88);
    case Carriere:   return QColor(225, 225, 217);
    default:         return QColor(225, 225, 217);
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
