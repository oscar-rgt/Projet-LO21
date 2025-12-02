#ifndef JOUEUR_H
#define JOUEUR_H
#include <string>



using namespace std;

class Score;
class Cite;

class Joueur {
private:
    Score* score;
    string nom;
    int pierres;
    Cite* cite;

public:
    Score* getScore() const { return score; }
    Cite* getCite() const { return cite; }
    Joueur(const string& n);
    const string& getNom() const { return nom; }
	int getPierres() const { return pierres; }
    void ajouterPierres(int n = 1);
    bool utiliserPierres(int n = 1);
    void ajouterPierresRecouvrement(int nbRecouverts);
    virtual ~Joueur();
};

#endif