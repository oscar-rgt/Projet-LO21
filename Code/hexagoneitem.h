#ifndef HEXAGONEITEM_H
#define HEXAGONEITEM_H

#include <QGraphicsPolygonItem>
#include <QBrush>
#include <QPen>

class HexagoneItem : public QGraphicsPolygonItem {
public:
    // On stocke les coordonnées logiques du jeu pour renvoyer l'info au clic
    int gridX, gridY, gridZ;

    HexagoneItem(double x, double y, double rayon, QColor couleur, int gx, int gy, int gz, QGraphicsItem* parent = nullptr)
        : QGraphicsPolygonItem(parent), gridX(gx), gridY(gy), gridZ(gz) {

        // Création du polygone hexagonal
        QPolygonF hex;
        for (int i = 0; i < 6; ++i) {
            double angle = 60 * i - 30;
            double rad = 3.14159 / 180 * angle;
            hex << QPointF(x + rayon * cos(rad), y + rayon * sin(rad));
        }
        setPolygon(hex);
        setBrush(QBrush(couleur));
        setPen(QPen(Qt::black));

        // Effet de profondeur : plus c'est haut (Z), plus on affiche devant
        setZValue(gz);
    }

protected:
    // gérer le clic pour placer une tuile
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        // Logique d'émission de signal vers MainWindow à implémenter ici
        QGraphicsPolygonItem::mousePressEvent(event);
    }
};

#endif
