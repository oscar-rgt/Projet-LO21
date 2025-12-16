#ifndef SAVE_H
#define SAVE_H

#include <string>
class Partie;

class SaveManager {
public:
    static bool sauvegarder(const Partie& partie, const std::string& nomFichier);
    static bool charger(Partie& partie, const std::string& nomFichier);
    static bool supprimerSauvegarde(const std::string& nomFichier);
    static bool sauvegardeExiste(const std::string& nomFichier);
};

#endif // SAVE_H