#include "save.h"
#include "partie.h"
#include "tuiles.h"
#include "joueur.h"
#include "ia.h" // Nécessaire pour new IA()
#include "pile.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// =========================================================
// FONCTION SAUVEGARDER
// =========================================================
bool SaveManager::sauvegarder(const Partie& partie, const string& nomFichier) {
    ofstream f(nomFichier);
    if (!f) return false;

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
        }
    }

    f.close();
    return true;
}

// =========================================================
// FONCTION CHARGER
// =========================================================
bool SaveManager::charger(Partie& partie, const string& nomFichier) {
    ifstream f(nomFichier);
    if (!f) return false;

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

    for (size_t i = 0; i < nbTuilesChantier; ++i) {
        unsigned int id, prix;
        f >> id >> prix;

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
    }

    f.close();
    return true;
}

bool SaveManager::supprimerSauvegarde(const string& nomFichier) {
    return std::remove(nomFichier.c_str()) == 0;
}


bool SaveManager::sauvegardeExiste(const string& nomFichier) {
    ifstream f(nomFichier);
    return f.good();
}