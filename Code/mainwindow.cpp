#include "mainwindow.h"
#include "tuileitem.h"
#include "save.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGraphicsPolygonItem>
#include <cmath>
#include <QTextEdit>
#include<QApplication>
#include <QInputDialog>
#include <QFileDialog>
#include <QDialog>
#include <QMouseEvent>
#include <QGraphicsProxyWidget>

using namespace std;

// --- PALETTE UNIFIÉE (Pour garder le même style partout) ---
namespace Theme {
const QColor ORANGE(220, 141, 85);       // #dc8d55
const QColor MARRON_FONCE(78, 46, 30);    // #4E2E1E
const QColor CUIVRE(115, 69, 38);         // #734526
const QColor BEIGE_CARTE(217, 180, 143);  // #D9B48F
const QColor ROUGE_ERREUR(192, 57, 43); //#c0392b
}

const double TUILE_TAILLE = 30.0;
const double COEFF_X = 1.53;
const double OFFSET_Y = sqrt(3.0) * TUILE_TAILLE;



// =============================================================
// 1. CONSTRUCTEUR & DESTRUCTEUR
// =============================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), indexTuileSelectionnee(-1), rotationCompteur(0), inversionEtat(false), previewActive(false), previewX(0), previewY(0), previewZ(0), affichageResultatIA(false)
{

    setWindowTitle("Akropolis - Qt");
    resize(1024, 768);

    // --- THEME GRAPHIQUE AKROPOLIS ---
    // On définit une palette globale inspirée du jeu
    QString styleGlobal = R"(
        /* 1. FOND GÉNÉRAL (Effet Pierre Chaude / Marbre) */
        QMainWindow, QWidget {
            background-color: #FAF8EF;
            color: #2C3E50; /* Texte Gris Foncé (Lisible) */
            font-family: 'Segoe UI', Arial, sans-serif;
        }

        /* 2. BOUTONS STANDARDS */
        QPushButton {
            background-color: #dc8d55;
            color: white;
            font-weight: bold;
            border-radius: 8px;
            padding: 8px 15px;
            border: 2px solid #b56d38;
        }
        QPushButton:hover {
            background-color: #e89e6b; /* Plus clair au survol */
            border-color: #dc8d55;
        }
        QPushButton:pressed {
            background-color: #bf703d; /* Un peu plus foncé au clic */
            border-color: #a05828;
        }
        QPushButton:disabled {
            background-color: #e0e0e0;
            color: #a0a0a0;
            border: 2px solid #cccccc;
        }

        /* 3. CHAMPS DE TEXTE (Style Parchemin/Papier) */
        QLineEdit {
            background-color: #FFFFFF;
            border: 2px solid #BDC3C7;
            border-radius: 5px;
            padding: 5px;
            selection-background-color: #F39C12;
        }
        QLineEdit:focus {
            border-color: #F39C12; /* Bordure Orange quand on écrit */
        }

        /* 4. GROUPBOX */
        QGroupBox {
            background-color: transparent;
            border: 1px solid #dc8d55;
            border-radius: 8px;
            margin-top: 25px; /* Laisse de la place pour le titre */
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left; /* Titre en haut à gauche */
            left: 10px;
            padding: 0 5px;
            color: #dc8d55; /* Titre Orange */
            font-size: 14px;
        }

        /* 5. CHECKBOX & RADIOBUTTON (CORRIGÉ - VISIBILITÉ) */
        QCheckBox, QRadioButton {
            spacing: 8px;
            font-size: 13px;
            color: #2C3E50;
        }

        /* L'indicateur (le petit carré ou rond) */
        QCheckBox::indicator, QRadioButton::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #BDC3C7;
            background-color: white; /* Fond blanc bien visible */
            border-radius: 4px; /* Coins arrondis pour checkbox */
        }

        /* Quand on passe la souris dessus */
        QCheckBox::indicator:hover, QRadioButton::indicator:hover {
            border-color: #dc8d55;
        }

        /* Quand c'est COCHÉ */
        QCheckBox::indicator:checked, QRadioButton::indicator:checked {
            background-color: #dc8d55; /* Fond Orange */
            border-color: #dc8d55;
            image: none; /* On remplit juste de couleur pour faire simple et visible */
        }

        /* Arrondi spécifique pour les boutons radio */
        QRadioButton::indicator {
            border-radius: 10px; /* Cercle parfait */
        }

        /* 6. CADRE SPÉCIFIQUE CONFIGURATION */

        /* Gros bouton pour les menus (Accueil, Config, Fin) */
        .BoutonMenu {
            min-width: 220px;
            padding: 12px;
            font-size: 18px;
            /* La couleur orange est héritée du QPushButton standard */
        }

        /* Modificateur pour les boutons "Dangereux" (Quitter) */
        .BoutonDanger {
            background-color: #c0392b;
            border-color: #922B21;
        }
        .BoutonDanger:hover {
            background-color: #e74c3c;
        }

        /* CADRE UNIFIÉ (Transparent + Bord Orange) */
        .CadreConfig {
            border: 2px solid #dc8d55;
            border-radius: 15px;
            background-color: #FAF8EF; /* Fond semi-opaque pour lisibilité si besoin, ou transparent */
            /* Si vous voulez 100% transparent comme avant, mettez : background-color: transparent; */
        }

        /* 7. ETIQUETTES D'INFORMATION (Score, Tour, Piles) */
        .InfoLabel {
            background-color: rgba(255, 255, 255, 0.6); /* Fond blanc semi-transparent */
            border: 2px solid #dc8d55;                  /* Bordure Orange */
            border-radius: 10px;                        /* Coins ronds */
            padding: 8px;                              /* Espace intérieur */
            font-size: 13px;
            color: #4E2E1E;                             /* Texte Marron foncé */
        }
    )";

    // Application du style à toute l'application
    this->setStyleSheet(styleGlobal);
    // ---------------------------------

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

    pagePrecedente = nullptr;
}

MainWindow::~MainWindow() {}


// =============================================================
// 2. INITIALISATION DES PAGES (INTERFACE)
// =============================================================

