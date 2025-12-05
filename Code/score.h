#ifndef SCORE_H
#define SCORE_H

#include <map>
#include <string>
#include <vector>
#include "hexagone.h" 
using namespace std;
// Pas d'include de cite.h ici si on utilise seulement des pointeurs, 
// mais nécessaire si on utilise Cite::Coord dans les méthodes protégées.
// On assume une forward declaration pour Cite.
class Cite;
class Joueur;

// =========================================================
// INTERFACE STRATÉGIE
// =========================================================

class RegleScore {
public:
    virtual ~RegleScore() = default;
    virtual int calculer(Cite* cite) const = 0;
    virtual string getNom() const = 0;

protected:
    int compterEtoiles(Cite* cite, TypeQuartier typeQ) const;
};

// --- HABITATIONS ---
class RegleHabitation : public RegleScore {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Habitation (Standard)"; }
protected:
    int getValeurPlusGrandGroupe(Cite* cite) const;
};

class RegleHabitationVariante : public RegleHabitation {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Habitation (Variante)"; }
};

// --- MARCHÉS ---
class RegleMarche : public RegleScore {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Marche (Standard)"; }
};
class RegleMarcheVariante : public RegleMarche {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Marche (Variante)"; }
};

// --- CASERNES ---
class RegleCaserne : public RegleScore {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Caserne (Standard)"; }
};
class RegleCaserneVariante : public RegleCaserne {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Caserne (Variante)"; }
};

// --- TEMPLES ---
class RegleTemple : public RegleScore {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Temple (Standard)"; }
};
class RegleTempleVariante : public RegleTemple {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Temple (Variante)"; }
};

// --- JARDINS ---
class RegleJardin : public RegleScore {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Jardin (Standard)"; }
};
class RegleJardinVariante : public RegleJardin {
public:
    int calculer(Cite* cite) const override;
    string getNom() const override { return "Jardin (Variante)"; }
};

// =========================================================
// CLASSE SCORE
// =========================================================

class Score {
    friend class Partie;
private:
    Joueur* joueur;
    int total;
    map<TypeQuartier, RegleScore*> strategies;
    map<TypeQuartier, int> pointsParType;

public:
    Score(Joueur* j);
    ~Score();
    void calculerScore();
    int getTotal() const { return total; }
    int getScoreType(TypeQuartier type) const;
};

#endif
