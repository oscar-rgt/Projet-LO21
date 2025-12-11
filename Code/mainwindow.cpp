#include "mainwindow.h"
#include "tuileitem.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGraphicsPolygonItem>
#include <cmath>
#include <QTextEdit>
#include<QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), indexTuileSelectionnee(-1)
{
    setWindowTitle("Akropolis - Qt");
    resize(1024, 768);

    // StackedWidget pour gérer les différentes pages
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Initialisation des pages
    initialiserPageMenuPrincipal();
    initialiserPageRegles();
    initialiserPageJeu();

    // Affichage du menu principal par défaut
    stackedWidget->setCurrentWidget(pageMenuPrincipal);
}

MainWindow::~MainWindow() {}

void MainWindow::initialiserPageMenuPrincipal()
{
    pageMenuPrincipal = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(pageMenuPrincipal);

    QLabel *titleLabel = new QLabel("AKROPOLIS", pageMenuPrincipal);
    titleLabel->setStyleSheet("font-size: 36px; font-weight: bold; margin-bottom: 30px;");
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);

    // Ascii Art pour le titre
    QLabel *asciiArt = new QLabel(
        "                __       __   ___   _______     ______    _______    ______    ___        __      ________  \n"
        "               /\"\"\\     |/\"| /  \") /\"      \\   /    \" \\  |   __ \"\\  /    \" \\  |\"  |      |\" \\    /\"       ) \n"
        "              /    \\    (: |/   / |:        | // ____  \\ (. |__) :)// ____  \\ ||  |      ||  |  (:   \\___/  \n"
        "             /' /\\  \\   |    __/  |_____/   )/  /    ) :)|:  ____//  /    ) :)|:  |      |:  |   \\___  \\    \n"
        "            //  __'  \\  (// _  \\   //      /(: (____/ // (|  /   (: (____/ //  \\  |___   |.  |    __/  \\\\   \n"
        "           /   /  \\\\  \\ |: | \\  \\ |:  __   \\ \\        / /|__/ \\   \\        /  ( \\_|:  \\  /\\  |\\  /\" \\   :)  \n"
        "          (___/    \\___)(__|  \\__)|__|  \\___) \\\"_____/ (_______)   \\\"_____/    \\_______)(__\\_|_)(_______/   \n"
        "\n\n"
        "                              ===========================================================\n"
        "                                         B I E N V E N U E   D A N S   L A               \n"
        "                                            C I T E   D E S   D I E U X                  \n"
        "                              ===========================================================\n",
        pageMenuPrincipal
        );
    asciiArt->setStyleSheet("font-family: Courier; font-size: 14px;");
    layout->addWidget(asciiArt, 0, Qt::AlignCenter);

    QPushButton *btnJouer = new QPushButton("JOUER UNE PARTIE", pageMenuPrincipal);
    QPushButton *btnRegles = new QPushButton("REGLES DU JEU", pageMenuPrincipal);
    QPushButton *btnQuitter = new QPushButton("QUITTER", pageMenuPrincipal);

    btnJouer->setFixedSize(200, 50);
    btnRegles->setFixedSize(200, 50);
    btnQuitter->setFixedSize(200, 50);

    layout->addWidget(btnJouer, 0, Qt::AlignCenter);
    layout->addWidget(btnRegles, 0, Qt::AlignCenter);
    layout->addWidget(btnQuitter, 0, Qt::AlignCenter);

    connect(btnJouer, &QPushButton::clicked, this, &MainWindow::afficherMenuJeu);
    connect(btnRegles, &QPushButton::clicked, this, &MainWindow::afficherMenuRegles);
    connect(btnQuitter, &QPushButton::clicked, this, &MainWindow::quitterJeu);

    QLabel *credits = new QLabel(
        "-----------------------------------------------------------\n"
        " LO21 - Programmation C++ | Semestre Automne 2025          \n"
        " Oscar.R, Louane.R, Valentin.R, Noemie.M, Jeanne.D     \n"
        "-----------------------------------------------------------",
        pageMenuPrincipal
        );
    credits->setStyleSheet("font-size: 12px; margin-top: 20px;");
    layout->addWidget(credits, 0, Qt::AlignCenter);

    stackedWidget->addWidget(pageMenuPrincipal);
}

