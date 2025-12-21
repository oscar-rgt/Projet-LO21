#ifndef SAVE_H
#define SAVE_H
#include"except.h"

#include <string>
class Partie;

class SaveException : public AkropolisException {
public:
    SaveException(const std::string& i) : AkropolisException(i) {}
};

class SaveManager {
public:
    static bool sauvegarder(const Partie& partie, const std::string& nomFichier);
    static bool charger(Partie& partie, const std::string& nomFichier);
    static bool supprimerSauvegarde(const std::string& nomFichier);
};

#endif // SAVE_H
