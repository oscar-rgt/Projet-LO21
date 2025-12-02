#include "sauvegarde.h"
#include <fstream>
#include <iostream>

using namespace std;

bool Save::savePartie(const string& nameFich) {
    ofstream fichier(nomFichier);
    
    if (!fichier.is_open()) {
        cout << "Impossible de créer le fichier de sauvegarde" << endl;
        return false;
    }
    
    try {
        Partie& partie = Partie::getInstance();

        fichier << "# Sauvegarde partie Akropolis" << endl;
        fichier << endl;

        fichier << "Info sur la Partie" << endl;
        fichier << "nb_joueurs=" << partie.getNbJoueurs() << endl;
        fichier << "index_joueur_actuel=" << partie.getJoueurActuel()->getNom() << endl;
        fichier << "index_pile_actuelle=" << partie.getIndexPileActuelle() << endl;
        fichier << "nb_piles_total=" << partie.getNbPiles() << endl;
        fichier << endl;
        
  
        for (int i = 0; i < partie.getNbJoueurs(); i++) {
            Joueur* j = partie.getJoueur(i);
            
            fichier << "Info sur Joueur" << i << endl;
            fichier << "nom=" << j->getNom() << endl;
            fichier << "points=" << j->getPoints() << endl;
            fichier << "pierres=" << j->getPierres() << endl;
            

            const auto& carte = j->getCite()->getCarte();
            fichier << "nb_hexagones=" << carte.size() << endl;
            
            // Chaque hexagone sur une ligne
            for (const auto& paire : carte) {
                fichier << "hexa="
                       << paire.first.x << ","
                       << paire.first.y << ","
                       << paire.first.z << ","
                       << static_cast<int>(paire.second->getType()) << ","
                       << paire.second->getNiveau() << ","
                       << paire.second->getEtoiles()
                       << endl;
            }
            fichier << endl;
        }
        
        // === 3. SAUVEGARDER LE CHANTIER ===
        fichier << "[CHANTIER]" << endl;
        fichier << "nb_tuiles=" << partie.getChantier().getNbTuiles() << endl;
        
        for (size_t i = 0; i < partie.getChantier().getNbTuiles(); i++) {
            Tuile* t = partie.getChantier().getTuile(i);
            fichier << "tuile=" << t->getId() << "," << t->getPrix() << endl;
        }
        fichier << endl;
        
        fichier.close();
        cout << "Partie sauvegardée << endl;
        return true;
        
    } catch (const exception& e) {
        cout << "Erreur lors de la sauvegarde : " << e.what() << endl;
        fichier.close();
        return false;
    }
}

bool Sauvegarde::chargerPartie(const string& nomFichier) {
    ifstream fichier(nomFichier);
    
    if (!fichier.is_open()) {
        cout << "❌ Aucune sauvegarde trouvée" << endl;
        return false;
    }
    
    try {
        string ligne;
        

        int nbJoueurs = 0;
        string nomJoueurActuel;
        int indexPileActuelle = 0;
        
        // === LECTURE DU FICHIER ===
        while (getline(fichier, ligne)) {
            // Ignorer les commentaires et lignes vides
            if (ligne.empty() || ligne[0] == '#') continue;
            
            // SECTION GENERAL
            if (ligne == "[GENERAL]") {
                while (getline(fichier, ligne) && !ligne.empty() && ligne[0] != '[') {
                    size_t pos = ligne.find('=');
                    if (pos != string::npos) {
                        string cle = ligne.substr(0, pos);
                        string valeur = ligne.substr(pos + 1);
                        
                        if (cle == "nb_joueurs") {
                            nbJoueurs = stoi(valeur);
                        } else if (cle == "index_joueur_actuel") {
                            nomJoueurActuel = valeur;
                        } else if (cle == "index_pile_actuelle") {
                            indexPileActuelle = stoi(valeur);
                        }
                    }
                }
            }
            
            // SECTION JOUEUR
            if (ligne.find("[JOUEUR_") != string::npos) {
                string nom;
                int points = 0, pierres = 0;
                
                while (getline(fichier, ligne) && !ligne.empty() && ligne[0] != '[') {
                    size_t pos = ligne.find('=');
                    if (pos != string::npos) {
                        string cle = ligne.substr(0, pos);
                        string valeur = ligne.substr(pos + 1);
                        
                        if (cle == "nom") {
                            nom = valeur;
                        } else if (cle == "points") {
                            points = stoi(valeur);
                        } else if (cle == "pierres") {
                            pierres = stoi(valeur);
                        } else if (cle == "hexa") {
                            // Format: x,y,z,type,niveau,etoiles
                            // Pour l'instant on skip la reconstruction de la cité
                            // (nécessiterait d'ajouter des méthodes dans Cite)
                        }
                    }
                }
                
                // NOTE : Ici tu devrais recréer le joueur avec ses données
                // Pour l'instant, on affiche juste ce qu'on a lu
                cout << "  - Joueur trouvé : " << nom 
                     << " (Points: " << points 
                     << ", Pierres: " << pierres << ")" << endl;
            }
            
            // SECTION CHANTIER
            if (ligne == "[CHANTIER]") {
                while (getline(fichier, ligne) && !ligne.empty() && ligne[0] != '[') {
                    // Lecture des tuiles du chantier
                }
            }
        }
        
        fichier.close();
        
        cout << "\n✓ Partie chargée depuis '" << nomFichier << "'" << endl;
        cout << "  Nombre de joueurs : " << nbJoueurs << endl;
        cout << "  Joueur actuel : " << nomJoueurActuel << endl;
        
        return true;
        
    } catch (const exception& e) {
        cout << "❌ Erreur lors du chargement : " << e.what() << endl;
        fichier.close();
        return false;
    }
}

bool Sauvegarde::sauvegardeExiste(const string& nomFichier) {
    ifstream fichier(nomFichier);
    bool existe = fichier.good();
    fichier.close();
    return existe;
}

bool Sauvegarde::supprimerSauvegarde(const string& nomFichier) {
    if (remove(nomFichier.c_str()) == 0) {
        cout << "✓ Sauvegarde supprimée" << endl;
        return true;
    }
    return false;
}
