#include "save.h"
#include "partie.h"
#include "tuiles.h"
#include "joueur.h"
#include "ia.h" 
#include "pile.h"
#include "cite.h"
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>

using namespace std;

// =========================================================
// SAUVEGARDER - VERSION DÉTERMINISTE
// =========================================================
bool SaveManager::sauvegarder(const Partie& partie, const string& nomFichier) {
    ofstream f(nomFichier);
    if (!f) return false;

    // 1. MODE DE JEU
    // On parcourt les joueurs pour voir si le deuxième est une IA.
    bool modeSolo = false;
    int indexJ = 0;
    for (auto it = partie.debutJoueurs(); it != partie.finJoueurs(); ++it) {
        // Si le joueur à l'index 1 est une IA, c'est une partie Solo
        if (indexJ == 1 && (*it)->estIA()) {
            modeSolo = true;
            break;
        }
        indexJ++;
    }
    f << (modeSolo ? 1 : partie.getNbJoueurs()) << endl;

    // 2. ETATS GLOBAUX
    f << partie.getIndexJoueurActuel() << endl;
    f << partie.getIndexPileActuelle() << endl;
    f << partie.getNiveauIllustreConstructeur() << endl;

    // 3. MODE TUILE CITE
    // On convertit l'enum TuileCite en entier pour l'écriture
    f << static_cast<int>(partie.getModeTuileCite()) << endl;

    // 4. VARIANTES (5 booléens)
    const auto& variantes = partie.getVariantes();
    for (int i = 0; i < 5; i++) {
        f << (variantes[i] ? 1 : 0) << " ";
    }
    f << endl;

    // 5. PILES - SAUVEGARDE DÉTERMINISTE

    // On sauvegarde le contenu exact des piles pour que l'ordre de tirage reste identique.
    f << partie.getNbPiles() << endl;
    //On regarde le nombre de tuiles dans chaque pile
    for (auto it = partie.debutPiles(); it != partie.finPiles(); ++it) {
        Pile* p = *it;
        f << p->getNbTuiles() << endl;
        //On sauvegarde les attributs de chaque tuile, pour une reconstrustion fidele
        for (auto itT = p->begin(); itT != p->end(); ++itT) {
            Tuile* t = *itT;
            f << t->getId() << " " << t->getPrix() << " " << t->getInversion() << " " << endl;
            for (size_t h = 0; h < t->getNbHexagones(); h++) {
                const Hexagone* hexa = t->getHexagone(static_cast<int>(h));
                f << static_cast<int>(hexa->getType()) << " " << hexa->getEtoiles() << " ";
            }
            f << endl;
        }
    }

    // 6. CHANTIER
    const Chantier& chantier = partie.getChantier();
    f << chantier.getNbTuiles() << endl;
    
    for (auto it = chantier.begin(); it != chantier.end(); ++it){
        const Tuile* t = *it;
        if (!t) continue;
        
        // Métadonnées
        f << t->getId() << " " << t->getPrix() << " " << t->getInversion() << endl;
        
        // Hexagones
        for (size_t h = 0; h < t->getNbHexagones(); h++) {
            Hexagone* hexa = t->getHexagone(static_cast<int>(h));
            f << static_cast<int>(hexa->getType()) << " ";
            f << hexa->getEtoiles() << " ";
        }
        f << endl;
    }

    // 7. JOUEURS
    for (auto itJ = partie.debutJoueurs(); itJ != partie.finJoueurs(); ++itJ) {
        const Joueur* j = *itJ;
        if (!j) continue;
        string nom = j->getNom();
        std::replace(nom.begin(), nom.end(), ' ', '_'); //On met des _ pour les espaces, plus facil pour la lectuere de charger()
        f << nom << endl;
        f << j->getPierres() << endl;

        // CITE (Historique)
        // Au lieu de sauvegarder la grille finale, on sauvegarde l'HISTORIQUE des coups joués.(utile pour replacer les niveaux)
        Cite* c = j->getCite();
        const vector<Action>& historique = c->getHistorique();
        f << historique.size() << endl;
        //Utilisation de action qui fait des "photos" des tuiles pour les enregistrer dans la savegarde
        for (const Action& a : historique) {
            f << a.tuileId << " " << a.pos.x << " " << a.pos.y << " " << a.pos.z 
              << " " << a.inversion << " " << a.rotation << endl;

            for(int k=0; k<3; ++k) {
                f << a.hexas[k].type << " " << a.hexas[k].etoiles << " ";
            }
            f << endl;
        }

        // IA 
        // L'IA ne construit pas une cité visuelle, elle a un "inventaire" de tuiles pour le scoring.
        if (j->estIA()) {
            const IA* l_ia = dynamic_cast<const IA*>(j);
            if (l_ia) {
                int nbTuilesReelles = 0;
                for (auto it = l_ia->begin(); it != l_ia->end(); ++it) {
                    Tuile* t = *it; // Déréférencement de l'itérateur
                    if (t && t->getId() != 0 && t->getNbHexagones() == 3) {
                        nbTuilesReelles++;
                    }
                }
                f << nbTuilesReelles << endl;
                for (auto it = l_ia->begin(); it != l_ia->end(); ++it) {
                    Tuile* t = *it;
                    if (!t) continue;

                    // Filtre : On ignore la tuile de départ
                    if (t->getId() == 0 || t->getNbHexagones() != 3) {
                        continue;
                    }

                    // Sauvegarde d'une tuile standard (3 hexagones)
                    f << t->getId() << " " << t->getPrix() << " " << t->getInversion() << endl;
                    
                    // Hexagones
                    for (int h = 0; h < 3; h++) {
                        const Hexagone* hexa = t->getHexagone(h);
                        f << static_cast<int>(hexa->getType()) << " " << hexa->getEtoiles() << " ";
                    }
                    f << endl;
                }
            }
        } else {
            f << 0 << endl;
        }
    }

    f.close();
    return true;
}

