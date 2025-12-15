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
#include <cstdio>

using namespace std;

// =========================================================
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
    f << partie.getIndexJoueurActuel() << endl;
    f << partie.getIndexPileActuelle() << endl;
    f << partie.getNiveauIllustreConstructeur() << endl;

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
                f << a.tuileId << " " << a.x << " " << a.y << " " << a.z 
                  << " " << a.inversion << " " << a.rotation << endl;

                for(int k=0; k<3; ++k) {
                    f << a.hexas[k].type << " " << a.hexas[k].etoiles << " ";
                }
                f << endl;
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
}

// =========================================================
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

        size_t nbActions;
        f >> nbActions;
        Cite* cite = j->getCite();
        
        for(size_t k = 0; k < nbActions; k++) {
            int tid, x, y, z, rot;
            bool inv;
            f >> tid >> x >> y >> z >> inv >> rot;

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
        }
    }

    f.close();
    return true;
}

bool SaveManager::supprimerSauvegarde(const string& nomFichier) {
    if (std::remove(nomFichier.c_str()) == 0) return true;
    return false;
}