void MainWindow::initialiserPageMenuPrincipal()
{
    pageMenuPrincipal = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(pageMenuPrincipal);

    // --- DESIGN GÉNÉRAL ---
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30);
    layout->setContentsMargins(50, 50, 50, 50);

    // --- 1. LE LOGO (IMAGE) ---
    QLabel *labelLogo = new QLabel(pageMenuPrincipal);

    labelLogo->setStyleSheet("border: none; background-color: transparent; margin-bottom: 10px;");
    // Chargement de l'image depuis les ressources
    QPixmap logoPixmap(":/images/akropolis-title.png");

    if (logoPixmap.isNull()) {
        labelLogo->setText("AKROPOLIS");
        QFont fontLogo;
        fontLogo.setPixelSize(50);
        fontLogo.setBold(true);
        labelLogo->setFont(fontLogo);
    } else {
        QPixmap scaledLogo = logoPixmap.scaledToWidth(600, Qt::SmoothTransformation);
        labelLogo->setPixmap(scaledLogo);
    }

    labelLogo->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelLogo);

    layout->addSpacing(20);

    // --- 2. LES BOUTONS ---
    QVBoxLayout *layoutBoutons = new QVBoxLayout();
    layoutBoutons->setSpacing(20);

    // Bouton JOUER
    QPushButton *btnJouer = new QPushButton("JOUER UNE PARTIE", pageMenuPrincipal);
    btnJouer->setProperty("class", "BoutonMenu"); // <--- Utilise la classe globale
    btnJouer->setCursor(Qt::PointingHandCursor);
    connect(btnJouer, &QPushButton::clicked, this, &MainWindow::afficherMenuConfig);
    layoutBoutons->addWidget(btnJouer, 0, Qt::AlignCenter);

    // Bouton RÈGLES
    QPushButton *btnRegles = new QPushButton("RÈGLES DU JEU", pageMenuPrincipal);
    btnRegles->setProperty("class", "BoutonMenu"); // <--- Utilise la classe globale
    btnRegles->setCursor(Qt::PointingHandCursor);
    connect(btnRegles, &QPushButton::clicked, this, &MainWindow::afficherMenuRegles);
    layoutBoutons->addWidget(btnRegles, 0, Qt::AlignCenter);

    // Bouton CHARGER
    QPushButton *btnCharger = new QPushButton("CHARGER UNE PARTIE", pageMenuPrincipal);
    btnCharger->setProperty("class", "BoutonMenu"); // <--- Utilise la classe globale
    btnCharger->setCursor(Qt::PointingHandCursor);
    connect(btnCharger, &QPushButton::clicked, this, &MainWindow::onChargerPartieClicked);
    layoutBoutons->addWidget(btnCharger, 0, Qt::AlignCenter);

    // Bouton QUITTER
    QPushButton *btnQuitter = new QPushButton("QUITTER", pageMenuPrincipal);
    // On combine les deux classes : C'est un bouton menu ET il est rouge
    btnQuitter->setProperty("class", "BoutonMenu BoutonDanger");
    btnQuitter->setCursor(Qt::PointingHandCursor);
    connect(btnQuitter, &QPushButton::clicked, this, &MainWindow::quitterJeu);
    layoutBoutons->addWidget(btnQuitter, 0, Qt::AlignCenter);

    layout->addLayout(layoutBoutons);

    QLabel *credits = new QLabel(
        "-----------------------------------------------------------\n"
        "LO21 - Programmation et conception orientée objet         \n"
        "                       Semestre Automne 2025\n"
        "Oscar R., Louane R., Valentin R., Noemie M., Jeanne D.     \n"
        "-----------------------------------------------------------",
        pageMenuPrincipal
        );
    credits->setStyleSheet("font-size: 12px; margin-top: 20px; text-align: justify;");
    layout->addWidget(credits, 0, Qt::AlignCenter);

    stackedWidget->addWidget(pageMenuPrincipal);
}



