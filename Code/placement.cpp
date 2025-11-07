#ifndef PLACEMENT_AMELIORE_HPP
#define PLACEMENT_AMELIORE_HPP

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "tuiles.h"

// Structure pour identifier une cellule hexagonale en 3D
struct CelluleHex {
    int ligne;
    int colonne;
    int niveau;
    
    CelluleHex(int l = 0, int c = 0, int n = 1) 
        : ligne(l), colonne(c), niveau(n) {}
    
    // Opérateur < pour utiliser dans std::map
    bool operator<(const CelluleHex& other) const {
        if (niveau != other.niveau) return niveau < other.niveau;
        if (ligne != other.ligne) return ligne < other.ligne;
        return colonne < other.colonne;
    }
    
    bool operator==(const CelluleHex& other) const {
        return ligne == other.ligne && 
               colonne == other.colonne && 
               niveau == other.niveau;
    }
};

// Information sur un hexagone placé
struct HexagonePlacé {
    Hexagone hex;          // L'hexagone lui-même
    int idTuile;           // ID de la tuile dont il fait partie
    int indexDansTuile;    // Son index dans la tuile (0, 1 ou 2)
    
    HexagonePlacé() : idTuile(-1), indexDansTuile(-1) {}
    HexagonePlacé(const Hexagone& h, int id, int idx)
        : hex(h), idTuile(id), indexDansTuile(idx) {}
};

// Classe pour gérer le placement avec niveaux
class PlacementAvecNiveaux {
private:
    // Stockage : chaque cellule 3D contient un hexagone placé
    std::map<CelluleHex, HexagonePlacé> grille;
    
    // Pour retrouver rapidement les 3 cellules d'une tuile
    std::map<int, std::vector<CelluleHex>> tuilesPlacees;
    
    int pierres;
    
public:
    PlacementAvecNiveaux() : pierres(0) {}
    
    // ==================== GESTION DES VOISINS ====================
    
    // Obtenir les 6 voisins d'une cellule (même niveau)
    std::vector<CelluleHex> getVoisins(const CelluleHex& cell) const {
        std::vector<CelluleHex> voisins;
        
        // Selon si on est sur une ligne paire ou impaire
        bool lignePaire = (cell.ligne % 2 == 0);
        
        if (lignePaire) {
            // Ligne paire : voisins décalés différemment
            voisins = {
                CelluleHex(cell.ligne - 1, cell.colonne - 1, cell.niveau), // Haut-gauche
                CelluleHex(cell.ligne - 1, cell.colonne, cell.niveau),     // Haut-droite
                CelluleHex(cell.ligne, cell.colonne + 1, cell.niveau),     // Droite
                CelluleHex(cell.ligne + 1, cell.colonne, cell.niveau),     // Bas-droite
                CelluleHex(cell.ligne + 1, cell.colonne - 1, cell.niveau), // Bas-gauche
                CelluleHex(cell.ligne, cell.colonne - 1, cell.niveau)      // Gauche
            };
        } else {
            // Ligne impaire
            voisins = {
                CelluleHex(cell.ligne - 1, cell.colonne, cell.niveau),     // Haut-gauche
                CelluleHex(cell.ligne - 1, cell.colonne + 1, cell.niveau), // Haut-droite
                CelluleHex(cell.ligne, cell.colonne + 1, cell.niveau),     // Droite
                CelluleHex(cell.ligne + 1, cell.colonne + 1, cell.niveau), // Bas-droite
                CelluleHex(cell.ligne + 1, cell.colonne, cell.niveau),     // Bas-gauche
                CelluleHex(cell.ligne, cell.colonne - 1, cell.niveau)      // Gauche
            };
        }
        
        return voisins;
    }
    
    // Compter combien de voisins sont occupés
    int compterVoisinsOccupes(const CelluleHex& cell) const {
        int count = 0;
        for (const auto& voisin : getVoisins(cell)) {
            if (estOccupe(voisin)) {
                count++;
            }
        }
        return count;
    }
    
