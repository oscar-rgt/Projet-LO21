#include "save.h"
#include "partie.h"
#include "tuiles.h"
#include "joueur.h"
#include "ia.h" 
#include "pile.h"
#include "cite.h" 
#include <fstream>
#include <iostream>
<<<<<<< Updated upstream
#include <vector>
#include <cstdio> 
=======
#include <string>
#include <vector>
#include <cstdio>
>>>>>>> Stashed changes

using namespace std;

// =========================================================
<<<<<<< Updated upstream
// SAUVEGARDER
// =========================================================
bool SaveManager::sauvegarder(const Partie& partie, const string& nomFichier) {
    cout << "[INFO] Debut sauvegarde..." << endl;
    ofstream f(nomFichier);
    if (!f) return false;

    // 1. CONFIG
    bool modeSolo = false;
    if (partie.getJoueurs().size() >= 2) {
        if (partie.getJoueurs()[1]->estIA()) modeSolo = true;
    }
    f << (modeSolo ? 1 : partie.getNbJoueurs()) << endl;

    // 2. ETATS
=======
// SAUVEGARDER - VERSION COMPLÈTE
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
>>>>>>> Stashed changes
    f << partie.getIndexJoueurActuel() << endl;
    f << partie.getIndexPileActuelle() << endl;
    f << partie.getNiveauIllustreConstructeur() << endl;

<<<<<<< Updated upstream
    // 3. PILES
    const vector<Pile*>& piles = partie.getPiles();
    f << piles.size() << endl;
    for (Pile* p : piles) {
        const auto& contenu = p->getTuiles();
        f << contenu.size() << endl;
        for (Tuile* t : contenu) {
            f << t->getId() << " " << t->getPrix() << endl;
        }
    }

=======
    // ========================================
    // AJOUT : MODE TUILE CITE
    // ========================================
    f << static_cast<int>(partie.getModeTuileCite()) << endl;

    // ========================================
    // AJOUT : VARIANTES (5 booléens)
    // ========================================
    const auto& variantes = partie.getVariantes();
    for (int i = 0; i < 5; i++) {
        f << (variantes[i] ? 1 : 0) << " ";
    }
    f << endl;

    // 3. PILES
    const vector<Pile*>& piles = partie.getPiles();
    f << piles.size() << endl;
    for (Pile* p : piles) {
        const auto& contenu = p->getTuiles();
        f << contenu.size() << endl;
        for (Tuile* t : contenu) {
            f << t->getId() << " " << t->getPrix() << endl;
        }
    }

>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
=======
            // CITE (Historique)
>>>>>>> Stashed changes
            Cite* c = j->getCite();
            const vector<Action>& historique = c->getHistorique();
            f << historique.size() << endl;
            
            for (const Action& a : historique) {
                f << a.tuileId << " " << a.x << " " << a.y << " " << a.z 
                  << " " << a.inversion << " " << a.rotation << endl;
<<<<<<< Updated upstream
                for(int k=0; k<3; ++k) f << a.hexas[k].type << " " << a.hexas[k].etoiles << " ";
                f << endl;
            }
=======

                for(int k=0; k<3; ++k) {
                    f << a.hexas[k].type << " " << a.hexas[k].etoiles << " ";
                }
                f << endl;
            }

            // IA (Inventaire)
>>>>>>> Stashed changes
            if (j->estIA()) {
                const IA* l_ia = dynamic_cast<const IA*>(j);
                if (l_ia) {
                    const auto& sesTuiles = l_ia->getTuilesAcquises();
                    f << sesTuiles.size() << endl;
                    for (const Tuile* t : sesTuiles) {
                        if (t) f << t->getId() << " " << t->getPrix() << endl;
                    }
                }
<<<<<<< Updated upstream
            } else { f << 0 << endl; }
        }
    }
=======
            } else {
                f << 0 << endl;
            }
        }
    }

>>>>>>> Stashed changes
    f.close();
    return true;
}

