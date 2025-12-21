#include "jeuConsole.h"
#include "joueur.h"
#include "ia.h"
#include "cite.h"
#include "save.h"
#include "score.h"
#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

void JeuConsole::nettoyerEcran() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int JeuConsole::saisieNombre(const string& prompt, int min, int max) {
    int reponse = -1;
    cout << prompt << " (" << min << " - " << max << ") : ";
    while (!(cin >> reponse) || reponse < min || reponse > max) {
        cout << "Erreur. Entrez un nombre entre " << min << " et " << max << " : ";
        //si l'utilisateur saisie un mauvais nombre on efface le buffer et on redemande
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return reponse;
}

bool JeuConsole::saisieOuiNon(const string& prompt) {
    string reponse;
    cout << prompt << " (o/n) : ";
    while (true) {
        cin >> reponse;
        for (auto& c : reponse) c = tolower(c);
        if (reponse == "o") return true;
        if (reponse == "n") return false;
        cout << "Repondre par 'o' ou 'n' : ";
    }
}

void JeuConsole::afficherChantier() {
    const auto& chantier = Partie::getInstance().getChantier();
    vector<string> lignes(9, "");
    const int hauteur = 9;
    const int largeur_reelle = 20;
    const int largeur_visuelle = 19;

    int pilesRestantes = Partie::getInstance().getNbPiles() - Partie::getInstance().getIndexPileActuelle();
    cout << "\n=== CHANTIER === " << pilesRestantes << " pile(s) restante(s)" << endl;

    //affichage des tuiles du chantier
    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        Tuile* t = *it; 
        string designTuile = t->getDesign();
        for (int j = 0; j < hauteur; j++) {
            string segment = designTuile.substr(j * largeur_reelle, largeur_visuelle);
            lignes[j] += segment;
            lignes[j] += "         ";
        }
    }
    for (int j = 0; j < hauteur; j++) {
        cout << lignes[j] << endl;
    }
    cout << "\n\n";

    //affichage du prix des tuiles
    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        int pierres = (*it)->getPrix();
        cout << "    ";
        cout << pierres << " pierres";
        cout << "               ";
    }
    cout << endl;
}

void JeuConsole::afficherEtatJeu() {
    Joueur* j = Partie::getInstance().getJoueurActuel();
    if (!j) throw AkropolisException("Pointeur Joueur null.");
    cout << "\n##########################################" << endl;
    cout << "TOUR DE : " << j->getNom() << endl;
    cout << "Pierres : " << j->getPierres() << endl;
    cout << "Score : " << j->getScore()->calculerScore() << endl;
    cout << "##########################################" << endl;

    cout << "\n--- CITE ---" << endl;

    if (j->estIA()) {
        cout << "(Cite de l'IA - Gestion virtuelle)" << endl;
    }
    else {
        Cite * c = j->getCite();
        if (!c) throw AkropolisException("Cite non initialisée");

        vue.reset(); 

        //on remplit la cité du joueur avec ses tuiles
        for (auto it = c->begin(); it != c->end(); ++it) {
            vue.remplir(it->first, it->second);
        }

        //on dessine le résultat sur l'écran
        vue.afficher();

        afficherChantier();
    }


}