    // Vérifier si une cellule est adjacente à au moins une autre
    bool aVoisinOccupe(const CelluleHex& cell) const {
        return compterVoisinsOccupes(cell) > 0;
    }
    
    // ==================== VÉRIFICATIONS ====================
    
    bool estOccupe(const CelluleHex& cell) const {
        return grille.find(cell) != grille.end();
    }
    
    bool estLibre(const CelluleHex& cell) const {
        return !estOccupe(cell);
    }
    
    // Vérifier si une cellule a un support en dessous
    bool aSupport(const CelluleHex& cell) const {
        if (cell.niveau == 1) return true; // Sol = support automatique
        
        CelluleHex dessous(cell.ligne, cell.colonne, cell.niveau - 1);
        return estOccupe(dessous);
    }
    
    // Vérifier si un hexagone est en périphérie (< 6 voisins)
    bool estEnPeripherie(const CelluleHex& cell) const {
        if (!estOccupe(cell)) return false;
        return compterVoisinsOccupes(cell) < 6;
    }
    
    // Vérifier si un hexagone est complètement entouré (6 voisins)
    bool estEntoure(const CelluleHex& cell) const {
        if (!estOccupe(cell)) return false;
        return compterVoisinsOccupes(cell) == 6;
    }
    
    // ==================== PLACEMENT ====================
    
    // Vérifier si on peut placer une tuile
    bool peutPlacerTuile(int ligne[3], int colonne[3], int niveau) const {
        // 1. Vérifier que les 3 cellules sont libres
        for (int i = 0; i < 3; i++) {
            CelluleHex cell(ligne[i], colonne[i], niveau);
            if (estOccupe(cell)) {
                std::cout << "Erreur: cellule (" << ligne[i] << "," 
                         << colonne[i] << "," << niveau << ") déjà occupée\n";
                return false;
            }
        }
        
        // 2. Si niveau 1 : au moins une cellule doit être adjacente à la cité
        if (niveau == 1) {
            if (grille.empty()) {
                // Première tuile : OK
                return true;
            }
            
            // Au moins un hexagone doit toucher la cité existante
            bool auMoinsUnAdjacent = false;
            for (int i = 0; i < 3; i++) {
                CelluleHex cell(ligne[i], colonne[i], niveau);
                if (aVoisinOccupe(cell)) {
                    auMoinsUnAdjacent = true;
                    break;
                }
            }
            
            if (!auMoinsUnAdjacent) {
                std::cout << "Erreur: aucun hexagone n'est adjacent à la cité\n";
                return false;
            }
        }
        
        // 3. Si niveau > 1 : chaque cellule doit avoir un support
        if (niveau > 1) {
            for (int i = 0; i < 3; i++) {
                CelluleHex cell(ligne[i], colonne[i], niveau);
                if (!aSupport(cell)) {
                    std::cout << "Erreur: cellule (" << ligne[i] << "," 
                             << colonne[i] << ") n'a pas de support au niveau " 
                             << (niveau-1) << "\n";
                    return false;
                }
            }
        }
        
        // 4. Vérifier que les 3 hexagones forment bien une tuile valide
        // (ils doivent être adjacents entre eux)
        if (!adjacent(ligne[0], ligne[1], colonne[0], colonne[1]) ||
            !adjacent(ligne[0], ligne[2], colonne[0], colonne[2]) ||
            !adjacent(ligne[1], ligne[2], colonne[1], colonne[2])) {
            std::cout << "Erreur: les 3 hexagones ne sont pas adjacents\n";
            return false;
        }
        
        return true;
    }
    
