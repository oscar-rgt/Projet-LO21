#include "mainwindow.h"
#include "tuileitem.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGraphicsPolygonItem>
#include <cmath>
#include <QTextEdit>
#include<QApplication>
#include <QInputDialog>
#include <QMouseEvent>

using namespace std;

const double TUILE_TAILLE = 30.0;
const double COEFF_X = 1.53;
const double OFFSET_Y = sqrt(3.0) * TUILE_TAILLE;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), indexTuileSelectionnee(-1), rotationCompteur(0), inversionEtat(false), previewActive(false), previewX(0), previewY(0), previewZ(0)
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
    initialiserPageConfiguration();

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

    connect(btnJouer, &QPushButton::clicked, this, &MainWindow::afficherMenuConfig);
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

void MainWindow::mettreAJourVisibiliteConfig()
{
    int nbJoueurs = groupeNbJoueurs->checkedId(); // Récupère l'ID (1, 2, 3 ou 4)

    // 1. Gestion de l'IA
    if (nbJoueurs == 1) {
        groupeIA->setVisible(true);
        // En mode solo, on force le nom du joueur 2 à être l'IA pour la logique interne (optionnel mais propre)
        champsNomsJoueurs[1]->setText("Illustre Architecte");
        champsNomsJoueurs[1]->setEnabled(false); // On ne peut pas renommer l'IA
    } else {
        groupeIA->setVisible(false);
        champsNomsJoueurs[1]->setEnabled(true);
        if (champsNomsJoueurs[1]->text() == "Illustre Architecte") champsNomsJoueurs[1]->clear();
    }

    // 2. Gestion des champs de noms
    // On affiche autant de champs que de joueurs
    // Si mode solo (1 joueur), on affiche quand même le champ 1
    // Pour simplifier, on affiche les champs 1 à nbJoueurs

    for (int i = 0; i < 4; ++i) {
        // Le champ est visible si son index < nbJoueurs
        // Exception: en mode solo, on veut juste le nom du joueur 1, l'IA est gérée à part
        bool visible = (i < nbJoueurs);
        champsNomsJoueurs[i]->setVisible(visible);
    }
}

void MainWindow::validerConfiguration()
{
    int nbJoueurs = groupeNbJoueurs->checkedId();

    // Récupération des noms + verif erreurs (non saisie)
    std::vector<std::string> noms;

    for (int i = 0; i < nbJoueurs; ++i) {
        // .trimmed() enlève les espaces au début et à la fin
        // Cela empêche un joueur de s'appeler juste "   "
        QString nomSaisi = champsNomsJoueurs[i]->text().trimmed();

        if (nomSaisi.isEmpty()) {
            // Affichage de l'erreur
            QMessageBox::warning(this, "Configuration incomplète",
                                 QString("Le nom du Joueur %1 est obligatoire !").arg(i + 1));

            // UX : On met le focus (le curseur) directement dans la case fautive
            champsNomsJoueurs[i]->setFocus();

            return; // <--- STOP ! On n'exécute pas la suite, le jeu ne se lance pas.
        }

        noms.push_back(nomSaisi.toStdString());
    }

    // Mode IA
    int niveauIA = 0;
    if (nbJoueurs == 1) {
        niveauIA = groupeNiveauIA->checkedId();
    }

    // Variantes
    std::array<bool, 5> variantes;
    for (int i = 0; i < 5; ++i) {
        variantes[i] = checkBoxesVariantes[i]->isChecked();
    }

    // Mode Tuiles
    Partie::TuileCite mode = checkModeAugmente->isChecked() ? Partie::TuileCite::AUGMENTE : Partie::TuileCite::STANDARD;

    // --- LANCEMENT DU MOTEUR ---
    try {
        Partie::getInstance().initialiser(nbJoueurs, noms, mode, variantes, niveauIA);

        // Changement de page
        stackedWidget->setCurrentWidget(pageJeu);
        mettreAJourInterface(); // Premier dessin

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erreur", e.what());
    }
}