void MainWindow::initialiserPageRegles() {
    pageRegles = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(pageRegles);
    layout->setContentsMargins(20, 20, 20, 20); // Marges externes
    layout->setSpacing(10); // Espacement entre les éléments

    // Titre
    QLabel *titleLabel = new QLabel("RÈGLES D'AKROPOLIS", pageRegles);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #dc8d55; margin-bottom: 20px;");
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);

    // Zone de défilement (occupe tout l'espace disponible)
    QScrollArea *scrollArea = new QScrollArea(pageRegles);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: transparent; border: none; }"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #f0f0f0;"
        "    width: 12px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #dc8d55;"
        "    min-height: 20px;"
        "    border-radius: 6px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        );

    // Widget interne pour le texte (avec layout centré)
    QWidget *textContainer = new QWidget();
    QVBoxLayout *textContainerLayout = new QVBoxLayout(textContainer);
    textContainerLayout->setContentsMargins(20, 20, 20, 20); // Marges internes

    // Widget pour centrer le texte (largeur fixe)
    QWidget *centerWidget = new QWidget();
    centerWidget->setFixedWidth(1400); // Largeur fixe pour le texte
    centerWidget->setFixedHeight(1000);
    QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);

    QTextEdit *textRegles = new QTextEdit(centerWidget);
    textRegles->setReadOnly(true);
    textRegles->setFrameShape(QFrame::NoFrame);
    textRegles->setStyleSheet("background-color: transparent; color: #2C3E50; font-size: 16px;");
    textRegles->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QString contenuHtml = R"(
        <div style='font-size: 16px; line-height: 1.4;'>
            <h3 style='color: #dc8d55;'>1. LE BUT</h3>
            <p style='text-align: justify;'>
                Construisez la cité la plus prestigieuse. Les points sont marqués grâce aux Quartiers multipliés par les Places correspondantes.
            </p>
            <h3 style='color: #dc8d55;'>2. LES QUARTIERS</h3>
            <p style='text-align: justify;'>
                <b>- Habitation (H) :</b> 1 point pour chaque quartier habitation adjacent à un autre. (Ne compte que pour votre plus grand groupe d'habitations)<br>
                <b>- Marché (M) :</b> 1 point si votre marché n'est adjacent à aucun autre.<br>
                <b>- Caserne (C) :</b> 1 point si votre caserne n'est pas complètement entourée par d'autres hexagones.<br>
                <b>- Temple (T) :</b> 1 point si votre temple est complètement entouré par d'autres hexagones.<br>
                <b>- Jardin (J) :</b> 1 point pour chaque jardin posé sans condition.<br>
                <b>- Carrière (X) :</b> Permet d'agrandir votre cité mais ne donne pas de points.
            </p>
            <h3 style='color: #dc8d55;'>3. LES PLACES</h3>
            <p style='text-align: justify;'>
                Les places de chaque type vous permettent de multiplier vos points en fonction du chiffre qui est écrit dessus.<br>
                Par exemple, un hexagone 2H est une place Habitation à 2 étoiles. Elle multiplie donc par 2 les points gagnés par vos quartiers Habitation.<br>
                <i>/!\ ATTENTION : Si vous n'avez aucune place d'un certain type, vous ne marquez aucun point pour ses quartiers correspondants.</i>
            </p>
            <h3 style='color: #dc8d55;'>4. LA PIERRE</h3>
            <p style='text-align: justify;'>
                Vous commencez avec un nombre de 2 pierres. Ces dernières vous permettront d'acheter des tuiles.<br>
                Les pierres influent aussi sur votre score. En effet, chaque pierre vous rapporte un point.<br>
                De plus, en cas d'égalité en fin de partie, le joueur avec le plus de pierres l'emporte.<br>
                Les pierres s'obtiennent en construisant au-dessus d'une carrière. Chaque carrière recouverte donne une pierre.
            </p>
            <h3 style='color: #dc8d55;'>5. PLACEMENT</h3>
            <p style='text-align: justify;'>
                Votre cité peut s'étendre aussi bien en surface qu'en hauteur. Lorsqu'un hexagone est placé en hauteur, son nombre de points est multiplié par son niveau d'élévation.<br>
                Par exemple, un quartier Jardin placé au niveau 3 rapporte 3 points.<br>
                À vous de trouver le bon équilibre pour devenir le plus prestigieux des architectes !
            </p>
        </div>
    )";
    textRegles->setHtml(contenuHtml);
    centerLayout->addWidget(textRegles);

    // Ajoute le widget centré au conteneur principal
    textContainerLayout->addWidget(centerWidget, 0, Qt::AlignCenter);
    scrollArea->setWidget(textContainer);

    // Ajoute la scrollArea au layout principal avec un stretch maximal
    layout->addWidget(scrollArea, 1); // Le "1" permet à la scrollArea de prendre tout l'espace disponible

    // Bouton Retour (toujours visible en bas)
    QPushButton *btnRetour = new QPushButton("RETOUR", pageRegles);
    btnRetour->setFixedSize(150, 40);
    btnRetour->setStyleSheet("font-weight: bold; background-color: #dc8d55; color: white; border-radius: 5px;");
    layout->addWidget(btnRetour, 0, Qt::AlignCenter);

    // Connexion du bouton
    connect(btnRetour, &QPushButton::clicked, [this]() {

        // Cas 1 : On vient du JEU (Pause)
        if (pagePrecedente == pageJeu) {
            stackedWidget->setCurrentWidget(pageJeu); // 1. On affiche le jeu
            onReglagesClicked();                      // 2. On ROUVRE le pop-up Réglages par-dessus
        }

        // Cas 2 : On vient du MENU PRINCIPAL
        else {
            stackedWidget->setCurrentWidget(pageMenuPrincipal);
        }
    });
    stackedWidget->addWidget(pageRegles);
}

