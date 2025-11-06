#include <string>

class Joueur {
private:
    std::string nom;
    int points;
    int pierres;

public:
    Joueur(const std::string& n);
    const std::string& getNom() const;
    int getPoints()  const;
    int getPierres() const;
    void setPoints(int p);
    void ajouterPoints(int delta);
    void ajouterPierre(int n=1);
    bool utiliserPierre(int n=1);
    void ajouterPierres(int nbRecouverts);
};
