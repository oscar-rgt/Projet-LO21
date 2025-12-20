#include "score.h"
#include "partie.h"
#include "joueur.h"
#include "IA.h" 
#include "cite.h" 
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

// =========================================================
// OUTILS COMMUNS
// =========================================================
// Compte le nombre total d'étoiles (multiplicateurs) pour une couleur donnée dans toute la cité.
int RegleScore::compterEtoiles(Cite* cite, TypeQuartier typeQ) const {
    int etoiles = 0;
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        // On ne compte que les hexagones placés et correspondant au type demandé
        if (h && h->estPlace() && h->getType() == typeQ) {
            etoiles += h->getEtoiles();
        }
    }
    return etoiles;
}

// =========================================================
// 1. HABITATIONS (Bleu)
// =========================================================

int RegleHabitation::getValeurPlusGrandGroupe(Cite* cite) const {
    // Cas de base : cité vide
    if (cite->begin() == cite->end()) return 0;

    // Vecteur pour mémoriser les hexagones déjà visités 
    vector<Coord> globalVisites;
    int maxValeur = 0;

	// Hauteur max pour chercher verticalement
    int hMax = cite->getHauteurMax();

    // 1. On parcourt chaque tuile de la cité comme point de départ potentiel
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Coord depart = it->first;
        Hexagone* h = it->second;

        // Vérification si ce point a déjà été traité dans un groupe précédent
        bool dejaVu = false;
        for (const auto& v : globalVisites) { if (v == depart) { dejaVu = true; break; } }

        // Si c'est une Habitation standard (pas une Place) et non visitée, on lance la recherche
        if (h->getType() == Habitation && !h->estPlace() && !dejaVu) {

            // --- DÉBUT RECHERCHE ---
            int valeurGroupe = 0;
            vector<Coord> aTraiter; // File d'attente

            aTraiter.push_back(depart);
            globalVisites.push_back(depart);

            // Tant qu'il reste des tuiles à explorer dans le groupe
            for (size_t i = 0; i < aTraiter.size(); ++i) {
                Coord actuelle = aTraiter[i];
                valeurGroupe += (actuelle.z + 1); // La valeur dépend de la hauteur (z+1)

				// Récupération des 6 voisins en fonction de la parité de x
                auto voisinsVecteurs = cite->getVecteursVoisins(actuelle.x % 2);

                for (const auto& vec : voisinsVecteurs) {
                    // Calcul des coordonnées cibles en vue 
                    int targetX = actuelle.x + vec.x;
                    int targetY = actuelle.y + vec.y;


                    // On cherche la tuile visible (la plus haute) à la coordonnée (targetX, targetY).
                    for (int zTest = 0; zTest <= hMax; ++zTest) {
                        bool trouve = false;
                        Hexagone* hVoisin = nullptr;
                        Coord voisinCoord;

                        for (auto itScan = cite->begin(); itScan != cite->end(); ++itScan) {
                            if (itScan->first.x == targetX &&
                                itScan->first.y == targetY &&
                                itScan->first.z == zTest) {

                                trouve = true;
                                hVoisin = itScan->second;
                                voisinCoord = itScan->first;
                                break;
                            }
                        }

                        if (trouve) {
                            // Si on a trouvé la tuile visible de cette colonne

                            // Vérification doublon local
                            bool voisinDejaVu = false;
                            for (const auto& v : globalVisites) { if (v == voisinCoord) { voisinDejaVu = true; break; } }

                            // Si c'est une habitation connectée au groupe
                            if (hVoisin->getType() == Habitation && !hVoisin->estPlace() && !voisinDejaVu) {
                                aTraiter.push_back(voisinCoord);
                                globalVisites.push_back(voisinCoord);
                            }

                            // Puisque les tuiles recouvertes sont supprimées, 
                            // il n'y a qu'une tuile visible par colonne. On arrête le scan vertical.
                            break;
                        }
                    }
                }
            }
            // Mise à jour du maximum trouvé
            if (valeurGroupe > maxValeur) maxValeur = valeurGroupe;
        }
    }
    return maxValeur;
}

int RegleHabitation::calculer(Cite* cite) const {
    return getValeurPlusGrandGroupe(cite) * compterEtoiles(cite, Habitation);
}

int RegleHabitationVariante::calculer(Cite* cite) const {
    int valeur = getValeurPlusGrandGroupe(cite);
    int score = valeur * compterEtoiles(cite, Habitation);
    // Bonus variante : score doublé si la valeur brute du groupe >= 10
    if (valeur >= 10) score *= 2;
    return score;
}

// =========================================================
// 2. MARCHÉS (Jaune)
// =========================================================

