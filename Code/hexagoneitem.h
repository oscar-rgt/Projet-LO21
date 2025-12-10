#ifndef HEXAGONEITEM_H
#define HEXAGONEITEM_H

#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include <QFont>

class HexagoneItem : public QGraphicsPolygonItem {
public:
    // On stocke les coordonnées logiques du jeu pour renvoyer l'info au clic
    int gridX, gridY, gridZ;

    HexagoneItem(double x, double y, double rayon, QColor couleur, int gx, int gy, int gz, int etoiles = 0, QGraphicsItem* parent = nullptr)
        : QGraphicsPolygonItem(parent), gridX(gx), gridY(gy), gridZ(gz) {

        // Création du polygone hexagonal
        QPolygonF hex;
        for (int i = 0; i < 6; ++i) {
            double angle = 60 * i;
            double rad = 3.14159 / 180 * angle;
            hex << QPointF(x + rayon * cos(rad), y + rayon * sin(rad));
        }
        //setRotation(30);
        setPolygon(hex);
        setBrush(QBrush(couleur));
        setPen(QPen(Qt::black));

        // Effet de profondeur : plus c'est haut (Z), plus on affiche devant
        setZValue(gz);
        setZValue(gz);

        // 2. DESSIN DES ÉTOILES (NOUVEAU)
        if (etoiles > 0) {
            // Création d'une chaîne avec N étoiles
            QString strEtoiles;
            for(int k=0; k<etoiles; k++) strEtoiles += "★"; // Caractère Unicode étoile pleine

            QGraphicsTextItem* txt = new QGraphicsTextItem(strEtoiles, this);

            // Style : Blanc, Gras, taille adaptée au rayon
            txt->setDefaultTextColor(Qt::black);
            QFont font = txt->font();
            font.setPixelSize(static_cast<int>(rayon * 0.45)); // Taille relative
            font.setBold(true);
            txt->setFont(font);

            // Centrage approximatif
            // On centre le texte par rapport à l'origine (0,0) de l'hexagone
            QRectF bound = txt->boundingRect();
            txt->setPos(-bound.width() / 2, -bound.height() / 2);
        }
    }


protected:
    // gérer le clic pour placer une tuile
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        // Logique d'émission de signal vers MainWindow à implémenter ici
        QGraphicsPolygonItem::mousePressEvent(event);
    }
};

#endif
