/* NUMERO 1
#include "score.h"

// compte combien de voisins d'un type donné
int nb_voisins(Type voisins[6], Type t) {
    int c = 0;
    for (int i = 0; i < 6; i++)
        if (voisins[i] == t) c++;
    return c;
}

bool temple_valide(Type voisins[6]) {
    // doit avoir 4 voisins de couleurs différentes (pas Place)
    int count = 0;
    Type couleurs[4];
    for (int i = 0; i < 6; i++) {
        if (voisins[i] != Place && voisins[i] != (Type)0) {
            bool deja = false;
            for (int j = 0; j < count; j++)
                if (voisins[i] == couleurs[j]) deja = true;
            if (!deja)
                couleurs[count++] = voisins[i];
        }
    }
    return (count >= 4);
}

int calcul_points(ScoreInput centre, Type voisins[6]) {
    int base = 0;

    // 1️⃣ règles selon le type
    if (centre.type == Caserne) { // BLEU
        int nbBleus = nb_voisins(voisins, Caserne);
        if (nbBleus == 0)
            base = 0;                // isolé
        else if (nbBleus == 1)
            base = 2;                // active son voisin + elle-même
        else
            base = 1;                // ajoute à un groupe
    }

    else if (centre.type == Marche) { // JAUNE
        int nbJaunes = nb_voisins(voisins, Marche);
        if (nbJaunes == 0)
            base = +1;                // isolé
        else
            base = -nbJaunes;         // casse les isolés adjacents
    }

    else if (centre.type == Habitation) { // ROUGE
        base = +1;
    }

    else if (centre.type == Temple) { // VIOLET
        if (temple_valide(voisins))
            base = +1;
    }

    else if (centre.type == Jardin) { // VERT
        base = +2;
    }

    // 2️⃣ Multiplicateur étoiles
    int score = base * centre.etoiles;

    // 3️⃣ Bonus de hauteur (niveau - 1)
    score += (centre.niveau - 1);

    return score;
}

ScoreInput s;
s.type = t.getHexagone(0).getType(); // ou celui du centre
s.niveau = t.getHexagone(0).getNiveau();
s.etoiles = t.getHexagone(0).getEtoiles();

// tableau des 6 voisins du centre
Type voisins[6] = { ... }; // à remplir selon la grille autour

int gain = calcul_points(s, voisins);
cout << "→ Cette pose rapporte " << gain << " points !" << endl;

*/ 












/* NUMERO 2
#include "score.h"
#include <iostream>

using namespace std;

// Vérifie les voisins d’un hexagone sur le plateau
vector<Hexagone*> getVoisins(vector<vector<Hexagone>>& plateau, int ligne, int colonne) {
    vector<Hexagone*> voisins;
    int maxL = plateau.size();
    int maxC = plateau[0].size();

    // Coordonnées voisines (approximation grille hexagonale)
    int directions[6][2] = {
        {-1, 0}, {+1, 0}, // haut / bas
        {0, -1}, {0, +1}, // gauche / droite
        {-1, +1}, {+1, -1} // diagonales
    };

    for (auto &d : directions) {
        int nl = ligne + d[0];
        int nc = colonne + d[1];
        if (nl >= 0 && nl < maxL && nc >= 0 && nc < maxC) {
            voisins.push_back(&plateau[nl][nc]);
        }
    }

    return voisins;
}


// Calcule le score d’un seul hexagone
int calculPoints(Hexagone& h, vector<vector<Hexagone>>& plateau, int ligne, int colonne) {
    if (h.est_recouvert()) return 0; // rien s’il est recouvert

    int score = 0;
    int niveau = h.getNiveau();
    int etoiles = h.getEtoiles();
    auto voisins = getVoisins(plateau, ligne, colonne);

    switch (h.getType()) {

        case Habitation: {
            // +1 par habitation dans le même groupe, multiplié par étoiles adjacentes
            int nbH = 1;
            for (auto v : voisins)
                if (v->getType() == Habitation && !v->est_recouvert()) nbH++;

            score = nbH * etoiles * niveau;
            break;
        }

        case Marche: {
            // +1 par couleur différente autour
            int couleurs[5] = {0};
            for (auto v : voisins)
                if (!v->est_recouvert())
                    couleurs[v->getType()] = 1;

            int diversite = 0;
            for (int i = 0; i < 5; i++) diversite += couleurs[i];

            score = diversite * etoiles * niveau;
            break;
        }

        case Caserne: {
            // +1 si sur le bord
            if (ligne == 0 || colonne == 0 || ligne == (int)plateau.size() - 1 || colonne == (int)plateau[0].size() - 1)
                score = etoiles * niveau;
            break;
        }

        case Temple: {
            // +1 si entouré de 4 types différents
            int types[6] = {0};
            for (auto v : voisins)
                if (!v->est_recouvert())
                    types[v->getType()] = 1;

            int count = 0;
            for (int i = 0; i < 6; i++) count += types[i];
            if (count >= 4)
                score = etoiles * niveau;
            break;
        }

        case Jardin: {
            // +1 pour chaque jardin visible
            score = etoiles * niveau;
            break;
        }

        case Place:
            // La place ne rapporte rien directement
            score = 0;
            break;
    }

    return score;
}


// Calcule le score total du plateau
int calculScore(vector<vector<Hexagone>>& plateau) {
    int total = 0;
    for (int i = 0; i < (int)plateau.size(); i++) {
        for (int j = 0; j < (int)plateau[i].size(); j++) {
            total += calculPoints(plateau[i][j], plateau, i, j);
        }
    }
    return total;
}
*/
