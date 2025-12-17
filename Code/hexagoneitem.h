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

    HexagoneItem(double x, double y, double rayon, QColor couleur, int gx, int gy, int gz, int etoiles = 0, QGraphicsItem* parent = nullptr);

    void setSelection(bool estSelectionne);
};

#endif