    // Placer une tuile
    bool placerTuile(Tuile& tuile, int ligne[3], int colonne[3], int niveau) {
        if (!peutPlacerTuile(ligne, colonne, niveau)) {
            return false;
        }
        
        int idTuile = tuile.getId();
        int carrieresCouvertes = 0;
        
        // Placer les 3 hexagones
        for (int i = 0; i < 3; i++) {
            CelluleHex cell(ligne[i], colonne[i], niveau);
            
            // Vérifier si on recouvre une carrière
            if (niveau > 1) {
                CelluleHex dessous(ligne[i], colonne[i], niveau - 1);
                auto it = grille.find(dessous);
                if (it != grille.end() && it->second.hex.estCarriere()) {
                    carrieresCouvertes++;
                }
            }
            
            // Placer l'hexagone
            HexagonePlacé hp(tuile.getHexagone(i), idTuile, i);
            grille[cell] = hp;
            
            // Mémoriser la position dans la tuile
            tuilesPlacees[idTuile].push_back(cell);
        }
        
        // Ajouter les pierres
        pierres += carrieresCouvertes;
        
        std::cout << "✓ Tuile #" << idTuile << " placée au niveau " << niveau;
        if (carrieresCouvertes > 0) {
            std::cout << " (+" << carrieresCouvertes << " pierre(s))";
        }
        std::cout << "\n";
        
        return true;
    }
    
    // ==================== ACCÈS AUX DONNÉES ====================
    
    // Obtenir l'hexagone à une position
    const HexagonePlacé* getHexagone(const CelluleHex& cell) const {
        auto it = grille.find(cell);
        return (it != grille.end()) ? &it->second : nullptr;
    }
    
    // Obtenir toutes les positions d'une tuile
    std::vector<CelluleHex> getPositionsTuile(int idTuile) const {
        auto it = tuilesPlacees.find(idTuile);
        return (it != tuilesPlacees.end()) ? it->second : std::vector<CelluleHex>();
    }
    
    // Obtenir tous les hexagones d'un type donné
    std::vector<CelluleHex> getHexagonesParType(QuartierType type) const {
        std::vector<CelluleHex> resultat;
        for (const auto& pair : grille) {
            if (pair.second.hex.getType() == type) {
                resultat.push_back(pair.first);
            }
        }
        return resultat;
    }
    
    int getPierres() const { return pierres; }
    
    // ==================== AFFICHAGE ====================
    
    void afficherNiveau(int niveau) const {
        std::cout << "\n=== Niveau " << niveau << " ===\n";
        
        // Trouver les limites
        int minL = 99, maxL = -99, minC = 99, maxC = -99;
        for (const auto& pair : grille) {
            if (pair.first.niveau == niveau) {
                minL = std::min(minL, pair.first.ligne);
                maxL = std::max(maxL, pair.first.ligne);
                minC = std::min(minC, pair.first.colonne);
                maxC = std::max(maxC, pair.first.colonne);
            }
        }
        
        if (minL > maxL) {
            std::cout << "(vide)\n";
            return;
        }
        
        // Afficher la grille
        for (int l = minL; l <= maxL; l++) {
            // Indentation pour effet hexagonal
            if (l % 2 == 1) std::cout << "  ";
            
            for (int c = minC; c <= maxC; c++) {
                CelluleHex cell(l, c, niveau);
                const HexagonePlacé* hp = getHexagone(cell);
                
                if (hp) {
                    std::cout << hp->hex.toChar() << " ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }
    }
    
    void afficher() const {
        std::cout << "\n========== CITÉ ==========\n";
        std::cout << "Pierres: " << pierres << "\n";
        std::cout << "Tuiles placées: " << tuilesPlacees.size() << "\n";
        
        // Trouver le niveau max
        int maxNiv = 0;
        for (const auto& pair : grille) {
            maxNiv = std::max(maxNiv, pair.first.niveau);
        }
        
        for (int niv = 1; niv <= maxNiv; niv++) {
            afficherNiveau(niv);
        }
        
        std::cout << "==========================\n";
    }
    
private:
    // Fonction helper pour vérifier adjacence (votre code original)
    bool adjacent(int l1, int l2, int c1, int c2) const {
        return (abs(l1 - l2) <= 2 && abs(c1 - c2) <= 1);
    }
};

#endif // PLACEMENT_AMELIORE_HPP
