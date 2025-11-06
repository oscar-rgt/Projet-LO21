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

