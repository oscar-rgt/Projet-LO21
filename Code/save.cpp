#include "save.h"
#include "partie.h"
#include "tuiles.h"
#include "joueur.h"
#include "ia.h" 
#include "pile.h"
#include "cite.h" 
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio> // Pour std::remove

using namespace std;

// =========================================================
// SAUVEGARDER
// =========================================================
bool SaveManager::sauvegarder(const Partie& partie, const string& nomFichier) {
    ofstream f(nomFichier);
    if (!f) return false;

    // 1. MODE DE JEU
    bool modeSolo = false;
    if (partie.getJoueurs().size() >= 2) {
        if (partie.getJoueurs()[1]->estIA()) {
            modeSolo = true;
        }
    }
    f << (modeSolo ? 1 : partie.getNbJoueurs()) << endl;

    // 2. ETATS GLOBAUX
    f << partie.getIndexJoueurActuel() << endl;
    f << partie.getIndexPileActuelle() << endl;
    f << partie.getNiveauIllustreConstructeur() << endl;

    // 3. PILES
    const vector<Pile*>& piles = partie.getPiles();
    f << piles.size() << endl;
    for (Pile* p : piles) {
<<<<<<< Updated upstream
        // On utilise getTuiles() défini dans pile.h
=======
>>>>>>> Stashed changes
        const auto& contenu = p->getTuiles();
        f << contenu.size() << endl;
        for (Tuile* t : contenu) {
            f << t->getId() << " " << t->getPrix() << endl;
        }
    }

    // 4. CHANTIER
    const Chantier& chantier = partie.getChantier();
    f << chantier.getNbTuiles() << endl;
    for (size_t i = 0; i < chantier.getNbTuiles(); i++) {
        const Tuile* t = chantier.getTuile(i);
        if (t) f << t->getId() << " " << t->getPrix() << endl;
    }

    // 5. JOUEURS
    for (const Joueur* j : partie.getJoueurs()) {
        if (j) {
            f << j->getNom() << endl;
            f << j->getPierres() << endl;

            // CITE (Historique)
            Cite* c = j->getCite();
            const vector<Action>& historique = c->getHistorique();
            f << historique.size() << endl;
            
            for (const Action& a : historique) {
<<<<<<< Updated upstream
                // On sauvegarde ID, X, Y, Z, Inversion
                f << a.tuileId << " " << a.x << " " << a.y << " " << a.z << " " << a.inversion << endl;
=======
                // Métadonnées de l'action
                f << a.tuileId << " " << a.x << " " << a.y << " " << a.z 
                  << " " << a.inversion << " " << a.rotation << endl;

                // SAUVEGARDE DU CONTENU VISUEL (Type + Etoiles pour les 3 hexagones)
                for(int k=0; k<3; ++k) {
                    f << a.hexas[k].type << " " << a.hexas[k].etoiles << " ";
                }
                f << endl; // Saut de ligne pour lisibilité
>>>>>>> Stashed changes
            }

            // IA (Inventaire)
            if (j->estIA()) {
                const IA* l_ia = dynamic_cast<const IA*>(j);
                if (l_ia) {
                    const auto& sesTuiles = l_ia->getTuilesAcquises();
                    f << sesTuiles.size() << endl;
                    for (const Tuile* t : sesTuiles) {
                        if (t) f << t->getId() << " " << t->getPrix() << endl;
                    }
                }
            } else {
                f << 0 << endl;
            }
        }
    }

    f.close();
    return true;
<<<<<<< Updated upstream
}

