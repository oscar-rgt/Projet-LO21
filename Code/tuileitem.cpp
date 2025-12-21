#include "tuileitem.h"
#include <Qcolor>
#include <cmath>


TuileItem::TuileItem(Tuile* t, int index, double rayon): modeleTuile(t), indexChantier(index){

    setFlag(QGraphicsItem::ItemIsSelectable);
    setHandlesChildEvents(false); //désactive la gestion d'événements enfants

    //dimensions de nos hexagones
    double w = 1.53 * rayon;
    double h = sqrt(3.0) * rayon;

    int nbHex = t->getNbHexagones(); //3 ou 4 selon si tuile départ ou non

	//vecteur des positions possibles des hexagones dans la tuile
    struct Position { double x; double y; };
    std::vector<Position> pos;

	if (nbHex == 4) { //tuile de départ

        //centre
        pos.push_back({0.0, 0.0});

        //haut
        pos.push_back({0.0, -1.0});

        //bas gauche
        pos.push_back({-1.0, 0.5});

        //bas droite
        pos.push_back({1.0, 0.5});

    } else { //tuile normale
        //centre
        pos.push_back({0.0, 0.0});

        //bas
        pos.push_back({0.0, 1.0});

		//coté (gauche ou droite selon inversion)
        if (t->getInversion()) {
            pos.push_back({1.0, 0.5}); //à droite
        } else {
            pos.push_back({-1.0, 0.5}); //à gauche
        }
    }

	//centrage de la tuile sous la souris :
	double decalageX = 0; //la tuile de depart n'a pas de decalage
    if (nbHex == 3) {
        //on ramène l'hexagone principal (pos centre) sous la souris
		double coteX = pos[2].x * w; //on trouve la position pixel de l'hexagone avec pos = coté
		decalageX = -(coteX / 3.0); //on décale d'un tiers de cette position
    }

	//on crée la tuile hexagone par hexagone
    for (int i = 0; i < nbHex; ++i) {
        Hexagone* hexModel = t->getHexagone(i);

        //sécurité : si on a défini moins de pos que d'hexagones (ne devrait pas arriver)
        if (i >= static_cast<int>(pos.size())) break;

		//récupération des coordonnées relatives de l'hexagone
        double relX = pos[i].x;
        double relY = pos[i].y;

		//transforme les positions relatives en valeur pixel écran
		double pixelX = relX * w; //ex : si relX = 3, l'hexagone sera dessiné en x = 3 * largeur_hexagone (défini plus haut)
        double pixelY = relY * h;

		//application du décalage pour centrer la tuile sous la souris
		//tous les hexagones sont décalés de la même valeur vers la droite ou la gauche
        pixelX += decalageX;

		//récupération de la couleur et du nombre d'étoiles de chaque hexagone
        QColor couleur = getTypeColor(hexModel->getType());
        int nbEtoiles = hexModel->estPlace() ? hexModel->getEtoiles() : 0;

		//création et ajout de l'HexagoneItem au groupe (qui forme la tuile)
        HexagoneItem* item = new HexagoneItem(pixelX, pixelY, rayon, couleur, 0, 0, 0, nbEtoiles);
        addToGroup(item);
    }
}


void TuileItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItemGroup::mousePressEvent(event);
    emit clicked(indexChantier); //emet le signal avec l'index de la tuile
}

//methode statique pour obtenir la couleur de l'hexagone selon son type
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

//méthode pour marquer la sélection d'une tuile lorsqu'on clique dessus
void TuileItem::setSelection(bool estSelectionne) {
    //on parcourt les enfants du groupe (les 3 hexagones)
    for (QGraphicsItem* enfant : childItems()) {
        //on vérifie que c'est bien un HexagoneItem
        HexagoneItem* hex = dynamic_cast<HexagoneItem*>(enfant);
        if (hex) {
			//si oui on applique la sélection
            hex->setSelection(estSelectionne);
        }
    }
}