void MainWindow::initialiserPageConfiguration()
{
    pageConfig = new QWidget();

    // 1. Layout PRINCIPAL de la page (qui contient le ScrollArea)
    QVBoxLayout *layoutPage = new QVBoxLayout(pageConfig);
    layoutPage->setContentsMargins(0, 0, 0, 0); // On occupe tout l'espace

    // --- CRÉATION DE LA ZONE DE SCROLL ---
    QScrollArea *scrollArea = new QScrollArea(pageConfig);
    scrollArea->setWidgetResizable(true);       // Le contenu s'adapte à la largeur
    scrollArea->setFrameShape(QFrame::NoFrame); // Pas de bordure autour du scroll
    scrollArea->setStyleSheet("background: transparent;"); // Fond transparent pour voir le background global

    // Conteneur interne (ce qui va scroller)
    QWidget *scrollContent = new QWidget();
    scrollContent->setStyleSheet("background: transparent;");

    // Layout du contenu (pour centrer le cadre au milieu)
    QVBoxLayout *layoutScroll = new QVBoxLayout(scrollContent);
    layoutScroll->setAlignment(Qt::AlignCenter);
    layoutScroll->setContentsMargins(20, 20, 20, 20); // Marges de sécurité

    // --------------------------------------

    // 2. LE CADRE DE CONFIGURATION (Maintenant dans le scrollContent)
    QFrame *cadreConfig = new QFrame(scrollContent);
    // On utilise la classe unifiée définie dans le constructeur
    cadreConfig->setProperty("class", "CadreConfig");
    cadreConfig->setFixedWidth(600);

    // Layout INTERNE du cadre
    QVBoxLayout *layoutCadre = new QVBoxLayout(cadreConfig);
    layoutCadre->setSpacing(15);
    layoutCadre->setContentsMargins(30, 30, 30, 30);

    // --- CONTENU DU FORMULAIRE ---

    // A. TITRE
    QLabel *titre = new QLabel("CONFIGURATION DE LA PARTIE", cadreConfig);
    titre->setStyleSheet("font-size: 22px; font-weight: bold; color: #dc8d55; margin-bottom: 10px;");
    titre->setAlignment(Qt::AlignCenter);
    layoutCadre->addWidget(titre);

    // B. NOMBRE DE JOUEURS
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

    // C. NOMS DES JOUEURS
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

    // D. CONFIGURATION IA
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

    // E. MODE DE JEU
    QGroupBox *boxMode = new QGroupBox("Mode de jeu", cadreConfig);
    QVBoxLayout *layoutMode = new QVBoxLayout(boxMode);
    checkModeAugmente = new QCheckBox("Tuiles Cité Augmentée", boxMode);
    layoutMode->addWidget(checkModeAugmente);
    layoutCadre->addWidget(boxMode);

    // F. VARIANTES
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

    // G. BOUTONS ACTION
    QHBoxLayout *layoutAction = new QHBoxLayout();
    layoutAction->setContentsMargins(0, 20, 0, 0);

    QPushButton *btnRetour = new QPushButton("ANNULER", cadreConfig);
    QPushButton *btnValider = new QPushButton("JOUER", cadreConfig);

    // a cause du scrollbar, on doit forcer le style des boutons
    QString styleBtn =
        "QPushButton { "
        "  background-color: #dc8d55; color: white; border: 2px solid #b56d38; "
        "  border-radius: 8px; padding: 10px 20px; font-weight: bold; min-width: 120px; font-size: 14px;"
        "} "
        "QPushButton:hover { background-color: #e89e6b; }";

    btnRetour->setStyleSheet(styleBtn);
    btnValider->setStyleSheet(styleBtn);

    btnValider->setCursor(Qt::PointingHandCursor);
    btnRetour->setCursor(Qt::PointingHandCursor);

    layoutAction->addWidget(btnRetour);
    layoutAction->addSpacing(20);
    layoutAction->addWidget(btnValider);

    layoutCadre->addLayout(layoutAction);

    // --- ASSEMBLAGE FINAL ---

    // 1. On ajoute le cadre au conteneur scrollable
    layoutScroll->addWidget(cadreConfig);

    // 2. On définit le widget du ScrollArea
    scrollArea->setWidget(scrollContent);

    // 3. On ajoute le ScrollArea à la page principale
    layoutPage->addWidget(scrollArea);

    // Connexions
    connect(groupeNbJoueurs, &QButtonGroup::idClicked, this, &MainWindow::mettreAJourVisibiliteConfig);
    connect(btnRetour, &QPushButton::clicked, [this]() { stackedWidget->setCurrentWidget(pageMenuPrincipal); });
    connect(btnValider, &QPushButton::clicked, this, &MainWindow::validerConfiguration);

    mettreAJourVisibiliteConfig();
    stackedWidget->addWidget(pageConfig);
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
    labelInfoJoueur->setProperty("class", "InfoLabel");
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

    // --- AJOUT BOUTON RÉGLAGES (FLOTTANT) ---
    btnReglages = new QPushButton("Paramètres", pageJeu);
    // On le place en haut à gauche (absolu) pour qu'il soit par-dessus la vue 3D
    btnReglages->move(20, 20);

    btnReglages->setCursor(Qt::PointingHandCursor);

    // Connexion
    connect(btnReglages, &QPushButton::clicked, this, &MainWindow::onReglagesClicked);

    // Important : On s'assure qu'il reste au premier plan
    btnReglages->raise();

    stackedWidget->addWidget(pageJeu);


    QLabel *labelChantier = new QLabel("--- CHANTIER ---", pageJeu);
    labelChantier->setAlignment(Qt::AlignCenter);
    labelChantier->setStyleSheet("font-size: 16px; font-weight: bold; color: #dc8d55; margin-top: 20px; margin-bottom: 5px;");
    sideLayout->addWidget(labelChantier);

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

    labelPilesRestantes = new QLabel("Piles : -", pageJeu);
    labelPilesRestantes->setProperty("class", "InfoLabel");
    labelPilesRestantes->setAlignment(Qt::AlignCenter);
    sideLayout->addWidget(labelPilesRestantes);
}

// =============================================================
// 3. NAVIGATION & MENUS
// =============================================================


void MainWindow::afficherMenuJeu()
{
    stackedWidget->setCurrentWidget(pageJeu);
}

void MainWindow::afficherMenuRegles()
{
    pagePrecedente = pageMenuPrincipal;
    stackedWidget->setCurrentWidget(pageRegles);
}

void MainWindow::afficherMenuConfig()
{
    resetConfiguration(); // <--- On nettoie d'abord
    stackedWidget->setCurrentWidget(pageConfig); // Ensuite on affiche
}

void MainWindow::quitterJeu()
{
    QApplication::quit();
}

void MainWindow::onReglagesClicked() {
    // Fenêtre modale transparente
    QDialog dialog(this);
    dialog.setWindowTitle("Pause");
    dialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.setFixedSize(300, 350);

    // Style
    dialog.setStyleSheet(
        "QFrame { background-color: #FAF8EF; border: 3px solid #dc8d55; border-radius: 15px; }"
        "QLabel { color: #dc8d55; font-size: 20px; font-weight: bold; margin-bottom: 15px; }"
        );

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QFrame* frame = new QFrame();
    QVBoxLayout* l = new QVBoxLayout(frame);
    l->setAlignment(Qt::AlignCenter);

    QLabel* titre = new QLabel("PAUSE");
    titre->setAlignment(Qt::AlignCenter);
    l->addWidget(titre);

    // BOUTON REPRENDRE

    QString styleTaille = "font-size: 14px;"; // changement pour une taille de texte plus petite

    QPushButton* btnReprendre = new QPushButton("REPRENDRE");
    btnReprendre->setProperty("class", "BoutonMenu");
    btnReprendre->setStyleSheet(styleTaille);
    connect(btnReprendre, &QPushButton::clicked, &dialog, &QDialog::accept);
    l->addWidget(btnReprendre);

    // BOUTON RÈGLES (Navigation Intelligente)
    QPushButton* btnRegles = new QPushButton("RÈGLES");
    btnRegles->setProperty("class", "BoutonMenu");
    btnRegles->setStyleSheet(styleTaille);
    connect(btnRegles, &QPushButton::clicked, [this, &dialog]() {
        pagePrecedente = pageJeu;     // <--- On mémorise "Jeu"
        stackedWidget->setCurrentWidget(pageRegles);
        dialog.accept(); // On ferme le pop-up
    });
    l->addWidget(btnRegles);

    // BOUTON SAUVEGARDER
    QPushButton* btnSave = new QPushButton("SAUVEGARDER & QUITTER");
    btnSave->setProperty("class", "BoutonMenu");
    btnSave->setStyleSheet(styleTaille);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSauvegarderClicked);
    l->addWidget(btnSave);

    // BOUTON SAUVEGARDER
    QPushButton* btnQuitter = new QPushButton("QUITTER SANS SAUVEGARDER");
    btnQuitter->setProperty("class", "BoutonMenu BoutonDanger");
    btnQuitter->setStyleSheet(styleTaille);
    btnQuitter->setCursor(Qt::PointingHandCursor);
    connect(btnQuitter, &QPushButton::clicked, this, &MainWindow::quitterJeu);
    l->addWidget(btnQuitter);

    layout->addWidget(frame);
    dialog.exec();
}



