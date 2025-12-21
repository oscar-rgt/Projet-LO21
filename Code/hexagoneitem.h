#ifndef HEXAGONEITEM_H
#define HEXAGONEITEM_H

#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include <QFont>

//la classe hérite de QGraphicsPolygonItem pour dessiner un hexagone dans une scène graphique
class HexagoneItem : public QGraphicsPolygonItem {
public:
	int gridX, gridY, gridZ; //coordonnées dans la grille hexagonale

    HexagoneItem(double x, double y, double rayon, QColor couleur, int gx, int gy, int gz, int etoiles = 0, QGraphicsItem* parent = nullptr);

    void setSelection(bool estSelectionne);
};

#endif
