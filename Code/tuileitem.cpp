#include "tuileitem.h"
#include <cmath>

TuileItem::TuileItem(Tuile* t, int index, double rayon)
    : modeleTuile(t), indexChantier(index)
{
    // On permet la sélection du groupe entier
    setFlag(QGraphicsItem::ItemIsSelectable);

    // Calculs géométriques (identiques à ceux de MainWindow::dessinerCite)
    // Note : w et h dépendent de l'orientation de vos hexagones (pointes en haut ou sur les côtés)
    // Ici je reprends votre logique vue dans mainwindow.cpp
    double w = 1.53 * rayon;
    double h = sqrt(3.0) * rayon;

    // Pour chaque hexagone de la tuile (0, 1, 2)
    // On itère manuellement car on a besoin de savoir "qui est où" relativement
    // La logique de placement relatif (0,0), (0,-1) etc. est interne à la classe Tuile/Cite
    // Mais ici, on veut juste dessiner la forme "locale" de la tuile.

    // Position relative des hexagones dans une tuile standard (non posée sur la grille)
    // Hexagone 0 : Centre (0, 0)
    // Hexagone 1 : Sud (0, -1) -> graphique Y augmente vers le bas en Qt, attention au sens
    // Hexagone 2 : Côté (selon inversion)

    // Coordonnées RELATIVES visuelles pour dessiner la tuile
    struct Offset { int x; int y; };
    std::vector<Offset> offsets;

    // Centre
    offsets.push_back({0, 0});

    // Sud (dans le repère Qt, Y vers le bas, donc y+1)
    offsets.push_back({0, 1});

    // Côté (Dépend de l'inversion et de la parité, c'est la partie complexe)
    // Pour l'affichage dans le chantier, on peut simplifier en fixant une forme par défaut
    // ou en réutilisant la logique de Coord::cote.
    // Supposons une forme standard pour l'affichage :
    if (t->getInversion()) {
        offsets.push_back({1, 0}); // Exemple : droite
    } else {
        offsets.push_back({-1, 0}); // Exemple : gauche
    }

    double sideX = offsets[2].x * w;
    double shiftX = -(sideX / 3.0);

    for (int i = 0; i < 3; ++i) {
        Hexagone* hexModel = t->getHexagone(i);

        // Calcul position pixel relative au centre du groupe (0,0)
        int relX = offsets[i].x;
        int relY = offsets[i].y;

        double pixelX = relX * w;
        // Décalage vertical pour les colonnes impaires (logique hexagonale)
        double pixelY = relY * h;
        if (relX % 2 != 0) {
            pixelY += h / 2.0;
        }
        pixelX += shiftX;
        QColor couleur = getTypeColor(hexModel->getType());

        int nbEtoiles = hexModel->estPlace() ? hexModel->getEtoiles() : 0;
        // On crée l'hexagone visuel (pas besoin de coordonnées de grille ici, c'est de la déco)
        HexagoneItem* item = new HexagoneItem(pixelX, pixelY, rayon, couleur, 0, 0, 0, nbEtoiles);

        // AJOUT AU GROUPE
        addToGroup(item);
    }
}

void TuileItem::mousePressEvent(QGraphicsSceneMouseEvent *event)  {

    // C'est ici que vous exécutez votre fonction
    // Important : Passer l'événement à la classe parente si nécessaire
    QGraphicsItemGroup::mousePressEvent(event);
    emit clicked(indexChantier);
}

QColor TuileItem::getTypeColor(TypeQuartier t) {
    // Copie de votre logique existante ou appel à une fonction statique
    switch(t) {
    case Habitation: return Qt::blue;
    case Marche:     return Qt::yellow;
    case Caserne:    return Qt::red;
    case Temple:     return Qt::magenta;
    case Jardin:     return Qt::green;
    case Carriere:   return Qt::gray;
    default:         return Qt::white;
    }
}