// =========================================================
<<<<<<< Updated upstream
// CHARGER
// =========================================================
bool SaveManager::charger(Partie& partie, const string& nomFichier) {
    cout << "[INFO] Chargement de " << nomFichier << "..." << endl;
    ifstream f(nomFichier);
    if (!f) return false;

    // 1. Lecture Config
    int codeModeJeu, idxJ, idxP;
    unsigned int niveauIA;
    if (!(f >> codeModeJeu >> idxJ >> idxP >> niveauIA)) return false;

    bool modeSolo = (codeModeJeu == 1);
    int nombreParticipants = modeSolo ? 2 : codeModeJeu;

    // --- RESET TOTAL ---
    partie.setNbJoueurs(nombreParticipants);
    partie.resetPourChargement(); 
    
    partie.setIndexJoueurActuel(idxJ);
    partie.setIndexPileActuelle(idxP);
    partie.setNiveauIllustreConstructeur(niveauIA);

    // 3. PILES
    size_t nbPilesFichier;
    f >> nbPilesFichier; 

    vector<Pile*>& pilesJeu = const_cast<vector<Pile*>&>(partie.getPiles());

    for (size_t i = 0; i < nbPilesFichier; i++) {
        size_t nbTuiles;
        f >> nbTuiles;

        // On recrée la pile
        Pile* p = Pile::creer(i);
        pilesJeu.push_back(p);

        for (size_t k = 0; k < nbTuiles; k++) {
            unsigned int id, prix;
            f >> id >> prix;
            Tuile* t = Tuile::fabriquer(id, prix);
            p->ajouterTuile(t);
        }
    }

    // 4. CHANTIER
    size_t nbTuilesChantier;
    f >> nbTuilesChantier;
    
    // CA MARCHE MAINTENANT CAR VIDER() EST DANS LE .H !
    partie.getChantier().vider(); 
=======
// CHARGER - VERSION COMPLÈTE
// =========================================================
bool SaveManager::charger(Partie& partie, const string& nomFichier) {
    ifstream f(nomFichier);
    if (!f) return false;

    partie.resetPourChargement(); 

    int codeModeJeu, idxJ, idxP;
    unsigned int niveauIA;

    if (!(f >> codeModeJeu >> idxJ >> idxP >> niveauIA)) return false;

    // ========================================
    // CHARGEMENT : MODE TUILE CITE
    // ========================================
    int modeTuileInt;
    f >> modeTuileInt;
    Partie::TuileCite modeTuile = static_cast<Partie::TuileCite>(modeTuileInt);

    // ========================================
    // CHARGEMENT : VARIANTES
    // ========================================
    std::array<bool, 5> variantes;
    for (int i = 0; i < 5; i++) {
        int val;
        f >> val;
        variantes[i] = (val == 1);
    }

    bool modeSolo = (codeModeJeu == 1);
    int nombreParticipants = modeSolo ? 2 : codeModeJeu;

    partie.setIndexJoueurActuel(idxJ);
    partie.setIndexPileActuelle(idxP);
    partie.setNiveauIllustreConstructeur(niveauIA);
    
    // ========================================
    // APPLICATION DES PARAMÈTRES
    // ========================================
    partie.setModeTuileCite(modeTuile);
    partie.setVariantes(variantes);

    // ========================================
    // RECONSTRUCTION MANUELLE DES PILES
    // ========================================
    size_t nbPiles;
    f >> nbPiles;

    for (size_t i = 0; i < nbPiles; i++) {
        size_t nbTuilesDansPile;
        f >> nbTuilesDansPile;

        Pile* nouvellePile = new Pile(i, 1);
        nouvellePile->vider();
        
        for (size_t k = 0; k < nbTuilesDansPile; k++) {
            unsigned int id, prix;
            f >> id >> prix;
            Tuile* t = Tuile::fabriquer(id, prix);
            nouvellePile->ajouterTuile(t);
        }
        
        partie.ajouterPile(nouvellePile);
    }

    // --- CHARGEMENT DU CHANTIER ---
    size_t nbTuilesChantier;
    f >> nbTuilesChantier;
>>>>>>> Stashed changes

    for (size_t i = 0; i < nbTuilesChantier; ++i) {
        unsigned int id, prix;
        f >> id >> prix;
        Tuile* t = Tuile::fabriquer(id, prix);
        partie.getChantier().ajouterTuileSpecifique(t);
    }

<<<<<<< Updated upstream
    // 5. JOUEURS
=======
    // --- CHARGEMENT DES JOUEURS ---
>>>>>>> Stashed changes
    for (int i = 0; i < nombreParticipants; i++) {
        string nom;
        f >> nom; 

        Joueur* j = nullptr;
<<<<<<< Updated upstream
        if (modeSolo && i == 1) {
            j = new IA(nom, niveauIA);
=======
        IA* ptrIA = nullptr;

        if (modeSolo && i == 1) {
            ptrIA = new IA(nom, niveauIA);
            j = ptrIA;
>>>>>>> Stashed changes
        } else {
            j = new Joueur(nom);
        }
        partie.ajouterJoueur(j);

<<<<<<< Updated upstream
        int pierres;
        f >> pierres;
        j->ajouterPierres(pierres); 

        // CITE
=======
        int pierresSauvegardees;
        f >> pierresSauvegardees;

>>>>>>> Stashed changes
        size_t nbActions;
        f >> nbActions;
        Cite* cite = j->getCite();
        
        for(size_t k = 0; k < nbActions; k++) {
            int tid, x, y, z, rot;
            bool inv;
            f >> tid >> x >> y >> z >> inv >> rot;
<<<<<<< Updated upstream
            struct HexaInfo { int type; int etoiles; };
            HexaInfo infos[3];
            for(int h=0; h<3; ++h) f >> infos[h].type >> infos[h].etoiles;

            Tuile* t = Tuile::fabriquer(tid, 0); 
            if (inv) t->inverser();
            for (int r = 0; r < rot; ++r) t->tourner();
            for(int h=0; h<3; ++h) t->reconstruireHexagone(h, infos[h].type, infos[h].etoiles);
            
            try { cite->placer(t, {x, y, z}, j, rot); } catch (...) { delete t; }
        }

        // IA inventaire
        size_t nbTuilesIA;
        f >> nbTuilesIA;
        IA* l_ia = dynamic_cast<IA*>(j);
        for (size_t k = 0; k < nbTuilesIA; k++) {
            unsigned int id, prix;
            f >> id >> prix;
            if (l_ia) l_ia->ajouterTuile(Tuile::fabriquer(id, prix)); 
=======

            struct HexaInfo { int type; int etoiles; };
            HexaInfo infos[3];
            for(int h=0; h<3; ++h) {
                f >> infos[h].type >> infos[h].etoiles;
            }

            Tuile* t = Tuile::fabriquer(tid, 0); 
            
            if (inv) t->inverser();
            for (int r = 0; r < rot; ++r) {
                t->tourner();
            }

            for(int h=0; h<3; ++h) {
                t->reconstruireHexagone(h, infos[h].type, infos[h].etoiles);
            }
            
            Coord c = {x, y, z};
            try {
                cite->placer(t, c, j, rot);
            } catch (...) {}
        }

        int pierresActuelles = j->getPierres();
        if (pierresActuelles > 0) j->utiliserPierres(pierresActuelles);
        j->ajouterPierres(pierresSauvegardees);

        size_t nbTuilesIA;
        f >> nbTuilesIA;
        for (size_t k = 0; k < nbTuilesIA; k++) {
            unsigned int id, prix;
            f >> id >> prix;
            if (ptrIA) {
                Tuile* t = Tuile::fabriquer(id, prix);
                ptrIA->ajouterTuile(t); 
            }
>>>>>>> Stashed changes
        }
    }

    f.close();
<<<<<<< Updated upstream
    cout << "[INFO] Chargement termine." << endl;
=======
>>>>>>> Stashed changes
    return true;
}

bool SaveManager::supprimerSauvegarde(const string& nomFichier) {
    if (std::remove(nomFichier.c_str()) == 0) return true;
    return false;
}