void JeuConsole::jouerTour() {

    nettoyerEcran();
    afficherEtatJeu();

    Joueur* j = Partie::getInstance().getJoueurActuel();

    if (j->estIA()) {
        cout << "\n\n--- TOUR DE L'IA ---" << endl;
        cout << "\nL'Illustre Constructeur reflechit..." << endl;
        afficherChantier();
        int indexChoisi = Partie::getInstance().jouerTourIA();
        try {
            cout << "\n\nL'IA choisit la tuile " << indexChoisi << endl;
            cout << "Appuyez sur une touche pour continuer...";
            //simule un appui sur entrée puis vide le buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        catch (const PartieException& e) {
            cout << "L'IA a rencontre une erreur : " << e.what() << endl;
        }
        return; //si c'est l'IA on quitte la fonction
    }
    
    //tour du joueur humain
    const auto& chantier = Partie::getInstance().getChantier();

    int maxChoix = chantier.getNbTuiles() - 1;

    cout << "\n--- ACTION ---" << endl;
    
    int index = saisieNombre("Quelle tuile choisir ? (-1 pour Sauvegarder & Quitter)", -1, (int)maxChoix);
   
    //cas de la sauvegarde (choix -1)
    if (index == -1) {
        if (saisieOuiNon("Voulez-vous sauvegarder et quitter la partie ?")) {
            cout << "\nSauvegarde en cours..." << endl;
            if (SaveManager::sauvegarder(Partie::getInstance(), "save.txt")) {
                cout << ">> Partie sauvegardee avec succes dans 'save.txt' !" << endl;
            } else {
                cout << ">> ERREUR CRITIQUE : La sauvegarde a echoue." << endl;
            }
            
            cout << "A bientot !" << endl;
            exit(0);
        } else {
            //Si on annule, on relance le tour
            jouerTour(); 
            return;
        }
    }

    //récupére la tuile choisie
    auto itTuile = chantier.begin();
    for (int k = 0; k < index; ++k) {
        ++itTuile;
    }
    Tuile* tuileAffichee = *itTuile;

    //prévisualisation :
    bool placementValide = false;
    int rotationCompteur = 0;
    bool inversionEtat = false;

    while (!placementValide) {
        nettoyerEcran();
        afficherEtatJeu(); 

        cout << "\n\nCommandes : [R]otation | [I]nversion | [V]alider | [A]nnuler choix" << endl;
        cout << "Votre choix : ";

        string choix;
        cin >> choix;

        if (choix == "R" || choix == "r") {
            tuileAffichee->tourner();
            rotationCompteur = (rotationCompteur + 1) % 3;
        }
        else if (choix == "I" || choix == "i") {
            tuileAffichee->inverser();
        }
        else if (choix == "A" || choix == "a") {

            //si on annule la tuile revient à son état initial
            if (tuileAffichee->getInversion() != 0) tuileAffichee->inverser();
            for (int i = 0; i < (3 - rotationCompteur) % 3; ++i) tuileAffichee->tourner();
            
            jouerTour(); //et on recommence le tour
            return;
        }
        else if (choix == "V" || choix == "v") {
            placementValide = true;
        }
    }

    //choix des coordonnées
    int x = saisieNombre("Coord X", -999, 999);
    int y = saisieNombre("Coord Y", -99, 999);
    int z = saisieNombre("Coord Z (Niveau)", 0, 10);

    cout << "\n\nLa tuile\n\n" << tuileAffichee->getDesign() << "\n\nva etre placee en (" << x << ", " << y << ", " << z << ").\n" << endl;

    if (saisieOuiNon("Valider ce choix ?")) {
        try {
            Partie::getInstance().actionPlacerTuile(index, x, y, z, rotationCompteur);
        }
        catch (const CiteException& e) {
            cout << ">> ECHEC : " << e.what() << endl;
            cout << "Appuyez sur Entree pour reessayer...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            jouerTour();
        }
        catch (const PartieException& e) {
            cout << ">> ECHEC : " << e.what() << endl;
            cout << "Appuyez sur Entree pour reessayer...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            jouerTour();
        }
        catch (const exception& e) {
            cout << ">> ECHEC INATTENDU : " << e.what() << endl;
            cout << "Appuyez sur Entree pour reessayer...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            jouerTour();
        }
    }
    else {
        //si le joueur annule, on remet la tuile choisie dans son état initial et on rejoue le tour
        if (tuileAffichee->getInversion() != 0) tuileAffichee->inverser(); 
        for (int i = 0; i < (3 - rotationCompteur) % 3; ++i) tuileAffichee->tourner(); 
        jouerTour();
    }
}