// =========================================================
// CHARGER - VERSION DÉTERMINISTE
// =========================================================
bool SaveManager::charger(Partie& partie, const string& nomFichier) {
    ifstream f(nomFichier);
    if (!f) return false;
    //On vide toute la mémoire actuelle (piles, joueurs, chantier) pour éviter de mélanger l'ancienne partie avec la sauvegarde.
    partie.resetPourChargement(); 

    int codeModeJeu, idxJ, idxP;
    unsigned int niveauIA;

    if (!(f >> codeModeJeu >> idxJ >> idxP >> niveauIA)) return false;

    // MODE TUILE CITE
    int modeTuileInt;
    f >> modeTuileInt;
    TuileCite modeTuile = static_cast<TuileCite>(modeTuileInt);

    // VARIANTES
    std::array<bool, 5> variantes;
    for (int i = 0; i < 5; i++) {
        int val;
        f >> val;
        variantes[i] = (val == 1);
    }

    bool modeSolo = (codeModeJeu == 1);
    int nombreParticipants = modeSolo ? 2 : codeModeJeu;
    //On enregitre les infos de la partie a charger
    partie.setIndexJoueurActuel(idxJ);
    partie.setIndexPileActuelle(idxP);
    partie.setNiveauIllustreConstructeur(niveauIA);
    partie.setModeTuileCite(modeTuile);
    partie.setVariantes(variantes);

    // ========================================
    // RECONSTRUCTION DÉTERMINISTE DES PILES
    // ========================================
    size_t nbPiles;
    f >> nbPiles;

    for (size_t i = 0; i < nbPiles; i++) {
        size_t nbTuilesDansPile;
        f >> nbTuilesDansPile;

        // Créer une pile vide
        Pile* nouvellePile = new Pile(static_cast<int>(i), 1);
        nouvellePile->vider();
        
        for (size_t k = 0; k < nbTuilesDansPile; k++) {
            unsigned int id, prix;
            bool inversion;
            
            if (!(f >> id >> prix >> inversion)) {
                delete nouvellePile;
                return false;
            }
            
            // ========================================
            // RECONSTRUCTION : Lire les hexagones
            // ========================================
            struct HexaInfo { int type; int etoiles; };
            HexaInfo hexas[3];
            
            for (int h = 0; h < 3; h++) {
                if (!(f >> hexas[h].type >> hexas[h].etoiles)) {
                    delete nouvellePile;
                    return false;
                }
            }
            
            // Créer une tuile temporaire (avec hexagones aléatoires)
            Tuile* t = Tuile::fabriquer(id, prix);
            
            // Écraser les hexagones aléatoires par les vrais
            for (int h = 0; h < 3; h++) {
                t->reconstruireHexagone(h, hexas[h].type, hexas[h].etoiles);
            }
            
            // Restaurer l'inversion
            if (inversion != t->getInversion()) {
                t->inverser();
            }
            
            nouvellePile->ajouterTuile(t);
        }
        
        partie.ajouterPile(nouvellePile);
    }

    // ========================================
    // CHARGEMENT DU CHANTIER (DÉTERMINISTE)
    // ========================================
    size_t nbTuilesChantier;
    f >> nbTuilesChantier;

    for (size_t i = 0; i < nbTuilesChantier; ++i) {
        unsigned int id, prix;
        bool inversion;
        
        if (!(f >> id >> prix >> inversion)) return false;
        
        // Lire les hexagones
        struct HexaInfo { int type; int etoiles; };
        HexaInfo hexas[3];
        
        for (int h = 0; h < 3; h++) {
            if (!(f >> hexas[h].type >> hexas[h].etoiles)) return false;
        }
        
        // Reconstruire la tuile
        Tuile* t = Tuile::fabriquer(id, prix);
        for (int h = 0; h < 3; h++) {
            t->reconstruireHexagone(h, hexas[h].type, hexas[h].etoiles);
        }
        if (inversion != t->getInversion()) {
            t->inverser();
        }
        
        partie.getChantier().ajouterTuileSpecifique(t);
    }

    // ========================================
    // CHARGEMENT DES JOUEURS
    // ========================================
    for (int i = 0; i < nombreParticipants; i++) {
        string nom;
        f >> nom;

        // --- A. GESTION DU NOM (_ -> Espaces) ---
        std::replace(nom.begin(), nom.end(), '_', ' ');
        // Polymorphisme : Création d'IA ou de Joueur selon le cas
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

        // ========================================
        // INVENTAIRE IA (DÉTERMINISTE)
        // ========================================
        size_t nbTuilesIA;
        f >> nbTuilesIA;
        
        for (size_t k = 0; k < nbTuilesIA; k++) {
            unsigned int id, prix;
            bool inversion;
            
            if (!(f >> id >> prix >> inversion)) return false;

            // Lire les hexagones
            struct HexaInfo { int type; int etoiles; };
            HexaInfo hexas[3];
            
            for (int h = 0; h < 3; h++) {
                if (!(f >> hexas[h].type >> hexas[h].etoiles)) return false;
            }
            //Si c'est l'IA on refait ses tuiles et on les ajoutent à l'inventaire
            if (ptrIA) {
                Tuile* t = Tuile::fabriquer(id, prix);
                for (int h = 0; h < 3; h++) {
                    t->reconstruireHexagone(h, hexas[h].type, hexas[h].etoiles);
                }
                if (inversion != t->getInversion()) {
                    t->inverser();
                }
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