void MainWindow::onSauvegarderClicked() {
    // 1. Pop-up de confirmation (Oui / Non)
    QMessageBox::StandardButton reponse = QMessageBox::question(
        this,
        "Sauvegarder & Quitter",
        "Voulez-vous sauvegarder et quitter la partie ?",
        QMessageBox::Yes | QMessageBox::No
        );

    // 2. Si l'utilisateur clique sur OUI
    if (reponse == QMessageBox::Yes) {

        // On tente de sauvegarder via le SaveManager
        bool succes = SaveManager::sauvegarder(Partie::getInstance(), "save.txt");

        if (succes) {
            // ">> Partie sauvegardee avec succes..."
            QMessageBox::information(this, "Succès", "Partie sauvegardée avec succès dans 'save.txt' !");

            // On quitte l'application proprement
            QApplication::quit();
        }
        else {
            // ">> ERREUR CRITIQUE..."
            QMessageBox::critical(this, "Erreur Critique", "La sauvegarde a échoué.\nImpossible d'écrire le fichier.");
            // On ne quitte PAS, pour laisser au joueur une chance de réessayer ou de continuer
        }
    }
    // Si Non, on ne fait rien, le pop-up se ferme juste.
}



// =============================================================
// 4. LOGIQUE DE CONFIGURATION (AVANT-PARTIE)
// =============================================================

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