int RegleMarche::calculer(Cite* cite) const {
    int valeur = 0;
	//parcours de tous les hexagones de la cité
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
		if (h->getType() == Marche && !h->estPlace()) { //on trouve un hexagone marché qui n'est pas une place
            bool isole = true;
            // Utilisation de la méthode helper de Cite pour récupérer les voisins immédiats
			auto voisins = cite->getAdjacents(it->first); //on récupère les voisins
            for (Hexagone* v : voisins) {
				// Si un voisin est aussi un marché (et pas une Place Etoile), il n'est pas isolé et on arrete la boucle
                if (v->getType() == Marche && !v->estPlace()) { isole = false; break; }
            }
            if (isole) valeur += (it->first.z + 1);
        }
    }
    return valeur * compterEtoiles(cite, Marche);
}

int RegleMarcheVariante::calculer(Cite* cite) const {
    int total = 0;
    int etoiles = compterEtoiles(cite, Marche);

    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Marche && !h->estPlace()) {
            bool isole = true;
            bool adjacentPlace = false;
            auto voisins = cite->getAdjacents(it->first);
            for (Hexagone* v : voisins) {
                if (v->getType() == Marche && !v->estPlace()) isole = false;
                // Variante : Bonus spécifique si adjacent à une Place de type Marche
                if (v->estPlace() && v->getType() == Marche) adjacentPlace = true;
            }
            if (isole) {
                int pts = (it->first.z + 1) * etoiles;
                if (adjacentPlace) pts *= 2;
                total += pts;
            }
        }
    }
    return total;
}

// =========================================================
// 3. CASERNES (Rouge)
// =========================================================

int RegleCaserne::calculer(Cite* cite) const {
    int valeur = 0;
	//parcours de tous les hexagones de la cité
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
		if (h->getType() == Caserne && !h->estPlace()) { //on trouve un hexagone caserne qui n'est pas une place
			if (cite->getAdjacents(it->first).size() < 6) { //si le nombre de voisins est inférieur à 6 (en periphérie)
                valeur += (it->first.z + 1);
            }
        }
    }
    return valeur * compterEtoiles(cite, Caserne);
}

int RegleCaserneVariante::calculer(Cite* cite) const {
    int total = 0;
    int etoiles = compterEtoiles(cite, Caserne);

    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Caserne && !h->estPlace()) {
            size_t nbVoisins = cite->getAdjacents(it->first).size();
            if (nbVoisins < 6) {
                int pts = (it->first.z + 1) * etoiles;
                int vides = 6 - (int)nbVoisins;
                // Variante : Points doublés si 3 ou 4 voisins 
                if (vides == 3 || vides == 4) pts *= 2;
                    total += pts;
                }
        }
    }
    return total;
}

// =========================================================
// 4. TEMPLES (Violet)
// =========================================================

int RegleTemple::calculer(Cite* cite) const {
    int valeur = 0;
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Temple && !h->estPlace()) {
            // Le temple marque s'il est complètement entouré (6 voisins)
            if (cite->getAdjacents(it->first).size() == 6) {
                valeur += (it->first.z + 1);
            }
        }
    }
    return valeur * compterEtoiles(cite, Temple);
}

int RegleTempleVariante::calculer(Cite* cite) const {
    int total = 0;
    int etoiles = compterEtoiles(cite, Temple);

    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Temple && !h->estPlace()) {
            if (cite->getAdjacents(it->first).size() == 6) {
                int pts = (it->first.z + 1) * etoiles;
                // Variante : Points doublés si en hauteur (z >= 1)
                if (it->first.z > 0) pts *= 2;
                total += pts;
            }
        }
    }
    return total;
}

// =========================================================
// 5. JARDINS (Vert)
// =========================================================

int RegleJardin::calculer(Cite* cite) const {
    int valeur = 0;
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Jardin && !h->estPlace()) {
            valeur += (it->first.z + 1);
        }
    }
    return valeur * compterEtoiles(cite, Jardin);
}

int RegleJardinVariante::calculer(Cite* cite) const {
    int total = 0;
    int etoiles = compterEtoiles(cite, Jardin);

	// On récupère la hauteur max pour savoir jusqu'où regarder verticalement
    int hMax = cite->getHauteurMax();

	// Parcours de tous les hexagones de la cité
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
		if (h->getType() == Jardin && !h->estPlace()) { // On trouve un hexagone jardin qui n'est pas une place

            // Calcul du score de base : (Hauteur + 1) * Nombre d'étoiles
            int pts = (it->first.z + 1) * etoiles;

            bool adjacentLac = false;

			// Récupération des vecteurs voisins en fonction de la parité de x
            auto voisinsVecteurs = cite->getVecteursVoisins(it->first.x % 2); 

			// Analyse du voisinage pour détecter un Lac
            for (const auto& vec : voisinsVecteurs) {
                int targetX = it->first.x + vec.x;
                int targetY = it->first.y + vec.y;

                // On regarde toute la colonne voisine pour voir s'il y a un Lac
                for (int zTest = 0; zTest <= hMax; ++zTest) {
                    Coord voisinCoord = { targetX, targetY, zTest };

                    // 1. Vérification manuelle de l'existence d'une tuile
                    bool caseOccupee = false;
                    for (auto itScan = cite->begin(); itScan != cite->end(); ++itScan) {
                        if (itScan->first == voisinCoord) { // Suppose que l'opérateur == existe pour Coord
                            caseOccupee = true;
                            break;
                        }
                    }

                    // 2. Si la case est vide (c'est un trou potentiel), on vérifie si c'est un Lac
                    if (!caseOccupee) {
                        // Un lac est défini par 6 voisins visibles autour du vide
                        if (cite->getAdjacents(voisinCoord).size() == 6) {
                            adjacentLac = true;
                            break; 
                        }
                    }
                    else {
                        break;
                    }
                }
                if (adjacentLac) break; // Si on a trouvé un lac adjacent, on passe au jardin suivant
            }

            if (adjacentLac) pts *= 2;
            total += pts;
        }
    }
    return total;
}