void JeuConsole::lancer() {

    
    cout << "\n\n\n\n\n\n\n";
    cout << "                                    __       __   ___   _______     ______    _______    ______    ___        __      ________  " << endl;
    cout << "                                   /\"\"\\     |/\"| /  \") /\"      \\   /    \" \\  |   __ \"\\  /    \" \\  |\"  |      |\" \\    /\"       ) " << endl;
    cout << "                                  /    \\    (: |/   / |:        | // ____  \\ (. |__) :)// ____  \\ ||  |      ||  |  (:   \\___/  " << endl;
    cout << "                                 /' /\\  \\   |    __/  |_____/   )/  /    ) :)|:  ____//  /    ) :)|:  |      |:  |   \\___  \\    " << endl;
    cout << "                                //  __'  \\  (// _  \\   //      /(: (____/ // (|  /   (: (____/ //  \\  |___   |.  |    __/  \\\\   " << endl;
    cout << "                               /   /  \\\\  \\ |: | \\  \\ |:  __   \\ \\        / /|__/ \\   \\        /  ( \\_|:  \\  /\\  |\\  /\" \\   :)  " << endl;
    cout << "                              (___/    \\___)(__|  \\__)|__|  \\___) \\\"_____/ (_______)   \\\"_____/    \\_______)(__\\_|_)(_______/   " << endl;
    cout << "\n\n";




    cout << "                                                  ===========================================================" << endl;
    cout << "                                                             B I E N V E N U E   D A N S   L A               " << endl;
    cout << "                                                                C I T E   D E S   D I E U X                  " << endl;
    cout << "                                                  ===========================================================" << endl;
    cout << "\n\n";
    cout << "                                                            >>> APPUYEZ SUR ENTREE POUR COMMENCER <<<        " << endl;
    cout << "\n\n\n\n\n";



    cout << "-----------------------------------------------------------" << endl;
    cout << " LO21 - Programmation C++ | Semestre Automne 2025          " << endl;
    cout << " Oscar.R, Louane.R, Valentin.R, Noemie.M, Jeanne.D     " << endl;
    cout << "-----------------------------------------------------------" << endl;

    //nettoyage ecran et lancement jeu
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    nettoyerEcran();

    unsigned int choixMenu;
    bool partieChargeeSucces = false;

    //boucle menu
    do {
        nettoyerEcran();
        cout << "1. JOUER UNE PARTIE" << endl;
        cout << "2. CHARGER UNE PARTIE" << endl;
        cout << "3. REGLES DU JEU" << endl;
        cout << "4. QUITTER" << endl;
        cout << "\n";
        
        choixMenu = saisieNombre("Choisissez une option", 1, 4);

        switch (choixMenu)
        {
        case 1:
            //si le joueur choisi de jouer une partie, on sort de la boucle
            break;

        case 2: {
            cout << "\nChargement..." << endl;
            if (SaveManager::charger(Partie::getInstance(), "save.txt")) {
                cout << ">> Succes !" << endl;
                partieChargeeSucces = true; 
                choixMenu = 1; //on force la sortie du menu
                
                cout << "Appuyez sur Entree pour reprendre...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            } else {
                cerr << ">> Echec du chargement." << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
            break;
        }

        case 3:
            afficherRegles();
            break;

        case 4:
            exit(0);

        default:
            break;
        }
    } while (choixMenu != 1);


    nettoyerEcran();

    // C'EST ICI LA CORRECTION PRINCIPALE :
    if (partieChargeeSucces) {
        cout << ">> Reprise de la partie sauvegardee..." << endl;
        cout << "Appuyez sur Entree...";
    } 
    else {
        //si la partie n'a pas été chargée, on demande la configuration
        demanderConfiguration();
    }

    //boucle de jeu
    while (!Partie::getInstance().estFinDePartie()) {
        jouerTour();
    }
    SaveManager::supprimerSauvegarde("save.txt");;

    nettoyerEcran();
    cout << "=== FIN DE PARTIE ===" << endl;

    cout << "\n--- SCORES ---" << endl;
	int s = 0;

    for (auto it = Partie::getInstance().debutJoueurs(); it != Partie::getInstance().finJoueurs(); ++it) {
        Joueur* j = *it;
        if (!j) throw AkropolisException("Joueur non initialisé");

        Score* score = j->getScore();
        if (!score) throw AkropolisException("Score non initialisé");
        s = j->getScore()->calculerScore();

        cout << j->getNom() << " : " << s << " points (" << j->getPierres() << " pierres)" << endl;
    }

    //determination du vainqueur
    vector<Joueur*> gagnants = Partie::getInstance().determinerGagnants();

    if (gagnants.empty()) {
        cout << "\nErreur : Aucun vainqueur determine." << endl;
    } 
    else if (gagnants.size() == 1) {
        cout << "\n##########################################" << endl;
        cout << "   LE VAINQUEUR EST : " << gagnants[0]->getNom() << " !" << endl;
        cout << "##########################################" << endl;
    } 
    else {
        cout << "\n##########################################" << endl;
        cout << "   EGALITE ENTRE :" << endl;
        for (auto* g : gagnants) {
            cout << "   - " << g->getNom() << endl;
        }
        cout << "##########################################" << endl;
    }

    cout << "\n\nAppuyez sur Entree pour quitter.";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    exit(0);
}

void JeuConsole::demanderConfiguration() {
    cout << "\n--- CONFIGURATION DE LA PARTIE ---" << endl;
    int nbJoueurs = saisieNombre("Combien de joueurs ?", 1, 4);
    nomsJoueurs.clear();
    for (int i = 0; i < nbJoueurs; ++i) {
        string nom;
        cout << "Nom du joueur " << i + 1 << " : ";
        cin >> nom;
        nomsJoueurs.push_back(nom);
    }
    int niveauIA = 0;
    if (nbJoueurs == 1) {
        cout << "Mode solo active" << endl;
        niveauIA = saisieNombre("Niveau Illustre Constructeur", 1, 3);
    }

    bool modeTuileCite = saisieOuiNon("Mode tuile cite augmente ?");
    TuileCite mode = modeTuileCite ? TuileCite::AUGMENTE : TuileCite::STANDARD;

    if (saisieOuiNon("Activer les variantes ?")) {
        cout << "Selection des variantes :\n" << endl;
        variantesActives[0] = saisieOuiNon("Variante habitations active ?");
        variantesActives[1] = saisieOuiNon("Variante marches active ?");
        variantesActives[2] = saisieOuiNon("Variante casernes active ?");
        variantesActives[3] = saisieOuiNon("Variante temples active ?");
        variantesActives[4] = saisieOuiNon("Variante jardins active ?");
    }


    Partie::getInstance().initialiser(nbJoueurs, nomsJoueurs, mode, variantesActives, niveauIA);
}

void JeuConsole::afficherRegles() {
    nettoyerEcran();
    cout << "===========================================================" << endl;
    cout << "                  REGLES D'AKROPOLIS                       " << endl;
    cout << "===========================================================" << endl;
    cout << "1. LE BUT :\n Construisez la cite la plus prestigieuse." << endl;
    cout << "   Les points sont marques grace aux Quartiers" << endl;
    cout << "   multiplies par les Places correspondantes." << endl;
    cout << "\n";
    cout << "2. LES QUARTIERS :" << endl;
    cout << "   - Habitation (H) : 1 point pour chaque quartier habitation adjacent a un autre." << endl;
    cout << "   (Ne compte que pour votre plus grand groupe d'habitations)" << endl;
    cout << "   - Marche (M)    : 1 point si votre marche n'est adjacent a aucun autre." << endl;
    cout << "   - Caserne (C)   : 1 point si votre caserne n'est pas completement entouree par d'autres hexagones." << endl;
    cout << "   - Temple (T)   : 1 point si votre temple est completement entoure par d'autres hexagones." << endl;
    cout << "   - Jardin (J)     : 1 point pour chaque jardin pose sans condition." << endl;
	cout << "   - Carriere (X)  : Permet d'agrandir votre cite mais ne donne pas de points." << endl;
    cout << "\n";
    cout << "3. LES PLACES :" << endl;
	cout << "   Les places de chaque type vous permettent de multiplier vos points" << endl;
	cout << "   en fonction du chiffre qui est ecrit dessus." << endl;
    cout << "   Par exemple, un hexagone 2H est une place Habitation a 2 etoiles." << endl;
    cout << "   Elle multiplie donc par 2 les points gagnes par vos quartiers Habitation." << endl;
    cout << "   /!\\ ATTENTION : Si vous n'avez aucune place d'un certain type," << endl;
	cout << "       vous ne marquez aucun point pour ses quartiers correspondants." << endl;
    cout << "\n";
    cout << "4. LA PIERRE :" << endl;
    cout << "   Vous commencez avec un nombre de 2 pierres." << endl;
    cout << "   Ces dernieres vous permettront d'acheter des tuiles." << endl;
	cout << "   Les pierres influent aussi sur votre score." << endl;
    cout << "   En effet, chaque pierre vous rapporte un point." << endl;
    cout << "   De plus, en cas d'egalite en fin de partie," << endl;
	cout << "   le joueur avec le plus de pierres l'emporte." << endl;
    cout << "   Les pierres s'obtiennent en construisant au dessus d'une carriere." << endl;
    cout << "   Chaque carriere recouverte donne une pierre." << endl;
    cout << "\n";
	cout << "5. PLACEMENT :" << endl;
	cout << "   Vous l'aurez compris, votre cite peut s'etendre aussi bien en surface qu'en hauteur." << endl;
    cout << "   Lorsqu'un hexagone est place en hauteur, son nombre de points est multiplie par son niveau d'elevation." << endl;
	cout << "   Par exemple, un quartier Jardin place au niveau 3 rapporte 3 points." << endl;
    cout << "   A vous de trouver le bon equilibre pour devenir le plus prestigieux des architectes !" << endl;
    cout << "===========================================================" << endl;
    cout << "Appuyez sur Entree pour revenir au menu.";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}