#include "hexagoneitem.h"


HexagoneItem::HexagoneItem(double x, double y, double rayon, QColor couleur, int gx, int gy, int gz, int etoiles, QGraphicsItem* parent): QGraphicsPolygonItem(parent), gridX(gx), gridY(gy), gridZ(gz) {

    //création de l'hexagone
    QPolygonF hex;
    for (int i = 0; i < 6; ++i) {
		//calcul des 6 sommets de l'hexagone
		double angle = 60 * i; //un sommet tous les 60 degrés
		double rad = 3.14159 / 180 * angle; //conversion en radians
        //les points sont ajoutés un par un
		//on utilise cos et sin pour calculer les coordonnées x et y 
		//projection polaire vers cartésienne
        hex << QPointF(rayon * cos(rad),rayon * sin(rad)); 
    }

	//dessin de l'hexagone
    setPolygon(hex);
    setBrush(QBrush(couleur));
    setPen(QPen(Qt::black));

	//positionnement de l'hexagone
    setPos(x, y);
    setZValue(gz);




	//dessin des étoiles si nécessaire :
    if (etoiles > 0) {
        QString strEtoiles;
        for(int k=0; k<etoiles; k++) strEtoiles += "★"; //création de la chaîne d'étoiles

        QGraphicsTextItem* txt = new QGraphicsTextItem(strEtoiles, this);

        //style de l'étoile
        txt->setDefaultTextColor(Qt::black);
        QFont font = txt->font();
        font.setPixelSize(static_cast<int>(rayon * 0.45)); 
        font.setBold(true);
        txt->setFont(font);

        //on centre létoile par rapport à l'origine de l'hexagone
        QRectF bound = txt->boundingRect();
        txt->setPos(-bound.width() / 2, -bound.height()/2);


    }
}

//méthode pour gérer la sélection/désélection de l'hexagone
//utilisée dans TuileItem::setSelection
void HexagoneItem::setSelection(bool estSelectionne) {
    QPen pen(Qt::black);
    if (estSelectionne) {
		//l'hexagone a un contour spécial pour marquer la sélection
        pen.setWidth(2);
        pen.setStyle(Qt::DashLine); 
        setPen(pen);

		setZValue(gridZ + 10); //passage au premier plan
    } else {
        //retour au style normal
        setZValue(gridZ);
    }

	QColor c = brush().color(); //on récupère la couleur actuelle de l'hexagone

    if (estSelectionne) {
		c.setAlpha(170); //si sélectionné on rend l'hexagone semi-transparent
    } else {
		c.setAlpha(255); //sinon on remet l'opacité totale
    }

    setBrush(QBrush(c)); //on applique la couleur modifiée
}