// =========================================================
// CHARGER
// =========================================================
bool SaveManager::charger(Partie& partie, const string& nomFichier) {
    ifstream f(nomFichier);
    if (!f) return false;

    // RESET : Remet le jeu à neuf (crée des piles pleines par défaut)
    partie.resetPourChargement(); 

    int codeModeJeu, idxJ, idxP;
    unsigned int niveauIA;

    if (!(f >> codeModeJeu >> idxJ >> idxP >> niveauIA)) return false;

    bool modeSolo = (codeModeJeu == 1);
    int nombreParticipants = modeSolo ? 2 : codeModeJeu;

    partie.setNbJoueurs(nombreParticipants);
    partie.setIndexJoueurActuel(idxJ);
    partie.setIndexPileActuelle(idxP);
    partie.setNiveauIllustreConstructeur(niveauIA);

    // --- CHARGEMENT DES PILES ---
    size_t nbPiles;
    f >> nbPiles;
    const vector<Pile*>& pilesExistantes = partie.getPiles();

    for (size_t i = 0; i < nbPiles; i++) {
        size_t nbTuilesDansPile;
        f >> nbTuilesDansPile;

        if (i < pilesExistantes.size()) {
            Pile* p = pilesExistantes[i];
            
            // IMPORTANT : On vide la pile par défaut
            // Nécessite void vider() { tuiles.clear(); } dans pile.h
            p->vider(); 

            for (size_t k = 0; k < nbTuilesDansPile; k++) {
                unsigned int id, prix;
                f >> id >> prix;
                Tuile* t = Tuile::fabriquer(id, prix);
                p->ajouterTuile(t);
            }
        } else {
            for (size_t k = 0; k < nbTuilesDansPile; k++) { 
                unsigned int d1, d2; f >> d1 >> d2; 
            }
        }
    }

    // --- CHARGEMENT DU CHANTIER ---
    size_t nbTuilesChantier;
    f >> nbTuilesChantier;
    
    // IMPORTANT : On vide le chantier par défaut
    // Nécessite void vider() { tuiles.clear(); } dans chantier.h
    partie.getChantier().vider(); 

    for (size_t i = 0; i < nbTuilesChantier; ++i) {
        unsigned int id, prix;
        f >> id >> prix;
        Tuile* t = Tuile::fabriquer(id, prix);
        partie.getChantier().ajouterTuileSpecifique(t);
    }

    // --- CHARGEMENT DES JOUEURS ---
    for (int i = 0; i < nombreParticipants; i++) {
        string nom;
        f >> nom; 

        Joueur* j = nullptr;
        IA* ptrIA = nullptr;

        if (modeSolo && i == 1) {
            ptrIA = new IA(nom, niveauIA);
            j = ptrIA;
        } else {
            j = new Joueur(nom);
        }
        partie.ajouterJoueur(j);

        int pierresSauvegardees;
        f >> pierresSauvegardees;

        // RECONSTRUCTION CITE
        size_t nbActions;
        f >> nbActions;
        Cite* cite = j->getCite();
        
        for(size_t k=0; k < nbActions; k++) {
            int tid, x, y, z; 
            bool inv;
            f >> tid >> x >> y >> z >> inv;

            Tuile* t = Tuile::fabriquer(tid, 0); 
            // if(inv) t->setInversion(true);
            
            Coord c = {x, y, z};
            
            try {
                cite->placer(t, c, j);
            } catch (...) {
                // Ignore les erreurs de placement (ex: tuile départ)
            }
        }

        // Restauration pierres
        int pierresActuelles = j->getPierres();
        if (pierresActuelles > 0) j->utiliserPierres(pierresActuelles);
        j->ajouterPierres(pierresSauvegardees);

        // Inventaire IA
        size_t nbTuilesIA;
        f >> nbTuilesIA;
        for (size_t k = 0; k < nbTuilesIA; k++) {
            unsigned int id, prix;
            f >> id >> prix;
            if (ptrIA) {
                Tuile* t = Tuile::fabriquer(id, prix);
                ptrIA->ajouterTuile(t); 
            }
        }
    }

    f.close();
    return true;
}