// =============================================================
// 5. GESTION DES ÉVÉNEMENTS (SOURIS & CLICS)
// =============================================================

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == viewCite->viewport() && event->type() == QEvent::MouseButtonPress) {
        if (affichageResultatIA) {
            return false;
        }
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


void MainWindow::selectionnerTuileChantier(int index)
{
    indexTuileSelectionnee = index;
    qDebug() << "Tuile selectionnee index :" << index;

    previewActive = false;

    // On redessine pour mettre à jour l'affichage de la sélection (l'opacité changée ci-dessus)
    dessinerChantier();
    mettreAJourInterface();
}

// =============================================================
// 6. ACTIONS DU JOUEUR (SLOTS BOUTONS)
// =============================================================


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


void MainWindow::onValidationClicked()
{
    if (!previewActive) {
        QMessageBox::information(this, "Info", "Veuillez d'abord placer une tuile sur le plateau (cliquez sur la grille).");
        return;
    }

    try {
        Partie::getInstance().actionPlacerTuile(indexTuileSelectionnee, previewX, previewY, previewZ, rotationCompteur);
        // Reset état
        previewActive = false;
        rotationCompteur = 0;
        inversionEtat = false;
        indexTuileSelectionnee = -1;
        mettreAJourInterface();

        if (Partie::getInstance().estFinDePartie()) {
            mettreAJourInterface();
            afficherFinDePartie();
            return;
        }

        //Tour IA
        Joueur* joueurSuivant = Partie::getInstance().getJoueurActuel(); //joueur suivant a été fait par placer tuile
        IA* ia = dynamic_cast<IA*>(joueurSuivant); //renvoie null si joueur pas IA

        if (ia) {
            dernierIndexIA = Partie::getInstance().jouerTourIA();

            //active le mode "Écran de l'IA"
            affichageResultatIA = true;
        }

        // Mise à jour de l'interface (affichera soit le tour de l'humain suivant, soit l'écran IA)
        mettreAJourInterface();


    } // --- GESTION DES ERREURS ---
    catch (const CiteException& e) {
        // Erreur géométrique (Cité)
        QMessageBox::warning(this, "Construction Impossible", e.what());
    }
    catch (const PartieException& e) {
        // Erreur de règles (Pierres)
        QMessageBox::warning(this, "Action Interdite", e.what());
    }
    catch (const std::exception& e) {
        // Autre (Bug ?)
        QMessageBox::critical(this, "Erreur", e.what());
    }
}

void MainWindow::onChargerPartieClicked() {
    // Petit message d'attente (optionnel, mais sympa)
    // On utilise un try/catch au cas où SaveManager lance une exception
    try {
        bool succes = SaveManager::charger(Partie::getInstance(), "save.txt");

        if (succes) {
            // 1. Message de succès
            QMessageBox::information(this, "Chargement terminé", "Votre partie a été restaurée avec succès !");

            // 2. On change de page (On va vers le JEU)
            stackedWidget->setCurrentWidget(pageJeu);

            // 3. IMPORTANT : On force le redessin de l'interface avec les données chargées
            mettreAJourInterface();
        }
        else {
            // Echec (Fichier introuvable ou corrompu)
            QMessageBox::warning(this, "Echec du chargement",
                                 "Impossible de charger la sauvegarde.\n"
                                 "Le fichier 'save.txt' est introuvable ou illisible.");
        }
    }
    catch (const std::exception& e) {
        // En cas de gros crash pendant la lecture
        QMessageBox::critical(this, "Erreur Critique",
                              QString("Une erreur est survenue pendant le chargement :\n%1").arg(e.what()));
    }
}


void MainWindow::onContinuerIAClicked()
{
    //désactive le mode IA
    affichageResultatIA = false;

    if (Partie::getInstance().estFinDePartie()) {
        afficherFinDePartie();
    } else {
        // La partie continue, on rafraîchit pour afficher le plateau du joueur humain
        mettreAJourInterface();
    }
}


// =============================================================
// 7. MOTEUR DE RENDU & MISE À JOUR VISUELLE
// =============================================================


void MainWindow::mettreAJourInterface()
{
    int pilesRestantes = Partie::getInstance().getNbPiles() - Partie::getInstance().getIndexPileActuelle();
    if (pilesRestantes < 0) pilesRestantes = 0;

    // Version améliorée avec indicateur visuel
    QString couleurPile = (pilesRestantes <= 2) ? Theme::ROUGE_ERREUR.name() : Theme::ORANGE.name(); // Rouge si critique
    QString iconePile = (pilesRestantes <= 2) ? "⚠️" : "📚";

    QString couleurTitre = Theme::CUIVRE.name();

    labelPilesRestantes->setText(QString(
                                     "<div style='text-align: center; padding: 8px;'>"
                                     "   <span style='font-size: 12px; color: #734526;'>PILES RESTANTES</span><br>"
                                     "   <span style='font-size: 24px;'>%1</span> "
                                     "   <span style='font-size: 28px; font-weight: bold; color: %2;'>%3</span>"
                                     "</div>"
                                     ).arg(iconePile).arg(couleurPile).arg(pilesRestantes));

    if (affichageResultatIA) {
        // Mode IA : on cache les contrôles de jeu, on montre "Continuer"
        btnRotation->hide();
        btnInversion->hide();
        btnValidation->hide();
        btnReglages->raise();

        // On récupère l'IA (Attention : comme jouerTourIA() a déjà passé le tour,
        // l'IA n'est plus le "joueur actuel" dans le backend.
        // Il faut la retrouver dans la liste des joueurs).
        IA* iaTrouvee = nullptr;
        auto it = Partie::getInstance().debutJoueurs();
        while(it != Partie::getInstance().finJoueurs()) {
            if (dynamic_cast<IA*>(*it)) {
                iaTrouvee = dynamic_cast<IA*>(*it);
                break;
            }
            ++it;
        }

        if (iaTrouvee) {
            labelInfoJoueur->setText("Tour de : " + QString::fromStdString(iaTrouvee->getNom()));
            dessinerInterfaceIA(iaTrouvee);
        }
    }
    else{
        Joueur* j = Partie::getInstance().getJoueurActuel();
        htmlTexteJoueur = QString(
                              "<div style='line-height: 150%; padding: 10px;'>"
                              "   <div style='text-align: center; margin-bottom: 15px;'>"
                              "      <span style='font-size: 13px; color: #734526; letter-spacing: 1px;'>TOUR DE</span><br>"
                              "      <span style='font-size: 22px; font-weight: bold; color: #dc8d55; text-shadow: 1px 1px 2px rgba(0,0,0,0.1);'>%1</span>"
                              "   </div>"
                              "   <div style='background: linear-gradient(135deg, rgba(220,141,85,0.15), rgba(220,141,85,0.05)); "
                              "               border-radius: 8px; padding: 12px; margin-top: 10px;'>"
                              "      <div style='display: flex; justify-content: space-between; margin-bottom: 8px;'>"
                              "         <span style='font-size: 14px; color: #734526;'>💎 Pierres</span>"
                              "         <span style='font-size: 18px; font-weight: bold; color: #dc8d55;'>%2</span>"
                              "      </div>"
                              "      <div style='display: flex; justify-content: space-between;'>"
                              "         <span style='font-size: 14px; color: #734526;'>⭐ Score</span>"
                              "         <span style='font-size: 18px; font-weight: bold; color: #e67e22;'>%3</span>"
                              "      </div>"
                              "   </div>"
                              "</div>"
                              ).arg(QString::fromStdString(j->getNom()))
                              .arg(j->getPierres())
                              .arg(j->getScore()->calculerScore());

        labelInfoJoueur->setText(htmlTexteJoueur);

        btnRotation->setEnabled(indexTuileSelectionnee!=-1);
        btnRotation->show();
        btnInversion->setEnabled(indexTuileSelectionnee!=-1);
        btnInversion->show();

        btnValidation->setEnabled(previewActive);
        btnValidation->show();

        dessinerCite(j);
        dessinerPreview(j);

    }
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


        QColor couleur = TuileItem::getTypeColor(hex->getType());
        int nbEtoiles = hex->estPlace() ? hex->getEtoiles() : 0;

        // Création de l'hexagone avec les étoiles
        HexagoneItem* item = new HexagoneItem(pixelX, pixelY, taille, couleur, pos.x, pos.y, pos.z, nbEtoiles);
        QString strHauteur = QString::number(pos.z);
        QGraphicsTextItem* txt = new QGraphicsTextItem(strHauteur, item);

        QFont font = txt->font();
        font.setPixelSize(10);
        txt->setFont(font);

        txt->setDefaultTextColor(Qt::black);

        QRectF bound = txt->boundingRect();
        txt->setPos(-bound.width() / 2.0, bound.height() - 14);
        txt->setZValue(1000);
        sceneCite->addItem(item);

    }
}


void MainWindow::dessinerPreview(Joueur* j) {
    if (!previewActive || indexTuileSelectionnee == -1) return;

    // Récupération de la tuile sélectionnée
    const Chantier& chantier = Partie::getInstance().getChantier();
    auto it = chantier.begin(); //CACA BOUDIN JE SUIS VALENTIN
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
    //on trouve le plus haut hexagone en dessous de la preview pour l'afficher au dessus
    int hmax = 0; // Par défaut au sol
    Cite* cite = j->getCite();
    // On scanne les étages pour trouver la tuile "survivante" (celle du dessus)
    for (int z = 0; z < 20; z++) {
        // Si on trouve une tuile occupée (!estLibre), c'est le sommet actuel de la tour
        if (!cite->estLibre({previewX, previewY, z})) {
            hmax = z + 1; // On se place donc à l'étage juste au-dessus
            break;        // On a trouvé, pas besoin de chercher plus haut
        }
    }

    // On met à jour la variable membre pour la validation future
    previewZ = hmax;

    double positionY = anchorPixelY - 11.0;

    // b. Compensation du centrage de TuileItem
    // TuileItem décale ses hexagones pour être centré.
    // L'ancre (Index 0) se trouve donc décalée de 'shiftX' à l'intérieur de l'item.
    // Il faut soustraire ce décalage pour que l'Ancre tombe pile sur la souris.

    double sideOffset = (t->getInversion() ? 1.0 : -1.0) * w;
    double shiftX = -(sideOffset / 3.0); // Le même calcul que dans TuileItem.cpp

    // 6. Placement final
    if(hmax == 0) ghost->setPos(anchorPixelX - shiftX, anchorPixelY);
    else ghost->setPos(anchorPixelX - shiftX, positionY);

    // 7. Ajout à la scène
    sceneCite->addItem(ghost);
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
        // 1. Texte avec le carré (■) au lieu de "pierres"
        QString textePrix = QString(
                                "<div style='text-align: center;'>"
                                "   <div style='font-size: 11px; color: #734526;'>COÛT</div>"
                                "   <div style='font-size: 22px; font-weight: bold; color: #dc8d55;'>%1 ◆</div>"
                                "</div>"
                                ).arg(tuile->getPrix());

        QGraphicsTextItem* txt = sceneChantier->addText("");
        txt->setHtml(textePrix);
        txt->setPos(125, item->y() - 5);

        index++;
    }

    // Ajustement de la zone de scroll
    sceneChantier->setSceneRect(0, 0, 180, yPos);
}