void MainWindow::initialiserPageConfiguration()
{
    pageConfig = new QWidget();

    // 1. Layout GLOBAL de la page (pour centrer le cadre)
    QVBoxLayout *layoutPage = new QVBoxLayout(pageConfig);
    layoutPage->setAlignment(Qt::AlignCenter); // C'est ça qui centre le rectangle !

    // 2. LE CADRE (Le "petit rectangle")
    QFrame *cadreConfig = new QFrame(pageConfig);
    cadreConfig->setFixedWidth(600); // Largeur fixe élégante
    // Style : Fond blanc, contour gris, coins arrondis
    cadreConfig->setStyleSheet(
        ".QFrame { "
        "   border: 1px solid #bdc3c7; "
        "   border-radius: 15px; "
        "}"
        "QLabel { border: none; background: transparent; }" // Pour éviter que le style s'applique aux labels
        );

    // Layout INTERNE du cadre
    QVBoxLayout *layoutCadre = new QVBoxLayout(cadreConfig);
    layoutCadre->setSpacing(15); // Espace entre les éléments
    layoutCadre->setContentsMargins(30, 30, 30, 30); // Marges intérieures (padding)

    // --- CONTENU DU CADRE ---

    // 1. TITRE
    QLabel *titre = new QLabel("CONFIGURATION DE LA PARTIE", cadreConfig);
    titre->setStyleSheet("font-size: 22px; font-weight: bold; color: white; margin-bottom: 10px;");
    titre->setAlignment(Qt::AlignCenter);
    layoutCadre->addWidget(titre);

    // 2. NOMBRE DE JOUEURS
    QGroupBox *boxJoueurs = new QGroupBox("Nombre de joueurs", cadreConfig);
    QHBoxLayout *layoutJoueurs = new QHBoxLayout(boxJoueurs);
    groupeNbJoueurs = new QButtonGroup(this);

    for (int i = 1; i <= 4; ++i) {
        QRadioButton *btn = new QRadioButton(QString::number(i), boxJoueurs);
        if (i == 2) btn->setChecked(true);
        groupeNbJoueurs->addButton(btn, i);
        layoutJoueurs->addWidget(btn);
    }
    layoutCadre->addWidget(boxJoueurs);

    // 3. NOMS DES JOUEURS
    QGroupBox *boxNoms = new QGroupBox("Noms des architectes", cadreConfig);
    QVBoxLayout *layoutNoms = new QVBoxLayout(boxNoms);

    champsNomsJoueurs.clear();
    for (int i = 0; i < 4; ++i) {
        QLineEdit *edit = new QLineEdit(boxNoms);
        edit->setPlaceholderText(QString("Nom du Joueur %1").arg(i + 1));
        edit->setStyleSheet("padding: 5px;");
        champsNomsJoueurs.push_back(edit);
        layoutNoms->addWidget(edit);
    }
    layoutCadre->addWidget(boxNoms);

    // 4. CONFIGURATION IA
    groupeIA = new QGroupBox("L'Illustre Constructeur (IA)", cadreConfig);
    QHBoxLayout *layoutIA = new QHBoxLayout(groupeIA);
    groupeNiveauIA = new QButtonGroup(this);

    QStringList niveaux = {"Hippodamos (Facile)", "Métagénès (Moyen)", "Callicratès (Difficile)"};
    for (int i = 0; i < 3; ++i) {
        QRadioButton *btn = new QRadioButton(niveaux[i], groupeIA);
        if (i == 0) btn->setChecked(true);
        groupeNiveauIA->addButton(btn, i + 1);
        layoutIA->addWidget(btn);
    }
    layoutCadre->addWidget(groupeIA);

    // 5. MODE DE JEU
    QGroupBox *boxMode = new QGroupBox("Mode de jeu", cadreConfig);
    QVBoxLayout *layoutMode = new QVBoxLayout(boxMode);
    checkModeAugmente = new QCheckBox("Tuiles Cité Augmentée", boxMode);
    layoutMode->addWidget(checkModeAugmente);
    layoutCadre->addWidget(boxMode);

    // 6. VARIANTES
    QGroupBox *boxVariantes = new QGroupBox("Variantes", cadreConfig);
    QVBoxLayout *layoutVariantes = new QVBoxLayout(boxVariantes);
    QStringList nomsVariantes = {
        "Habitations", "Marchés", "Casernes", "Temples", "Jardins"
    };

    checkBoxesVariantes.clear();
    for (const QString &nom : nomsVariantes) {
        QCheckBox *cb = new QCheckBox(nom, boxVariantes);
        checkBoxesVariantes.push_back(cb);
        layoutVariantes->addWidget(cb);
    }
    layoutCadre->addWidget(boxVariantes);

    // 7. BOUTONS ACTION
    QHBoxLayout *layoutAction = new QHBoxLayout();
    layoutAction->setContentsMargins(0, 20, 0, 0); // Un peu d'espace au dessus

    QPushButton *btnRetour = new QPushButton("ANNULER", cadreConfig);
    QPushButton *btnValider = new QPushButton("JOUER", cadreConfig);

    btnValider->setStyleSheet("QPushButton { background-color: #27ae60; color: white; font-weight: bold; border-radius: 5px; padding: 10px; }"
                              "QPushButton:hover { background-color: #2ecc71; }");
    btnRetour->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; border-radius: 5px; padding: 10px; }"
                             "QPushButton:hover { background-color: #7f8c8d; }");

    btnValider->setCursor(Qt::PointingHandCursor);
    btnRetour->setCursor(Qt::PointingHandCursor);

    layoutAction->addWidget(btnRetour);
    layoutAction->addSpacing(20);
    layoutAction->addWidget(btnValider);

    layoutCadre->addLayout(layoutAction);

    // --- FIN CONTENU CADRE ---

    // On ajoute le cadre centré à la page
    layoutPage->addWidget(cadreConfig);

    // Connexions
    connect(groupeNbJoueurs, &QButtonGroup::idClicked, this, &MainWindow::mettreAJourVisibiliteConfig);
    connect(btnRetour, &QPushButton::clicked, [this]() { stackedWidget->setCurrentWidget(pageMenuPrincipal); });
    connect(btnValider, &QPushButton::clicked, this, &MainWindow::validerConfiguration);

    mettreAJourVisibiliteConfig();
    stackedWidget->addWidget(pageConfig);
}