=======
}
// =========================================================
// CHARGER
// =========================================================
bool SaveManager::charger(Partie& partie, const string& nomFichier) {
    ifstream f(nomFichier);
    if (!f) return false;

    // RESET TOTAL
    partie.resetPourChargement(); 

    int codeModeJeu, idxJ, idxP;
    unsigned int niveauIA;

    if (!(f >> codeModeJeu >> idxJ >> idxP >> niveauIA)) return false;

    bool modeSolo = (codeModeJeu == 1);
    int nombreParticipants = modeSolo ? 2 : codeModeJeu;

    partie.setNbJoueurs(nombreParticipants);
    partie.setIndexJoueurActuel(idxJ);
    partie.setIndexPileActuelle(idxP);
    partie.setNiveauIllustreConstructeur(niveauIA);

    // --- CHARGEMENT DES PILES ---
    size_t nbPiles;
    f >> nbPiles;
    const vector<Pile*>& pilesExistantes = partie.getPiles();

    for (size_t i = 0; i < nbPiles; i++) {
        size_t nbTuilesDansPile;
        f >> nbTuilesDansPile;

        if (i < pilesExistantes.size()) {
            Pile* p = pilesExistantes[i];
            p->vider(); 
            for (size_t k = 0; k < nbTuilesDansPile; k++) {
                unsigned int id, prix;
                f >> id >> prix;
                Tuile* t = Tuile::fabriquer(id, prix);
                p->ajouterTuile(t);
            }
        } else {
            for (size_t k = 0; k < nbTuilesDansPile; k++) { 
                unsigned int d1, d2; f >> d1 >> d2; 
            }
        }
    }

    // --- CHARGEMENT DU CHANTIER ---
    size_t nbTuilesChantier;
    f >> nbTuilesChantier;
    partie.getChantier().vider(); 

    for (size_t i = 0; i < nbTuilesChantier; ++i) {
        unsigned int id, prix;
        f >> id >> prix;
        Tuile* t = Tuile::fabriquer(id, prix);
        partie.getChantier().ajouterTuileSpecifique(t);
    }

    // --- CHARGEMENT DES JOUEURS ---
    for (int i = 0; i < nombreParticipants; i++) {
        string nom;
        f >> nom; 

        Joueur* j = nullptr;
        IA* ptrIA = nullptr;

        if (modeSolo && i == 1) {
            ptrIA = new IA(nom, niveauIA);
            j = ptrIA;
        } else {
            j = new Joueur(nom);
        }
        partie.ajouterJoueur(j);

        int pierresSauvegardees;
        f >> pierresSauvegardees;

        // --- RECONSTRUCTION CITE ---
        size_t nbActions;
        f >> nbActions;
        Cite* cite = j->getCite();
        
        for(size_t k = 0; k < nbActions; k++) {
            int tid, x, y, z, rot;
            bool inv;
            
            // 1. Lecture métadonnées
            f >> tid >> x >> y >> z >> inv >> rot;

            // 2. Lecture des détails des hexagones (Type et Etoiles)
            struct HexaInfo { int type; int etoiles; };
            HexaInfo infos[3];
            for(int h=0; h<3; ++h) {
                f >> infos[h].type >> infos[h].etoiles;
            }

            // 3. Création de la tuile (aléatoire pour l'instant)
            Tuile* t = Tuile::fabriquer(tid, 0); 
            
            // 4. APPLICATION DE L'ORIENTATION (CRUCIAL : AVANT LA RECONSTRUCTION)
            // On tourne/inverse la tuile pour que ses indices 0,1,2 correspondent
            // à la position physique sauvegardée.
            if (inv) t->inverser();
            for (int r = 0; r < rot; ++r) {
                t->tourner();
            }

            // 5. RECONSTRUCTION DU CONTENU
            // Maintenant que la tuile est orientée, on injecte les bons types/étoiles
            for(int h=0; h<3; ++h) {
                t->reconstruireHexagone(h, infos[h].type, infos[h].etoiles);
            }
            
            // 6. PLACEMENT
            Coord c = {x, y, z};
            try {
                cite->placer(t, c, j, rot);
            } catch (...) {
                // En cas d'erreur (ex: tuile de départ), on continue
            }
        }

        // Restauration pierres
        int pierresActuelles = j->getPierres();
        if (pierresActuelles > 0) j->utiliserPierres(pierresActuelles);
        j->ajouterPierres(pierresSauvegardees);

        // Inventaire IA
        size_t nbTuilesIA;
        f >> nbTuilesIA;
        for (size_t k = 0; k < nbTuilesIA; k++) {
            unsigned int id, prix;
            f >> id >> prix;
            if (ptrIA) {
                Tuile* t = Tuile::fabriquer(id, prix);
                ptrIA->ajouterTuile(t); 
            }
        }
    }

    f.close();
    return true;
}

>>>>>>> Stashed changes
bool SaveManager::supprimerSauvegarde(const string& nomFichier) {
    if (std::remove(nomFichier.c_str()) == 0) return true;
    return false;
}