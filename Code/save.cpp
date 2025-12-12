#include "save.h"
#include "partie.h"
#include "tuiles.h"
#include "joueur.h"
<<<<<<< Updated upstream
#include "ia.h" // Nécessaire pour new IA()
#include "pile.h"
#include <fstream>
#include <iostream>
#include <string>
=======
#include "ia.h" 
#include "pile.h"
#include "cite.h" 
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio> // Pour std::remove
>>>>>>> Stashed changes

using namespace std;

// =========================================================
<<<<<<< Updated upstream
// FONCTION SAUVEGARDER
=======
// SAUVEGARDER
>>>>>>> Stashed changes
// =========================================================
bool SaveManager::sauvegarder(const Partie& partie, const string& nomFichier) {
    ofstream f(nomFichier);
    if (!f) return false;
<<<<<<< Updated upstream

    // 1. Sauvegarde des variables simples de la Partie
    // On utilise les GETTERS publics
    f << partie.getNbJoueurs() << endl;
    f << partie.getIndexJoueurActuel() << endl;
    f << partie.getIndexPileActuelle() << endl;

    // 2. Sauvegarde du Chantier
    // On récupère le chantier via getChantier()
    const Chantier& chantier = partie.getChantier();
    f << chantier.getNbTuiles() << endl;

    for (size_t i = 0; i < chantier.getNbTuiles(); i++) {
        const Tuile* t = chantier.getTuile(i);
        // On suppose que la classe Tuile a les getters getId() et getPrix()
        // Si vos méthodes s'appellent autrement, adaptez ces deux mots.
        if (t) {
            f << t->getId() << " " << t->getPrix() << endl;
        }
    }

    // 3. Sauvegarde des Joueurs
    // On récupère la liste via getJoueurs()
    const vector<Joueur*>& listeJoueurs = partie.getJoueurs();
    
    for (const Joueur* j : listeJoueurs) {
        if (j) {
            f << j->getNom() << endl;
            // Ici, si le joueur a une méthode interne pour se sauvegarder (sa cité, son score),
            // il faudrait l'appeler, par exemple : j->sauvegarder(f);
            // Pour l'instant, je sauvegarde juste le nom comme vu dans vos erreurs.
=======

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
        // On utilise getTuiles() défini dans pile.h
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
                // On sauvegarde ID, X, Y, Z, Inversion
                f << a.tuileId << " " << a.x << " " << a.y << " " << a.z << " " << a.inversion << endl;
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
>>>>>>> Stashed changes
        }
    }

    f.close();
    return true;
}

// =========================================================
<<<<<<< Updated upstream
// FONCTION CHARGER
=======
// CHARGER
>>>>>>> Stashed changes
// =========================================================
bool SaveManager::charger(Partie& partie, const string& nomFichier) {
    ifstream f(nomFichier);
    if (!f) return false;

<<<<<<< Updated upstream
    // 1. Nettoyage complet de l'ancienne partie
    // On utilise la méthode publique créée spécialement pour respecter SOLID
    partie.resetPourChargement();

    // 2. Lecture des variables simples
    int nbJ, idxJ, idxP;
    if (!(f >> nbJ >> idxJ >> idxP)) return false;

    // On utilise les SETTERS publics
    partie.setNbJoueurs(nbJ);
    partie.setIndexJoueurActuel(idxJ);
    partie.setIndexPileActuelle(idxP);

    // 3. Reconstruction du Chantier
    size_t nbTuilesChantier;
    f >> nbTuilesChantier;
=======
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
>>>>>>> Stashed changes

    for (size_t i = 0; i < nbTuilesChantier; ++i) {
        unsigned int id, prix;
        f >> id >> prix;
<<<<<<< Updated upstream

        // Création de la tuile (Constructeur doit être public dans tuiles.h)
        Tuile* t = Tuile::fabriquer(id, prix);

        // Ajout au chantier via l'accesseur
        partie.getChantier().ajouterTuileSpecifique(t);
    }

    // 4. Reconstruction des Joueurs
    // On boucle selon le nombre de joueurs lu dans le fichier
    for (int i = 0; i < partie.getNbJoueurs(); i++) {
        string nom;
        f >> nom; // Lit le nom sauvegardé

        Joueur* j = nullptr;

        // Logique de détection IA vs Humain basée sur vos erreurs précédentes
        // Si c'est une partie à 2 joueurs et qu'on est au 2ème (index 1), c'est l'IA
        if (partie.getNbJoueurs() == 2 && i == 1) {
            // On utilise le getter pour récupérer le niveau de l'IA
            j = new IA(nom, partie.getNiveauIllustreConstructeur());
        } else {
            // Sinon c'est un joueur humain classique
            j = new Joueur(nom);
        }

        // Si le joueur doit charger sa Cité depuis le fichier, appelez sa méthode ici :
        // j->charger(f);

        // Ajout du joueur dans la partie via la méthode publique
        partie.ajouterJoueur(j);
=======
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
>>>>>>> Stashed changes
    }

    f.close();
    return true;
}

bool SaveManager::supprimerSauvegarde(const string& nomFichier) {
<<<<<<< Updated upstream
    return std::remove(nomFichier.c_str()) == 0;
}


bool SaveManager::sauvegardeExiste(const string& nomFichier) {
    ifstream f(nomFichier);
    return f.good();
=======
    if (std::remove(nomFichier.c_str()) == 0) return true;
    return false;
>>>>>>> Stashed changes
}