void MainWindow::resetConfiguration()
{
    // 1. Remettre 2 joueurs par défaut
    // Note : QButtonGroup::button(id) permet d'accéder au bouton via son ID
    if (QAbstractButton *btn = groupeNbJoueurs->button(2)) {
        btn->setChecked(true);
    }

    // 2. Vider les noms
    for (QLineEdit *champ : champsNomsJoueurs) {
        champ->clear();
    }

    // 3. Remettre l'IA au niveau 1 (Hippodamos)
    if (QAbstractButton *btn = groupeNiveauIA->button(1)) {
        btn->setChecked(true);
    }

    // 4. Décocher le mode augmenté
    checkModeAugmente->setChecked(false);

    // 5. Décocher toutes les variantes
    for (QCheckBox *cb : checkBoxesVariantes) {
        cb->setChecked(false);
    }

    // 6. Forcer la mise à jour visuelle (cacher l'IA car on est repassé à 2 joueurs)
    mettreAJourVisibiliteConfig();
}

void MainWindow::initialiserPageJeu()
{
    pageJeu = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(pageJeu);

    // 2. Zone gauche : La Cité (Visuel)
    sceneCite = new QGraphicsScene(this);
    viewCite = new QGraphicsView(sceneCite);
    viewCite->setRenderHint(QPainter::Antialiasing);
    viewCite->viewport()->installEventFilter(this);
    mainLayout->addWidget(viewCite, 2);




    // 3. Zone droite : Contrôles et Chantier
    QVBoxLayout *sideLayout = new QVBoxLayout();
    labelInfoJoueur = new QLabel("En attente...", pageJeu);
    sideLayout->addWidget(labelInfoJoueur);

    btnRotation = new QPushButton("Pivoter Tuile", pageJeu);
    connect(btnRotation, &QPushButton::clicked, this, &MainWindow::onRotationClicked);
    sideLayout->addWidget(btnRotation);

    btnInversion = new QPushButton("Inverser Tuile", pageJeu);
    connect(btnInversion, &QPushButton::clicked, this, &MainWindow::onInversionClicked);
    sideLayout->addWidget(btnInversion);

    btnValidation = new QPushButton("Valider Placement", pageJeu);
    connect(btnValidation, &QPushButton::clicked, this, &MainWindow::onValidationClicked);
    sideLayout->addWidget(btnValidation);

    btnPasserTour = new QPushButton("Passer son tour", pageJeu);
    connect(btnPasserTour, &QPushButton::clicked, this, &MainWindow::onPasserTourClicked);
    sideLayout->addWidget(btnPasserTour);

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

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == viewCite->viewport() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            QPointF scenePos = viewCite->mapToScene(mouseEvent->pos());
            traiterClicPlateau(scenePos);
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::traiterClicPlateau(QPointF positionScene)
{
    if (indexTuileSelectionnee == -1) {
        QMessageBox::warning(this, "Attention", "Sélectionnez d'abord une tuile dans le chantier.");
        return;
    }

    // 1. Conversion Pixels -> Grille (Algorithme plus proche voisin)
    double w = COEFF_X * TUILE_TAILLE;
    double h = sqrt(3.0) * TUILE_TAILLE;

    // On estime la grille en ignorant le Z pour l'instant (on vise le sol)
    int x_est = round(positionScene.x() / w);

    double minDistance = 1000000.0;
    int bestX = 0, bestY = 0;

    // Recherche locale du centre d'hexagone le plus proche du clic
    for (int i = -2; i <= 2; ++i) {
        for (int j = -3; j <= 3; ++j) {
            int tx = x_est + i;
            // Formule inverse de Y : pixelY = y*-h + offset => y ~ (pixelY - offset)/-h
            double offset = abs(tx % 2) * (h / 2.0);
            int ty = round((positionScene.y() - offset) / -h) + j;

            // Recalcul du centre exact de ce candidat (au niveau 0)
            double cx = tx * w;
            double cy = ty * -h + offset;

            // On compare la distance (en 2D, vue de dessus)
            double dist = pow(positionScene.x() - cx, 2) + pow(positionScene.y() - cy, 2);
            if (dist < minDistance) {
                minDistance = dist;
                bestX = tx;
                bestY = ty;
            }
        }
    }

    // 2. CALCUL AUTOMATIQUE DE LA HAUTEUR (Z)
    // On regarde si la case est occupée. Si oui, on monte.
    int autoZ = 0;
    Joueur* j = Partie::getInstance().getJoueurActuel();
    Cite* cite = j->getCite();

    // On cherche le premier étage libre à ces coordonnées (x,y)
    while (!cite->estLibre({bestX, bestY, autoZ}) && autoZ < 10) {
        autoZ++;
    }

    // 3. Mise à jour de l'état de prévisualisation
    previewX = bestX;
    previewY = bestY;
    previewZ = autoZ;
    previewActive = true;


    mettreAJourInterface();
}



void MainWindow::afficherMenuJeu()
{
    stackedWidget->setCurrentWidget(pageJeu);
}

void MainWindow::afficherMenuRegles()
{
    stackedWidget->setCurrentWidget(pageRegles);
}

void MainWindow::quitterJeu()
{
    QApplication::quit();
}

void MainWindow::afficherMenuConfig()
{
    resetConfiguration(); // <--- On nettoie d'abord
    stackedWidget->setCurrentWidget(pageConfig); // Ensuite on affiche
}


void MainWindow::mettreAJourInterface()
{
    Joueur* j = Partie::getInstance().getJoueurActuel();
    labelInfoJoueur->setText(
        "Tour de : " + QString::fromStdString(j->getNom()) +
        "\nPierres : " + QString::number(j->getPierres()) +
        "\nScore : " + QString::number(j->getScore()->getTotal())
        );

    btnValidation->setEnabled(previewActive);

    dessinerCite(j);
    dessinerPreview();
    dessinerChantier();
}

void MainWindow::dessinerCite(Joueur* joueur) {
    sceneCite->clear();
    Cite* cite = joueur->getCite();
    double taille = 30.0;

    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Coord pos = it->first;
        Hexagone* hex = it->second;

        double w = COEFF_X * taille;
        double h = (sqrt(3.) * taille);
        double pixelX = pos.x * w;
        double pixelY = pos.y * -h + abs((pos.x % 2)) * (h / 2);
        pixelY -= (pos.z * 10.0);

        QColor couleur = getTypeColor(hex->getType());
        int nbEtoiles = hex->estPlace() ? hex->getEtoiles() : 0;

        // Création de l'hexagone avec les étoiles
        HexagoneItem* item = new HexagoneItem(pixelX, pixelY, taille, couleur, pos.x, pos.y, pos.z, nbEtoiles);
        sceneCite->addItem(item);

        QGraphicsTextItem* txt = sceneCite->addText(QString("%1, %2, %3").arg(pos.x).arg(pos.y).arg(pos.z));
        txt->setPos(pixelX, pixelY);
        txt->setZValue(pos.z + 0.1);
        txt->setDefaultTextColor(Qt::white);
    }
}


