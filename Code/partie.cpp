#include "partie.h"

using namespace std;

Partie& Partie::getInstance(int id_, Mode m, Pile* pile, array<bool, 5> variantes)
{
    static bool dejaCree = false;
    static Partie instance(id_, m, pile, variantes);

    // Si quelqu’un essaie de redonner des paramètres → prévenir
    if (dejaCree) {
        cerr << " getInstance() a déjà été appelé. ";
    }

    dejaCree = true;
    return instance;
}

Partie::Partie(int id_, Mode m, Pile* pile, array<bool, 5> variantes_)
    : id(id_), mode(m), nbJoueurs(0), pileTuiles(pile), variantes(variantes_)
{
    joueurs.fill(nullptr);
}