void MainWindow::initialiserPageRegles()
{
    pageRegles = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(pageRegles);

    QLabel *titleLabel = new QLabel("RÈGLES D'AKROPOLIS", pageRegles);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 20px;");
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);

    QTextEdit *textRegles = new QTextEdit(pageRegles);
    textRegles->setReadOnly(true);
    textRegles->setPlainText(
        "1. LE BUT :\n"
        "Construisez la cité la plus prestigieuse. Les points sont marqués grâce aux Quartiers multipliés par les Places correspondantes.\n\n"
        "2. LES QUARTIERS :\n"
        "- Habitation (H) : 1 point pour chaque quartier habitation adjacent à un autre.\n"
        "(Ne compte que pour votre plus grand groupe d'habitations)\n"
        "- Marche (M) : 1 point si votre marché n'est adjacent à aucun autre.\n"
        "- Caserne (C) : 1 point si votre caserne n'est pas complètement entourée par d'autres hexagones.\n"
        "- Temple (T) : 1 point si votre temple est complètement entouré par d'autres hexagones.\n"
        "- Jardin (J) : 1 point pour chaque jardin posé sans condition.\n"
        "- Carrière (X) : Permet d'agrandir votre cité mais ne donne pas de points.\n\n"
        "3. LES PLACES :\n"
        "Les places de chaque type vous permettent de multiplier vos points en fonction du chiffre qui est écrit dessus.\n"
        "Par exemple, un hexagone 2H est une place Habitation à 2 étoiles. Elle multiplie donc par 2 les points gagnés par vos quartiers Habitation.\n"
        "/!\\ ATTENTION : Si vous n'avez aucune place d'un certain type, vous ne marquez aucun point pour ses quartiers correspondants.\n\n"
        "4. LA PIERRE :\n"
        "Vous commencez avec un nombre de 2 pierres. Ces dernières vous permettront d'acheter des tuiles.\n"
        "Les pierres influent aussi sur votre score. En effet, chaque pierre vous rapporte un point.\n"
        "De plus, en cas d'égalité en fin de partie, le joueur avec le plus de pierres l'emporte.\n"
        "Les pierres s'obtiennent en construisant au-dessus d'une carrière. Chaque carrière recouverte donne une pierre.\n\n"
        "5. PLACEMENT :\n"
        "Votre cité peut s'étendre aussi bien en surface qu'en hauteur. Lorsqu'un hexagone est placé en hauteur, son nombre de points est multiplié par son niveau d'élévation.\n"
        "Par exemple, un quartier Jardin placé au niveau 3 rapporte 3 points.\n"
        "À vous de trouver le bon équilibre pour devenir le plus prestigieux des architectes !"
        );
    layout->addWidget(textRegles);

    QPushButton *btnRetour = new QPushButton("RETOUR", pageRegles);
    btnRetour->setFixedSize(150, 40);
    layout->addWidget(btnRetour, 0, Qt::AlignCenter);

    connect(btnRetour, &QPushButton::clicked, [this]() { stackedWidget->setCurrentWidget(pageMenuPrincipal); });

    stackedWidget->addWidget(pageRegles);
}

void MainWindow::initialiserPageJeu()
{
    pageJeu = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(pageJeu);

    // 2. Zone gauche : La Cité (Visuel)
    sceneCite = new QGraphicsScene(this);
    viewCite = new QGraphicsView(sceneCite);
    viewCite->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(viewCite, 2);

    // 3. Zone droite : Contrôles et Chantier
    QVBoxLayout *sideLayout = new QVBoxLayout();
    labelInfoJoueur = new QLabel("En attente...", pageJeu);
    sideLayout->addWidget(labelInfoJoueur);

    btnRotation = new QPushButton("Pivoter Tuile", pageJeu);
    connect(btnRotation, &QPushButton::clicked, this, &MainWindow::onRotationClicked);
    sideLayout->addWidget(btnRotation);

    btnValidation = new QPushButton("Valider Placement", pageJeu);
    connect(btnValidation, &QPushButton::clicked, this, &MainWindow::onValidationClicked);
    sideLayout->addWidget(btnValidation);

    // ... Zone droite ...
    sideLayout->addWidget(new QLabel("--- CHANTIER ---", pageJeu));

    // Création de la scène et de la vue pour le chantier
    sceneChantier = new QGraphicsScene(this);
    viewChantier = new QGraphicsView(sceneChantier);
    viewChantier->setFixedWidth(200); // Largeur fixe pour la colonne

    // On ajoute la VUE directement au layout (plus besoin de layoutChantier)
    sideLayout->addWidget(viewChantier);
    viewChantier->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // On ajoute le layout latéral au layout principal
    mainLayout->addLayout(sideLayout, 1); // Prend 1/3 de la largeur

    stackedWidget->addWidget(pageJeu);
}

void MainWindow::afficherMenuJeu()
{
    stackedWidget->setCurrentWidget(pageJeu);
    demarrerPartie();
}

void MainWindow::afficherMenuRegles()
{
    stackedWidget->setCurrentWidget(pageRegles);
}

void MainWindow::quitterJeu()
{
    QApplication::quit();
}

void MainWindow::demarrerPartie()
{
    // Configuration hardcodée pour le test graphique
    std::vector<std::string> noms = {"Joueur 1", "Joueur 2"};
    std::array<bool, 5> variantes = {false, false, false, false, false};
    Partie::getInstance().initialiser(2, noms, Partie::TuileCite::STANDARD, variantes, 0);
    mettreAJourInterface();
}

void MainWindow::mettreAJourInterface()
{
    Joueur* j = Partie::getInstance().getJoueurActuel();
    labelInfoJoueur->setText(
        "Tour de : " + QString::fromStdString(j->getNom()) +
        "\nPierres : " + QString::number(j->getPierres()) +
        "\nScore : " + QString::number(j->getScore()->getTotal())
        );
    dessinerCite(j);
    dessinerChantier();
}