void MainWindow::dessinerPreview() {
    if (!previewActive || indexTuileSelectionnee == -1) return;

    // Récupération de la tuile sélectionnée
    const Chantier& chantier = Partie::getInstance().getChantier();
    auto it = chantier.begin();
    for(int i =0; i < indexTuileSelectionnee; i++) ++it;
    Tuile* t = *it;

    // Création du "Fantôme" en utilisant TuileItem
    // On passe -1 en index car c'est un objet temporaire hors chantier
    TuileItem* ghost = new TuileItem(t, -1, TUILE_TAILLE);
    // Application du style (Pointillés blancs + Transparence)
    ghost->setSelection(true);
    ghost->setZValue(1000); // Toujours au-dessus de tout



    double taille = TUILE_TAILLE;
    double w = COEFF_X * taille;
    double h = sqrt(3.) * taille;

    double anchorPixelX = previewX * w;
    double anchorPixelY = previewY * -h + abs((previewX % 2)) * (h / 2);
    anchorPixelY -= (previewZ * 10.0);

    // b. Compensation du centrage de TuileItem
    // TuileItem décale ses hexagones pour être centré.
    // L'ancre (Index 0) se trouve donc décalée de 'shiftX' à l'intérieur de l'item.
    // Il faut soustraire ce décalage pour que l'Ancre tombe pile sur la souris.

    double sideOffset = (t->getInversion() ? 1.0 : -1.0) * w;
    double shiftX = -(sideOffset / 3.0); // Le même calcul que dans TuileItem.cpp

    // 6. Placement final
    ghost->setPos(anchorPixelX - shiftX, anchorPixelY);

    // 7. Ajout à la scène
    sceneCite->addItem(ghost);
}


