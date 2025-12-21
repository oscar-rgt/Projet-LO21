#ifndef SCORE_H
#define SCORE_H

#include <map>
#include <string>
#include <vector>
#include "hexagone.h" 

using namespace std;

class Cite;
class Joueur;

// =========================================================
// PATTERN STRATEGY : RÈGLES DE SCORING
// =========================================================
// Chaque type de quartier (Habitation, Marche, etc.) a sa propre classe
// dérivée de RegleScore pour encapsuler l'algorithme de calcul de points.

class RegleScore {
public:
    virtual ~RegleScore() = default;

    // Calcule le score pour un type de quartier spécifique dans la cité donnée
    virtual int calculer(Cite* cite) const = 0;

    // Retourne le nom de la règle (utile pour le debug ou l'affichage, mais inutilisée ici)
    virtual string getNom() const = 0;

protected:
    // Méthode utilitaire accessible par toutes les sous-règles pour compter les étoiles des places
    int compterEtoiles(Cite* cite, TypeQuartier typeQ) const;
};

// --- HABITATIONS (Bleu) ---
// Règle : Points basés sur la taille du plus grand groupe 
class RegleHabitation : public RegleScore {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Habitation (Standard)"; }
protected:
    // Algorithme de recherche du plus grand groupe de quartiers adjacents
    int getValeurPlusGrandGroupe(Cite* cite) const;
};

class RegleHabitationVariante : public RegleHabitation {
public:
    int calculer(Cite* cite) const override; // Variante : Double les points si score >= 10
};

// --- MARCHÉS (Jaune) ---
// Règle : Points si le marché n'est adjacent à aucun autre marché.
class RegleMarche : public RegleScore {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Marche (Standard)"; }
};
class RegleMarcheVariante : public RegleMarche {
public:
    int calculer(Cite* cite) const override; // Variante : Points doublés si adjacent à une place marché
    string getNom() const override { return "Marche (Variante)"; }
};

// --- CASERNES (Rouge) ---
// Règle : Points si la caserne est en périphérie (moins de 6 voisins).
class RegleCaserne : public RegleScore {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Caserne (Standard)"; }
};
class RegleCaserneVariante : public RegleCaserne {
public:
    int calculer(Cite* cite) const override; // Variante : Bonus si 3 ou 4 voisins libres
    string getNom() const override { return "Caserne (Variante)"; }
};

// --- TEMPLES (Violet) ---
// Règle : Points si le temple est complètement entouré (6 voisins).
class RegleTemple : public RegleScore {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Temple (Standard)"; }
};
class RegleTempleVariante : public RegleTemple {
public:
    int calculer(Cite* cite) const override; // Variante : Bonus si en hauteur (z > 0)
    string getNom() const override { return "Temple (Variante)"; }
};

// --- JARDINS (Vert) ---
// Règle : Points sans condition d'adjacence (dépend juste des étoiles).
class RegleJardin : public RegleScore {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Jardin (Standard)"; }
};
class RegleJardinVariante : public RegleJardin {
public:
    int calculer(Cite* cite) const override; // Variante : Bonus si adjacent à un "lac"
    string getNom() const override { return "Jardin (Variante)"; }
};

// =========================================================
// CONTEXTE SCORE
// =========================================================
// Classe principale appelée par le jeu pour calculer le total.
// Elle instancie les bonnes stratégies selon les variantes actives.

class Score {
private:
    Joueur* joueur;
    map<TypeQuartier, RegleScore*> strategies; // Stocke la règle active pour chaque couleur

public:
    Score(Joueur* j);
    ~Score(); 
    int calculerScore() const;
};

#endif