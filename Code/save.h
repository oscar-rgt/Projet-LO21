#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include <string>
#include "partie.h"

class Sauvegarde {
public:
    static bool savePartie(const string& nameFich = "akropolis_save.txt");
    
    static bool chargerPartie(const string& nameFich = "akropolis_save.txt");

    static bool saveTrue(const string& nameFich = "akropolis_save.txt");

    static bool delSave(const string& nameFich = "akropolis_save.txt");
};

#endif SAUVEGARDE_H