void MainWindow::dessinerInterfaceIA(IA* ia) {
    sceneCite->clear();

    // --- 1. POLICES (Réglées comme avant) ---
    QFont fontTitre; fontTitre.setPixelSize(24); fontTitre.setBold(true);
    QFont fontTexte; fontTexte.setPixelSize(16);
    QFont fontSousTitre; fontSousTitre.setPixelSize(18); fontSousTitre.setBold(true);
    QFont fontStats; fontStats.setPixelSize(16); fontStats.setBold(true);

    // --- 2. TITRE ---
    QGraphicsTextItem* titre = sceneCite->addText("L'ILLUSTRE ARCHITECTE A JOUÉ");
    titre->setDefaultTextColor(Theme::ORANGE); // Au lieu de 14454101
    titre->setFont(fontTitre);
    titre->setPos(-titre->boundingRect().width() / 2, -260);

    // --- 3. INFO ACTION ---
    QString actionText = QString("Il a pris la tuile n°%1 du chantier.").arg(dernierIndexIA + 1);
    QGraphicsTextItem* info = sceneCite->addText(actionText);
    info->setDefaultTextColor(Theme::MARRON_FONCE); // Au lieu de 5123614
    info->setFont(fontTexte);
    info->setPos(-info->boundingRect().width() / 2, -220);

    // --- 4. STATS (Cadre centré) ---
    double statsW = 260;
    double statsH = 80;
    // Cadre blanc avec bordure CUIVRE (comme dans votre ancien code 7554342)
    QGraphicsRectItem* statsBg = sceneCite->addRect(0, 0, statsW, statsH, QPen(Theme::CUIVRE, 2), QBrush(Qt::white));
    statsBg->setPos(-statsW / 2, -160);

    // Pierres
    QGraphicsTextItem* pierres = sceneCite->addText(QString("Pierres : %1 ■").arg(ia->getPierres()));
    pierres->setDefaultTextColor(Theme::ORANGE);
    pierres->setFont(fontStats);
    // Centrage relatif au cadre
    pierres->setPos(-statsW/2 + (statsW - pierres->boundingRect().width())/2, -160 + 10);

    // Score
    ia->getScore()->calculerScore();
    QGraphicsTextItem* score = sceneCite->addText(QString("Score Actuel : %1").arg(ia->getScore()->calculerScore()));
    score->setDefaultTextColor(Theme::MARRON_FONCE);
    score->setFont(fontStats);
    score->setPos(-statsW/2 + (statsW - score->boundingRect().width())/2, -160 + 45);

    // --- 5. COLLECTION ---
    QGraphicsTextItem* stashTitle = sceneCite->addText("SA COLLECTION :");
    stashTitle->setDefaultTextColor(Theme::CUIVRE);
    stashTitle->setFont(fontSousTitre);
    stashTitle->setPos(-stashTitle->boundingRect().width() / 2, -50);

    // Paramètres Grille
    double cardW = 90; double cardH = 110; double space = 15;
    int cols = 6;
    double gridTotalWidth = (cols * cardW) + ((cols - 1) * space);
    double startX = -gridTotalWidth / 2;
    double currentX = startX;
    double currentY = 0;
    int count = 0;

    for (auto it = ia->begin(); it != ia->end(); ++it) {
        // Carte Fond
        QGraphicsRectItem* card = new QGraphicsRectItem(0, 0, cardW, cardH);
        card->setBrush(QBrush(Theme::BEIGE_CARTE)); // Au lieu de 14267535
        card->setPen(QPen(Theme::CUIVRE, 3));       // Bordure épaisse
        card->setPos(currentX, currentY);
        sceneCite->addItem(card);

        // Tuile
        Tuile* t = *it;
        TuileItem* item = new TuileItem((*it), -1);
        if (t->getNbHexagones() == 4) {
            item->setScale(0.45); // Plus petit pour rentrer (0.45 au lieu de 0.55)
            item->setPos(currentX + cardW/2, currentY + cardH/2 + 5);
        } else {
            item->setScale(0.55); // Taille normale pour les trios
            item->setPos(currentX + cardW/2 + 2, currentY + cardH/2 - 11);
        }

        item->setZValue(1);
        sceneCite->addItem(item);

        currentX += cardW + space;
        count++;
        if (count % cols == 0) {
            currentX = startX;
            currentY += cardH + space;
        }
    }

    // --- 6. BOUTON CONTINUER ---
    QPushButton* btnLocal = new QPushButton("CONTINUER");
    // On garde votre style CSS qui marche bien ici
    btnLocal->setStyleSheet(
        "QPushButton { "
        "   background-color: #dc8d55; color: white; border: 2px solid #b56d38; "
        "   border-radius: 8px; font-weight: bold; font-size: 16px; padding: 10px;"
        "}"
        "QPushButton:hover { background-color: #e89e6b; }"
        );
    btnLocal->setCursor(Qt::PointingHandCursor);
    btnLocal->setFixedWidth(220);

    connect(btnLocal, &QPushButton::clicked, this, &MainWindow::onContinuerIAClicked, Qt::QueuedConnection);
    QGraphicsProxyWidget* proxy = sceneCite->addWidget(btnLocal);
    proxy->setPos(-110, currentY + cardH + 40);
}

// =============================================================
// 8. FIN DE PARTIE
// =============================================================