// =========================================================
// CLASSE SCORE
// =========================================================

Score::Score(Joueur* j) : joueur(j) {

    const auto& vars = Partie::getInstance().getVariantes();

	// on remplit la map des stratégies en fonction des variantes actives
    if (vars[0]) strategies[Habitation] = new RegleHabitationVariante();
    else         strategies[Habitation] = new RegleHabitation();

    if (vars[1]) strategies[Marche] = new RegleMarcheVariante();
    else         strategies[Marche] = new RegleMarche();

    if (vars[2]) strategies[Caserne] = new RegleCaserneVariante();
    else         strategies[Caserne] = new RegleCaserne();

    if (vars[3]) strategies[Temple] = new RegleTempleVariante();
    else         strategies[Temple] = new RegleTemple();

    if (vars[4]) strategies[Jardin] = new RegleJardinVariante();
    else         strategies[Jardin] = new RegleJardin();
}

Score::~Score() {
    for (auto& pair : strategies) delete pair.second;
    strategies.clear();
}

int Score::calculerScore() const {
    int totalCalcule = 0;
    // Cas spécial : IA (Optimisation)
    // Si c'est une IA, elle utilise un calcul simplifié 
    if(IA* ia = dynamic_cast<IA*>(joueur)) {
		
        // Maps pour stocker les comptes séparés
        // nbQuartiers : compte uniquement les tuiles Quartier (pas les places)
        // nbEtoiles : compte uniquement les étoiles des Places
        map<TypeQuartier, int> nbQuartiers;
        map<TypeQuartier, int> nbEtoiles;
        int nbCarrieres = 0;

        // --- 1. INVENTAIRE ---
        for (auto it = ia->begin(); it != ia->end(); ++it) {
            Tuile* t = *it;

            for (int i = 0; i < t->getNbHexagones(); ++i) {
                Hexagone* h = t->getHexagone(i);
                TypeQuartier type = h->getType();

                if (type == Carriere) {
                    nbCarrieres++;
                }
                else {
                    if (nbQuartiers.find(type) == nbQuartiers.end()) nbQuartiers[type] = 0;
                    if (nbEtoiles.find(type) == nbEtoiles.end()) nbEtoiles[type] = 0;

                    if (h->estPlace()) {
                        nbEtoiles[type] += h->getEtoiles();
                    }
                    else {
                        nbQuartiers[type]++;
                    }
                }
            }
        }

        // --- 2. RÈGLES DE DIFFICULTÉ  ---
        // Hippodamos (1) & Métagénès (2) : Hauteur = 1
        // Callicratès (3) : Hauteur = 2
        int hauteur = (ia->getDifficulte() == 3) ? 2 : 1;

        // --- 3. CALCUL DU SCORE ---
        // On parcourt les types de quartiers rencontrés
        for (map<TypeQuartier, int>::iterator it = nbQuartiers.begin(); it != nbQuartiers.end(); ++it) {
            TypeQuartier type = it->first;
            int nombreDeQuartiers = it->second; // Nombre d'hexagones "non-place"
            int nombreEtoiles = nbEtoiles[type]; // Total des étoiles accumulées

            // Formule : Quartiers x Etoiles x Hauteur
            totalCalcule += nombreDeQuartiers * nombreEtoiles * hauteur;
        }

        // --- 4. POINTS DES PIERRES ---
        totalCalcule += joueur->getPierres();

        // --- 5. BONUS MÉTAGÉNÈS (Difficulté 2) ---
        if (ia->getDifficulte() == 2) {
            totalCalcule += nbCarrieres * 2;
        }
	}
    else {
        // CAS JOUEUR HUMAIN
        Cite* maCite = joueur->getCite();
        if (!maCite) return 0;

        // Délégation du calcul à chaque stratégie
        for (auto const& pair : strategies) {
            if (pair.second) {
                totalCalcule += pair.second->calculer(maCite);
            }
        }

		// Ajout des pierres au score final
        totalCalcule += joueur->getPierres();
    }
    

    return totalCalcule;
}