void MainWindow::dessinerCite(Joueur* joueur)
{
    sceneCite->clear();
    Cite* cite = joueur->getCite();
    double taille = 30.0;

    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Coord pos = it->first;
        Hexagone* hex = it->second;

        double w = 1.53 * taille;
        double h = (sqrt(3.) * taille);
        double pixelX = pos.x * w;
        double pixelY = pos.y * -h + abs((pos.x % 2)) * (h / 2);
        pixelY -= (pos.z * 10.0);

        QColor couleur = getTypeColor(hex->getType());
        int nbEtoiles = hex->estPlace() ? hex->getEtoiles() : 0;
        HexagoneItem* item = new HexagoneItem(pixelX, pixelY, taille, couleur, pos.x, pos.y, pos.z, nbEtoiles);
        sceneCite->addItem(item);

        QGraphicsTextItem* txt = sceneCite->addText(QString("%1, %2, %3").arg(pos.x).arg(pos.y).arg(pos.z));
        txt->setPos(pixelX, pixelY);
        txt->setZValue(pos.z + 0.1);
        txt->setDefaultTextColor(Qt::white);
    }
}

void MainWindow::dessinerChantier()
{
    sceneChantier->clear(); // On vide l'ancienne scène

    const Chantier& chantier = Partie::getInstance().getChantier();
    int index = 0;
    double yPos = 40; // Position verticale dans le chantier
    // On parcourt les tuiles du modèle
    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        Tuile* tuile = *it;

        // Création de la tuile graphique
        // On réduit un peu la taille (rayon 20) pour que ça rentre dans la colonne
        TuileItem* item = new TuileItem(tuile, index, 20.0);
        // Positionnement vertical les unes sous les autres
        item->setPos(50, yPos);
        yPos += 120; // Espace entre les tuiles

        sceneChantier->addItem(item);        // Ajouter des items au groupe...        
        // Connecter le signal à votre fonction
        // --- GESTION DE LA SÉLECTION ---
        // On connecte le clic pour qu'il appelle selectionner... avec le bon index
        connect(item, &TuileItem::clicked, this, [this, index]() {
            selectionnerTuileChantier(index);
        });
        if (index == indexTuileSelectionnee) {
            item->setOpacity(0.5); // Exemple simple pour montrer la sélection
        }
        // Gestion du texte (Prix) à côté
        QGraphicsTextItem* txt = sceneChantier->addText(QString("%1 pierres").arg(tuile->getPrix()));
        txt->setPos(100, item->y()); // À droite de la tuile

        index++;
    }

    // Ajuster la zone de scroll pour tout voir
    sceneChantier->setSceneRect(0, 0, 180, yPos);
}

void MainWindow::selectionnerTuileChantier(int index)
{
    indexTuileSelectionnee = index;
    qDebug() << "Tuile selectionnee index :" << index;

    // On redessine pour mettre à jour l'affichage de la sélection (l'opacité changée ci-dessus)
    dessinerChantier();
}

void MainWindow::onRotationClicked()
{
    // Sécurité
    if (indexTuileSelectionnee == -1) return;

    // 1. Retrouver la tuile dans le Chantier via l'index
    const Chantier& chantier = Partie::getInstance().getChantier();

    // Vérification de bornes (au cas où le nombre de tuiles a changé)
    if (indexTuileSelectionnee >= chantier.getNbTuiles()) {
        indexTuileSelectionnee = -1;
        return;
    }

    // On avance un itérateur jusqu'à l'index voulu
    auto it = chantier.begin();
    // On avance manuellement jusqu'à l'index voulu
    for(int i = 0; i < indexTuileSelectionnee; ++i) {
        ++it;
    } // Fonction standard pour avancer de N cases
    Tuile* t = *it;

    // 2. Rotation du Modèle (Les couleurs changent de place logiquement)
    t->tourner();

    // 3. Mise à jour de l'Interface
    // Cela va appeler dessinerChantier(), qui va recréer les TuileItem.
    // Comme le modèle a changé, les couleurs seront dessinées aux nouvelles places.
    // Et comme on recrée l'item, le texte restera bien droit !
    mettreAJourInterface();
}

void MainWindow::onValidationClicked()
{
    if (indexTuileSelectionnee == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une tuile !");
        return;
    }

    // Exemple de placement (à adapter selon ta logique)
    // Partie::getInstance().actionPlacerTuile(indexTuileSelectionnee, 0, 0, 0, rotationActuelle, 0);

    mettreAJourInterface();
}

QColor MainWindow::getTypeColor(TypeQuartier t)
{
    switch(t) {
    case Habitation: return Qt::blue;
    case Marche:     return Qt::yellow;
    case Caserne:    return Qt::red;
    case Temple:     return Qt::magenta;
    case Jardin:     return Qt::green;
    case Carriere:   return Qt::gray;
    default:         return Qt::white;
    }
}