void MainWindow::selectionnerTuileChantier(int index)
{
    indexTuileSelectionnee = index;
    qDebug() << "Tuile selectionnee index :" << index;

    previewActive = false;

    // On redessine pour mettre à jour l'affichage de la sélection (l'opacité changée ci-dessus)
    dessinerChantier();
}

void MainWindow::onInversionClicked() {
    if (indexTuileSelectionnee == -1) return;
    const Chantier& chantier = Partie::getInstance().getChantier();
    if (indexTuileSelectionnee >= chantier.getNbTuiles()) {
        indexTuileSelectionnee = -1;
        return;
    }
    auto it = chantier.begin();
    for(int i = 0; i < indexTuileSelectionnee; ++i) ++it;
    Tuile* t = *it;
    t->inverser();
    inversionEtat = !inversionEtat; // Bascule l'état d'inversion
    mettreAJourInterface();
}

void MainWindow::dessinerChantier()
{
    sceneChantier->clear(); // On vide l'ancienne scène
    const Chantier& chantier = Partie::getInstance().getChantier();
    int index = 0;
    double yPos = 40; // Position verticale initiale
    double rayon = 20.0; // Rayon des hexagones

    // Position horizontale optimale pour centrer les tuiles (valeur ajustée manuellement)
    double xPos = 70; // Valeur testée pour un rayon de 20.0

    // On parcourt les tuiles du modèle
    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        Tuile* tuile = *it;

        // Création de la tuile graphique
        TuileItem* item = new TuileItem(tuile, index, rayon);
        if (index == indexTuileSelectionnee) {
            item->setSelection(true); // <--- Ça active les pointillés blancs !
        } else {
            item->setSelection(false);
        }
        item->setPos(xPos, yPos); // Position ajustée
        yPos += 120; // Espace entre les tuiles

        // Ajout de la tuile à la scène
        sceneChantier->addItem(item);

        // Connexion du signal de clic
        connect(item, &TuileItem::clicked, this, [this, index]() {
            selectionnerTuileChantier(index);
        });



        // Affichage du prix à droite de la tuile
        QGraphicsTextItem* txt = sceneChantier->addText(QString("%1 pierres").arg(tuile->getPrix()));
        txt->setPos(125, item->y()); // À droite de la tuile

        index++;
    }

    // Ajustement de la zone de scroll
    sceneChantier->setSceneRect(0, 0, 180, yPos);
}