void MainWindow::afficherFinDePartie() {
    // --- 1. CALCUL DES SCORES AVEC CLASSEMENT ---

    // Tri des joueurs par score (décroissant)
    vector<Joueur*> joueursClasses;
    for (auto it = Partie::getInstance().debutJoueurs(); it != Partie::getInstance().finJoueurs(); ++it) {
        joueursClasses.push_back(*it);
    }
    std::sort(joueursClasses.begin(), joueursClasses.end(), [](Joueur* a, Joueur* b) {
        return a->getScore()->calculerScore() > b->getScore()->calculerScore();
    });

    // Affichage avec médailles
    QString texteScores = "<div style='text-align: center; line-height: 180%;'>";
    QStringList medailles = {"🥇", "🥈", "🥉", "🎖️"};

    for (size_t i = 0; i < joueursClasses.size(); ++i) {
        Joueur* j = joueursClasses[i];
        QString couleur = (i == 0) ? "#E67E22" : "#734526"; // Orange pour le 1er
        QString bgColor = (i == 0) ? "rgba(230, 126, 34, 0.15)" : "rgba(115, 69, 38, 0.08)";

        texteScores += QString(
                           "<div style='background: %1; border-radius: 8px; padding: 12px; margin: 8px 0;'>"
                           "   <span style='font-size: 20px;'>%2</span> "
                           "   <span style='font-size: 18px; color: %3; font-weight: bold;'>%4</span> : "
                           "   <span style='font-size: 22px; font-weight: bold; color: %3;'>%5 pts</span>"
                           "</div>"
                           ).arg(bgColor)
                           .arg(medailles[std::min((int)i, 3)])
                           .arg(couleur)
                           .arg(QString::fromStdString(j->getNom()))
                           .arg(j->getScore()->calculerScore());
    }
    texteScores += "</div>";

    // --- 2. TEXTE DU VAINQUEUR AMÉLIORÉ ---
    vector<Joueur*> gagnants = Partie::getInstance().determinerGagnants();
    QString texteVainqueur;

    if (gagnants.size() == 1) {
        texteVainqueur = QString(
                             "<div style='text-align: center; margin: 30px 0;'>"
                             "   <div style='font-size: 48px; margin-bottom: 10px;'>👑</div>"
                             "   <div style='font-size: 18px; color: #734526; letter-spacing: 2px; margin-bottom: 8px;'>VICTOIRE DE</div>"
                             "   <div style='font-size: 32px; font-weight: bold; color: #dc8d55; "
                             "               text-shadow: 2px 2px 4px rgba(0,0,0,0.2);'>%1</div>"
                             "   <div style='font-size: 14px; color: #734526; margin-top: 10px; font-style: italic;'>"
                             "      L'architecte le plus prestigieux d'Akropolis !"
                             "   </div>"
                             "</div>"
                             ).arg(QString::fromStdString(gagnants[0]->getNom()).toUpper());
    } else {
        texteVainqueur = QString(
            "<div style='text-align: center; margin: 30px 0;'>"
            "   <div style='font-size: 48px; margin-bottom: 10px;'>🤝</div>"
            "   <div style='font-size: 28px; font-weight: bold; color: #dc8d55;'>ÉGALITÉ OLYMPIQUE !</div>"
            "   <div style='font-size: 14px; color: #734526; margin-top: 10px;'>Les architectes partagent la gloire</div>"
            "</div>"
            );
    }

    // --- 3. CRÉATION DE LA FENÊTRE PERSONNALISÉE (OVERLAY) ---
    QDialog overlay(this);
    overlay.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    overlay.setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout* layout = new QVBoxLayout(&overlay);

    QFrame* frame = new QFrame();
    frame->setProperty("class", "CadreConfig");
    frame->setMinimumWidth(500);
    frame->setMaximumWidth(600);

    QVBoxLayout* frameLayout = new QVBoxLayout(frame);
    frameLayout->setContentsMargins(40, 40, 40, 40);
    frameLayout->setSpacing(20);

    // Titre
    QLabel* lblTitre = new QLabel("FIN DE PARTIE", frame);
    QFont fontTitre;
    fontTitre.setPixelSize(30);
    fontTitre.setBold(true);
    lblTitre->setFont(fontTitre);
    QPalette pal = lblTitre->palette();
    pal.setColor(QPalette::WindowText, Theme::ORANGE);
    lblTitre->setPalette(pal);
    lblTitre->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(lblTitre);

    // Vainqueur (AVANT les scores pour plus d'impact)
    QLabel* lblVainqueur = new QLabel(texteVainqueur, frame);
    lblVainqueur->setObjectName("vainqueur");
    lblVainqueur->setAlignment(Qt::AlignCenter);
    lblVainqueur->setTextFormat(Qt::RichText);
    frameLayout->addWidget(lblVainqueur);

    // Liste des scores (classement avec médailles)
    QLabel* lblScores = new QLabel(texteScores, frame);
    lblScores->setAlignment(Qt::AlignCenter);
    lblScores->setTextFormat(Qt::RichText);
    frameLayout->addWidget(lblScores);

    // Boutons
    QHBoxLayout* btnLayout = new QHBoxLayout();

    QPushButton* btnMenu = new QPushButton("MENU PRINCIPAL", frame);
    btnMenu->setProperty("class", "BoutonMenu");
    btnMenu->setCursor(Qt::PointingHandCursor);

    QPushButton* btnQuitter = new QPushButton("QUITTER", frame);
    btnQuitter->setProperty("class", "BoutonMenu BoutonDanger");
    btnQuitter->setCursor(Qt::PointingHandCursor);

    btnLayout->addWidget(btnMenu);
    btnLayout->addWidget(btnQuitter);
    frameLayout->addLayout(btnLayout);

    layout->addWidget(frame);

    // --- 4. CONNEXIONS ---
    connect(btnMenu, &QPushButton::clicked, &overlay, &QDialog::accept);
    connect(btnQuitter, &QPushButton::clicked, this, &MainWindow::quitterJeu);

    // --- 5. AFFICHAGE MODAL ---
    if (overlay.exec() == QDialog::Accepted) {
        stackedWidget->setCurrentWidget(pageMenuPrincipal);
        sceneCite->clear();
        sceneChantier->clear();
    }
}



