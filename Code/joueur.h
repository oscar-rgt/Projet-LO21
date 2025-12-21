#ifndef JOUEUR_H
#define JOUEUR_H
#include <string>



using namespace std;

//forward declarations pour éviter les inclusions circulaires
class Score;
class Cite;

class Joueur {
private:
    Score* score;
    string nom;
    unsigned int pierres;
    Cite* cite;

public:
    Joueur(const string& n, unsigned int p = 2);
    virtual ~Joueur();

    Score* getScore() const { return score; }
    Cite* getCite() const { return cite; }
    const string& getNom() const { return nom; }
    int getPierres() const { return pierres; }

    void ajouterPierres(unsigned int n = 0);
    bool utiliserPierres(unsigned int n = 1);
    
    virtual bool estIA() const { return false; }
};

#endif