void MainWindow::onRotationClicked() {
    if (indexTuileSelectionnee == -1) return;
    const Chantier& chantier = Partie::getInstance().getChantier();
    if (indexTuileSelectionnee >= chantier.getNbTuiles()) {
        indexTuileSelectionnee = -1;
        return;
    }
    auto it = chantier.begin();
    for(int i = 0; i < indexTuileSelectionnee; ++i) ++it;
    Tuile* t = *it;
    t->tourner();
    rotationCompteur = (rotationCompteur + 1) % 3; // Incrémente le compteur
    mettreAJourInterface();
}


void MainWindow::onValidationClicked()
{
    if (!previewActive) {
        QMessageBox::information(this, "Info", "Veuillez d'abord placer une tuile sur le plateau (cliquez sur la grille).");
        return;
    }

    try {
        bool succes = Partie::getInstance().actionPlacerTuile(
            indexTuileSelectionnee, previewX, previewY, previewZ, rotationCompteur, inversionEtat
            );

        if (succes) {
            // Reset état
            previewActive = false;
            rotationCompteur = 0;
            inversionEtat = false;
            indexTuileSelectionnee = -1;
            mettreAJourInterface();

            if (Partie::getInstance().estFinDePartie()) {
                afficherFinDePartie();
            }
        } else {
            // Le backend a renvoyé false
            QMessageBox::warning(this, "Erreur", "Placement refusé par le jeu (vérifiez vos pierres ou les règles).");
        }
    } catch (const std::exception& e) {
        // Le backend a levé une exception
        QMessageBox::warning(this, "Placement invalide", QString("Erreur : %1\n\nEssayez de déplacer la tuile.").arg(e.what()));
    }
}



void MainWindow::onPasserTourClicked()
{
    // Demande de confirmation
    if (QMessageBox::question(this, "Passer son tour", "Êtes-vous sûr de vouloir passer votre tour ?") != QMessageBox::Yes) {
        return;
    }

    // Réinitialisation des états
    rotationCompteur = 0;
    inversionEtat = false;
    indexTuileSelectionnee = -1;

    // Passer au joueur suivant
    Partie::getInstance().passerAuJoueurSuivant();

    // Mise à jour de l'interface
    mettreAJourInterface();

    // Vérification de la fin de partie
    if (Partie::getInstance().estFinDePartie()) {
        afficherFinDePartie();
        return;
    }

    // Si le joueur suivant est une IA, on joue son tour automatiquement
    Joueur* j = Partie::getInstance().getJoueurActuel();
    if (dynamic_cast<IA*>(j)) {
        Partie::getInstance().jouerTourIA();
        mettreAJourInterface();
        if (Partie::getInstance().estFinDePartie()) {
            afficherFinDePartie();
        }
    }
}




void MainWindow::afficherFinDePartie() {
    QString message = "=== FIN DE PARTIE ===\n\n--- SCORES ---\n";
    for (auto it = Partie::getInstance().debutJoueurs(); it != Partie::getInstance().finJoueurs(); ++it) {
        Joueur* j = *it;
        message += QString::fromStdString(j->getNom()) + " : " + QString::number(j->getScore()->getTotal()) + " points\n";
    }
    vector<Joueur*> gagnants = Partie::getInstance().determinerGagnants();
    if (gagnants.size() == 1) {
        message += "\n>>> VAINQUEUR : " + QString::fromStdString(gagnants[0]->getNom()) + " ! <<<";
    } else {
        message += "\n>>> ÉGALITÉ ENTRE : ";
        for (Joueur* g : gagnants) {
            message += QString::fromStdString(g->getNom()) + " ";
        }
        message += "<<<";
    }
    QMessageBox::information(this, "Fin de Partie", message);

    // Retour à l'écran principal après la fermeture de la boîte de dialogue
    stackedWidget->setCurrentWidget(pageMenuPrincipal);
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
