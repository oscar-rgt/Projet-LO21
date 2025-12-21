#include "mainwindow.h"
#include "cite.h"
#include "tuileitem.h"
#include "save.h"
#include "score.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGraphicsPolygonItem>
#include <cmath>
#include <QTextEdit>
#include<QApplication>
#include <QDialog>
#include <QMouseEvent>
#include <QGraphicsProxyWidget>
#include <QOperatingSystemVersion>
#include <QFontDatabase>

using namespace std;

//palette de couleurs du thème Akropolis
namespace Theme {
const QColor ORANGE(220, 141, 85);       // #dc8d55
const QColor MARRON_FONCE(78, 46, 30);    // #4E2E1E
const QColor CUIVRE(115, 69, 38);         // #734526
const QColor BEIGE_CARTE(217, 180, 143);  // #D9B48F
const QColor ROUGE_ERREUR(192, 57, 43); //#c0392b
}

//constantes taille tuiles
const double TUILE_TAILLE = 30.0;
const double COEFF_X = 1.53;
const double OFFSET_Y = sqrt(3.0) * TUILE_TAILLE;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), indexTuileSelectionnee(-1), rotationCompteur(0), inversionEtat(false), previewActive(false), previewX(0), previewY(0), previewZ(0), affichageResultatIA(false)
{

    setWindowTitle("Akropolis");
    resize(1024, 768);

    //permet de capturer les évenements clavier et souris
    setFocusPolicy(Qt::StrongFocus);

    QString styleGlobal = R"(
        /* 1. Fond général*/
        QMainWindow, QWidget {
            background-color: #FAF8EF;
            color: #2C3E50; /* Texte Gris Foncé (Lisible) */
            font-family: 'Segoe UI', Arial, sans-serif;
        }

        /* 2. Boutons*/
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

        /* 3. Champs de texte*/
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

        /* 4. GroupBox*/
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

        /* 5. Checkbox/Radio bouton*/
        QCheckBox, QRadioButton {
            spacing: 8px;
            font-size: 13px;
            color: #2C3E50;
        }

        /*L'indicateur*/
        QCheckBox::indicator, QRadioButton::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #BDC3C7;
            background-color: white; /* Fond blanc bien visible */
            border-radius: 4px; /* Coins arrondis pour checkbox */
        }

        /*Quand on passe la souris dessus*/
        QCheckBox::indicator:hover, QRadioButton::indicator:hover {
            border-color: #dc8d55;
        }

        /*Quand c'est COCHÉ */
        QCheckBox::indicator:checked, QRadioButton::indicator:checked {
            background-color: #dc8d55; /* Fond Orange */
            border-color: #dc8d55;
            image: none; /* On remplit juste de couleur pour faire simple et visible */
        }

        /* Arrondi spécifique pour les boutons radio */
        QRadioButton::indicator {
            border-radius: 10px; /* Cercle parfait */
        }

        /* 6. Cadre spécifique*/

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

        /* Cadre unifié */
        .CadreConfig {
            border: 2px solid #dc8d55;
            border-radius: 15px;
            background-color: #FAF8EF; /* Fond semi-opaque pour lisibilité si besoin, ou transparent */
            /* Si vous voulez 100% transparent comme avant, mettez : background-color: transparent; */
        }

        /* 7. labels d'info (Score, Tour, Piles) */
        .InfoLabel {
            background-color: rgba(255, 255, 255, 0.6); /* Fond blanc semi-transparent */
            border: 2px solid #dc8d55;                  /* Bordure Orange */
            border-radius: 10px;                        /* Coins ronds */
            padding: 8px;                              /* Espace intérieur */
            font-size: 13px;
            color: #4E2E1E;                             /* Texte Marron foncé */
        }
    )";

    //application du style à toute l'application
    this->setStyleSheet(styleGlobal);

    //StackedWidget pour gérer les différentes pages
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    initialiserPageMenuPrincipal();
    initialiserPageRegles();
    initialiserPageJeu();
    initialiserPageConfiguration();

    //Affichage menu principal par défaut
    stackedWidget->setCurrentWidget(pageMenuPrincipal);

    pagePrecedente = nullptr;
}

MainWindow::~MainWindow() {}

void MainWindow::initialiserPageMenuPrincipal()
{
    pageMenuPrincipal = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(pageMenuPrincipal);

    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30);
    layout->setContentsMargins(50, 50, 50, 50);

	//logo Akropolis
    QLabel *labelLogo = new QLabel(pageMenuPrincipal);
    labelLogo->setStyleSheet("border: none; background-color: transparent; margin-bottom: 10px;");
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

	//boutons du menu principal :
    QVBoxLayout *layoutBoutons = new QVBoxLayout();
    layoutBoutons->setSpacing(20);

    //bouton jouer
    QPushButton *btnJouer = new QPushButton("JOUER UNE PARTIE", pageMenuPrincipal);
    btnJouer->setProperty("class", "BoutonMenu"); 
    btnJouer->setCursor(Qt::PointingHandCursor);
    connect(btnJouer, &QPushButton::clicked, this, &MainWindow::afficherMenuConfig);
    layoutBoutons->addWidget(btnJouer, 0, Qt::AlignCenter);

    //bouton règles
    QPushButton *btnRegles = new QPushButton("RÈGLES DU JEU", pageMenuPrincipal);
    btnRegles->setProperty("class", "BoutonMenu");
    btnRegles->setCursor(Qt::PointingHandCursor);
    connect(btnRegles, &QPushButton::clicked, this, &MainWindow::afficherMenuRegles);
    layoutBoutons->addWidget(btnRegles, 0, Qt::AlignCenter);

    //bouton charger
    QPushButton *btnCharger = new QPushButton("CHARGER UNE PARTIE", pageMenuPrincipal);
    btnCharger->setProperty("class", "BoutonMenu");
    btnCharger->setCursor(Qt::PointingHandCursor);
    connect(btnCharger, &QPushButton::clicked, this, &MainWindow::onChargerPartieClicked);
    layoutBoutons->addWidget(btnCharger, 0, Qt::AlignCenter);

    //bouton quitter
    QPushButton *btnQuitter = new QPushButton("QUITTER", pageMenuPrincipal);
    btnQuitter->setProperty("class", "BoutonMenu BoutonDanger"); //bouton menu rouge
    btnQuitter->setCursor(Qt::PointingHandCursor);
    connect(btnQuitter, &QPushButton::clicked, this, &MainWindow::quitterJeu);
    layoutBoutons->addWidget(btnQuitter, 0, Qt::AlignCenter);

    layout->addLayout(layoutBoutons);

    //crédits
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
    layout->setContentsMargins(20, 20, 20, 20); 
    layout->setSpacing(10); 

    //titre
    QLabel *titleLabel = new QLabel("RÈGLES D'AKROPOLIS", pageRegles);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #dc8d55; margin-bottom: 20px;");
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);

	//zone de scroll pour le texte des règles
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

    //widget interne pour contenir le texte des règles
    QWidget *textContainer = new QWidget();
    QVBoxLayout *textContainerLayout = new QVBoxLayout(textContainer);
    textContainerLayout->setContentsMargins(20, 20, 20, 20); 
    QWidget *centerWidget = new QWidget(); //on crée un autre widget pour centrer le texte
    centerWidget->setFixedWidth(1400);
    centerWidget->setFixedHeight(1000);
    QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    QTextEdit *textRegles = new QTextEdit(centerWidget); //et un widget pour le texte
    textRegles->setReadOnly(true);
    textRegles->setFrameShape(QFrame::NoFrame);
    textRegles->setStyleSheet("background-color: transparent; color: #2C3E50; font-size: 16px;");
    textRegles->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //on remplit le widget texte :
    QString contenuHtml = R"(
        <div style='font-size: 16px; line-height: 1.4;'>
            <h3 style='color: #dc8d55;'>1. LE BUT</h3>
            <p style='text-align: justify;'>
                Construisez la cité la plus prestigieuse. Les points sont marqués grâce aux Quartiers multipliés par les Places correspondantes.
            </p>
            <h3 style='color: #dc8d55;'>2. LES QUARTIERS</h3>
            <p style='text-align: justify;'>
                <b>- Habitation (BLEU) :</b> 1 point pour chaque quartier habitation adjacent à un autre. (Ne compte que pour votre plus grand groupe d'habitations)<br>
                <b>- Marché (JAUNE) :</b> 1 point si votre marché n'est adjacent à aucun autre.<br>
                <b>- Caserne (ROUGE) :</b> 1 point si votre caserne n'est pas complètement entourée par d'autres hexagones.<br>
                <b>- Temple (VIOLET) :</b> 1 point si votre temple est complètement entouré par d'autres hexagones.<br>
                <b>- Jardin (VERT) :</b> 1 point pour chaque jardin posé sans condition.<br>
                <b>- Carrière (GRIS) :</b> Permet d'agrandir votre cité mais ne donne pas de points.
            </p>
            <h3 style='color: #dc8d55;'>3. LES PLACES</h3>
            <p style='text-align: justify;'>
                Les places de chaque type vous permettent de multiplier vos points en fonction du chiffre qui est écrit dessus.<br>
                Par exemple, un hexagone bleu marqué par ★★ est une place Habitation à 2 étoiles. Elle multiplie donc par 2 les points gagnés par vos quartiers Habitation.<br>
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
                La hauteur d'un hexagone est indiqué par un chiffre inscrit dessus.<br>
                À vous de trouver le bon équilibre pour devenir le plus prestigieux des architectes !
            </p>
        </div>
    )";
    textRegles->setHtml(contenuHtml);
    centerLayout->addWidget(textRegles);

    //ajoute le widget centré au conteneur principal
    textContainerLayout->addWidget(centerWidget, 0, Qt::AlignCenter);
    scrollArea->setWidget(textContainer);
    layout->addWidget(scrollArea, 1); //1 permet à la scrollArea de prendre tout l'espace disponible

    //bouton retour
    QPushButton *btnRetour = new QPushButton("RETOUR", pageRegles);
    btnRetour->setFixedSize(150, 40);
    btnRetour->setStyleSheet("font-weight: bold; background-color: #dc8d55; color: white; border-radius: 5px;");
    layout->addWidget(btnRetour, 0, Qt::AlignCenter);
    connect(btnRetour, &QPushButton::clicked, [this]() {

        //Cas 1 : on vient du jeu (pause)
        if (pagePrecedente == pageJeu) {
            stackedWidget->setCurrentWidget(pageJeu); //on affiche le jeu
            onReglagesClicked();                      //on rouvre le pop-up Réglages par-dessus
        }

        //Cas 2 : on vient du menu
        else {
            stackedWidget->setCurrentWidget(pageMenuPrincipal);
        }
    });
    stackedWidget->addWidget(pageRegles);
}

void MainWindow::initialiserPageConfiguration()
{
	//creation de la page de configuration
    pageConfig = new QWidget();
    QVBoxLayout *layoutPage = new QVBoxLayout(pageConfig);
    layoutPage->setContentsMargins(0, 0, 0, 0); //pour occuper tout l'espace

    //zone de scroll
    QScrollArea *scrollArea = new QScrollArea(pageConfig);
    scrollArea->setWidgetResizable(true);       //le contenu s'adapte à la largeur
    scrollArea->setFrameShape(QFrame::NoFrame); //enlève les bordures
    scrollArea->setStyleSheet("background: transparent;");
    QWidget *scrollContent = new QWidget();
    scrollContent->setStyleSheet("background: transparent;");
    QVBoxLayout *layoutScroll = new QVBoxLayout(scrollContent);
    layoutScroll->setAlignment(Qt::AlignCenter);
    layoutScroll->setContentsMargins(20, 20, 20, 20);

	//Cadre principal
    QFrame *cadreConfig = new QFrame(scrollContent);
    cadreConfig->setProperty("class", "CadreConfig");
    cadreConfig->setFixedWidth(600);
    QVBoxLayout *layoutCadre = new QVBoxLayout(cadreConfig);
    layoutCadre->setSpacing(15);
    layoutCadre->setContentsMargins(30, 30, 30, 30);

    //Contenu du formulaire :

    //titre
    QLabel *titre = new QLabel("CONFIGURATION DE LA PARTIE", cadreConfig);
    titre->setStyleSheet("font-size: 22px; font-weight: bold; color: #dc8d55; margin-bottom: 10px;");
    titre->setAlignment(Qt::AlignCenter);
    layoutCadre->addWidget(titre);

    //nb joueurs
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

    //noms des joueurs
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

    //IA
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

    //mode de jeu
    QGroupBox *boxMode = new QGroupBox("Mode de jeu", cadreConfig);
    QVBoxLayout *layoutMode = new QVBoxLayout(boxMode);
    checkModeAugmente = new QCheckBox("Tuiles Cité Augmentée", boxMode);
    layoutMode->addWidget(checkModeAugmente);
    layoutCadre->addWidget(boxMode);

    //variantes
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

    //boutons
    QHBoxLayout *layoutAction = new QHBoxLayout();
    layoutAction->setContentsMargins(0, 20, 0, 0);
    QPushButton *btnRetour = new QPushButton("ANNULER", cadreConfig);
    QPushButton *btnValider = new QPushButton("JOUER", cadreConfig);

    //a cause du scrollbar, on doit forcer le style des boutons
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

    //assemblage de la page :

    layoutScroll->addWidget(cadreConfig);
    scrollArea->setWidget(scrollContent);
    layoutPage->addWidget(scrollArea);

    //connexions
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

    //zone gauche = cite
    sceneCite = new QGraphicsScene(this);
    viewCite = new QGraphicsView(sceneCite);
    viewCite->setRenderHint(QPainter::Antialiasing);
    viewCite->viewport()->installEventFilter(this);
    mainLayout->addWidget(viewCite, 2);

	//zone droite = chantier + boutons
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

    //bouton réglages (haut à gauche)
    btnReglages = new QPushButton("Paramètres", pageJeu);
    btnReglages->move(20, 20);
    btnReglages->setCursor(Qt::PointingHandCursor);
    connect(btnReglages, &QPushButton::clicked, this, &MainWindow::onReglagesClicked);
    btnReglages->raise(); //on le laisse au premier plan
    stackedWidget->addWidget(pageJeu);

	//label chantier
    QLabel *labelChantier = new QLabel("--- CHANTIER ---", pageJeu);
    labelChantier->setAlignment(Qt::AlignCenter);
    labelChantier->setStyleSheet("font-size: 16px; font-weight: bold; color: #dc8d55; margin-top: 20px; margin-bottom: 5px;");
    sideLayout->addWidget(labelChantier);

    //création de la scène et de la vue pour le chantier
    sceneChantier = new QGraphicsScene(this);
    viewChantier = new QGraphicsView(sceneChantier);
    viewChantier->setFixedWidth(200); 
    sideLayout->addWidget(viewChantier);
    viewChantier->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    //on ajoute le layout latéral au layout principal
    mainLayout->addLayout(sideLayout, 1); //prend 1/3 de la largeur

    stackedWidget->addWidget(pageJeu);

	//label piles restantes
    labelPilesRestantes = new QLabel("Piles : -", pageJeu);
    labelPilesRestantes->setProperty("class", "InfoLabel");
    labelPilesRestantes->setAlignment(Qt::AlignCenter);
    sideLayout->addWidget(labelPilesRestantes);
}

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
    resetConfiguration(); //nettoie la page
    stackedWidget->setCurrentWidget(pageConfig); 
}

void MainWindow::quitterJeu()
{
    QApplication::quit();
}

void MainWindow::onReglagesClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Pause");
    dialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.setFixedSize(300, 350);

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

    QString styleTaille = "font-size: 14px;"; 

    //bouton reprise
    QPushButton* btnReprendre = new QPushButton("REPRENDRE");
    btnReprendre->setProperty("class", "BoutonMenu");
    btnReprendre->setStyleSheet(styleTaille);
    connect(btnReprendre, &QPushButton::clicked, &dialog, &QDialog::accept);
    l->addWidget(btnReprendre);

    //bouton règles
    QPushButton* btnRegles = new QPushButton("RÈGLES");
    btnRegles->setProperty("class", "BoutonMenu");
    btnRegles->setStyleSheet(styleTaille);
    connect(btnRegles, &QPushButton::clicked, [this, &dialog]() {
		pagePrecedente = pageJeu;     //on mémorie jeu comme page précédente
        stackedWidget->setCurrentWidget(pageRegles);
        dialog.accept(); //ferme le pop-up
    });
    l->addWidget(btnRegles);

    //bouton sauvegarder et quitter
    QPushButton* btnSave = new QPushButton("SAUVEGARDER ET QUITTER");
    btnSave->setProperty("class", "BoutonMenu");
    btnSave->setStyleSheet(styleTaille);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSauvegarderClicked);
    l->addWidget(btnSave);

	//bouton sauvegarder sans quitter
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
	//pop up de confirmation
    QMessageBox::StandardButton reponse = QMessageBox::question(
        this,
        "Sauvegarder & Quitter",
        "Voulez-vous sauvegarder et quitter la partie ?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reponse == QMessageBox::Yes) {

        bool succes = SaveManager::sauvegarder(Partie::getInstance(), "save.txt");

        if (succes) {
            QMessageBox::information(this, "Succès", "Partie sauvegardée avec succès dans 'save.txt' !");
            QApplication::quit();
        }
        else {
            QMessageBox::critical(this, "Erreur Critique", "La sauvegarde a échoué.\nImpossible d'écrire le fichier.");
			//on ne quitte pas le jeu pour laisser le choix au joueur de continuer sa partie
        }
    }
	//si le joueur répond non, on ne fait rien et on revient au jeu
}


void MainWindow::mettreAJourVisibiliteConfig()
{
    int nbJoueurs = groupeNbJoueurs->checkedId(); //récupère l'id (1, 2, 3, 4)

    //Gestion ia
    if (nbJoueurs == 1) {
        groupeIA->setVisible(true);
        //en mode solo on force le nom du joueur 2 à être IA
        champsNomsJoueurs[1]->setText("Illustre Architecte");
        champsNomsJoueurs[1]->setEnabled(false); //pour ne pas pouvoir le renommer
    } else {
        groupeIA->setVisible(false);
        champsNomsJoueurs[1]->setEnabled(true);
        if (champsNomsJoueurs[1]->text() == "Illustre Architecte") champsNomsJoueurs[1]->clear();
    }

    //gestion champs de noms
    //on affiche autant de champs que de joueurs

    for (int i = 0; i < 4; ++i) {
        //le champ est visible si son index < nbJoueurs
        //exception: en mode solo, on veut juste le nom du joueur 1, l'IA est gérée à part
        bool visible = (i < nbJoueurs);
        champsNomsJoueurs[i]->setVisible(visible);
    }
}

void MainWindow::validerConfiguration()
{
    int nbJoueurs = groupeNbJoueurs->checkedId();

    vector<string> noms;

    for (int i = 0; i < nbJoueurs; ++i) {
       
        QString nomSaisi = champsNomsJoueurs[i]->text().trimmed(); //.trimmed() enlève les espaces au début et à la fin

        if (nomSaisi.isEmpty()) {
            QMessageBox::warning(this, "Configuration incomplète",
                                 QString("Le nom du Joueur %1 est obligatoire !").arg(i + 1));

			champsNomsJoueurs[i]->setFocus(); //on met le curseur dans le champ concerné

            return;
        }

        noms.push_back(nomSaisi.toStdString());
    }

    //mode IA
    int niveauIA = 0;
    if (nbJoueurs == 1) {
        niveauIA = groupeNiveauIA->checkedId();
    }

    //Variantes
    array<bool, 5> variantes;
    for (int i = 0; i < 5; ++i) {
        variantes[i] = checkBoxesVariantes[i]->isChecked();
    }

    //mode tuiles
    TuileCite mode = checkModeAugmente->isChecked() ? TuileCite::AUGMENTE : TuileCite::STANDARD;

	//lancement de la partie
    try {
        Partie::getInstance().initialiser(nbJoueurs, noms, mode, variantes, niveauIA);

        afficherInfoRaccourcisClavier();

        stackedWidget->setCurrentWidget(pageJeu);
        mettreAJourInterface(); 

    } catch (const exception &e) {
        QMessageBox::critical(this, "Erreur", e.what());
    }
}



void MainWindow::resetConfiguration()
{
    //on remet 2 joueurs par défaut
    if (QAbstractButton *btn = groupeNbJoueurs->button(2)) {
        btn->setChecked(true);
    }

    //vider les noms
    for (QLineEdit *champ : champsNomsJoueurs) {
        champ->clear();
    }

    //on remet l'IA au niveau 1
    if (QAbstractButton *btn = groupeNiveauIA->button(1)) {
        btn->setChecked(true);
    }

    //on décoche le mode augmenté
    checkModeAugmente->setChecked(false);

    //on décoche toutes les variantes
    for (QCheckBox *cb : checkBoxesVariantes) {
        cb->setChecked(false);
    }

    mettreAJourVisibiliteConfig();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
	//on ne regarde que les clics sur la vue de la cité
    if (watched == viewCite->viewport() && event->type() == QEvent::MouseButtonPress) {

		//si on est en train d'afficher le résultat de l'IA, on ignore les clics
        if (affichageResultatIA) {
            return false;
        }
        //reaction au bouton gauche de la souris
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
			//conversion position en pixel ecran -> position en scene du jeu
            QPointF scenePos = viewCite->mapToScene(mouseEvent->pos());
			//on envoie la position au gestionnaire de clics
            traiterClicPlateau(scenePos);
			return true; //indique que l'événement a été traité
        }
    }
	//pour les autres événements, on laisse le traitement par défaut
    return QMainWindow::eventFilter(watched, event);
}



void MainWindow::traiterClicPlateau(QPointF positionScene)
{
    if (indexTuileSelectionnee == -1) {
        QMessageBox::warning(this, "Attention", "Sélectionnez d'abord une tuile dans le chantier.");
        return;
    }

	//dimensions cases hexagonales
    double w = COEFF_X * TUILE_TAILLE;
    double h = sqrt(3.0) * TUILE_TAILLE;

	//on divise la coordonnée x par la largeur d'une case d'hexagone pour avoir une estimation de la case cliquée
    int x_est = round(positionScene.x() / w);

    double minDistance = 1000000.0;
    int bestX = 0, bestY = 0;

	//recherche du centre d'hexagone le plus proche du clic (meilleur candidat)
    for (int i = -2; i <= 2; ++i) {
        for (int j = -3; j <= 3; ++j) {
            int tx = x_est + i; //candidat potentiel
            double offset = abs(tx % 2) * (h / 2.0); //les colonnes impaires sont décalées verticalement d'un demi-hexagone (h/2).
			int ty = round((positionScene.y() - offset) / -h) + j; //on ajuste y avec le décalage

            //recalcul du centre du candidat 
            double cx = tx * w;
            double cy = ty * -h + offset;

            //on compare la distance
            double dist = pow(positionScene.x() - cx, 2) + pow(positionScene.y() - cy, 2);
            if (dist < minDistance) {
                minDistance = dist;
                bestX = tx;
                bestY = ty;
            }
        }
    }

	//on trouve la hauteur où il faut placer la tuile
    int autoZ = 0;
    Joueur* j = Partie::getInstance().getJoueurActuel();
    Cite* cite = j->getCite();
    int hMax = cite->getHauteurMax();
    for (int z = hMax + 1; z >= 0; --z) {
        //si on trouve une tuile à l'étage z, on doit se poser à z+1
        if (!cite->estLibre({ bestX, bestY, z })) {
            autoZ = z + 1;
            break;
        }
    }

    //mise à jour de l'état de prévisualisation
    previewX = bestX;
    previewY = bestY;
    previewZ = autoZ;
    previewActive = true;


    mettreAJourInterface();
}


void MainWindow::selectionnerTuileChantier(int index)
{
    indexTuileSelectionnee = index;

    previewActive = false;

    //on redessine pour mettre à jour l'affichage de la sélection (tuile foncée dans le chantier)
    dessinerChantier();
    mettreAJourInterface();
}


void MainWindow::onRotationClicked() {
    //verifs de securité
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

    rotationCompteur = (rotationCompteur + 1) % 3; 

    mettreAJourInterface();
}

void MainWindow::onInversionClicked() {
	//verifs de securité
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

    inversionEtat = !inversionEtat; 

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
		//reset tuile état initial
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
		Joueur* joueurSuivant = Partie::getInstance().getJoueurActuel(); //on est déjà passé au joueur suivant grace à actionPlacerTuile

        if (joueurSuivant->estIA()) {
            dernierIndexIA = Partie::getInstance().jouerTourIA();

            affichageResultatIA = true;
        }

        // Mise à jour de l'interface (affichera soit le tour de l'humain suivant, soit l'écran IA)
        mettreAJourInterface();


    } 
    //gestion des erreurs :
    catch (const CiteException& e) {
        QMessageBox::warning(this, "Construction Impossible", e.what());
    }
    catch (const PartieException& e) {
        QMessageBox::warning(this, "Action Interdite", e.what());
    }
    catch (const exception& e) {
        QMessageBox::critical(this, "Erreur", e.what());
    }
}

void MainWindow::onChargerPartieClicked() {
    try {
        bool succes = SaveManager::charger(Partie::getInstance(), "save.txt");

        if (succes) {
            QMessageBox::information(this, "Chargement terminé", "Votre partie a été restaurée avec succès !");

            //on lance le jeu
            stackedWidget->setCurrentWidget(pageJeu);
            mettreAJourInterface();
        }
        else {
            QMessageBox::warning(this, "Echec du chargement",
                                 "Impossible de charger la sauvegarde.\n"
                                 "Le fichier 'save.txt' est introuvable ou illisible.");
        }
    }
    catch (const exception& e) {
        //en cas de gros crash pendant la lecture
        QMessageBox::critical(this, "Erreur Critique",
                              QString("Une erreur est survenue pendant le chargement :\n%1").arg(e.what()));
    }
}


void MainWindow::onContinuerIAClicked() //bouton "Continuer" sur la page du tour de l'ia (en bas)
{
    //désactive le mode IA
    affichageResultatIA = false;

    if (Partie::getInstance().estFinDePartie()) {
        afficherFinDePartie();
    } else {
        mettreAJourInterface();
    }
}

void MainWindow::mettreAJourInterface()
{
    int pilesRestantes = Partie::getInstance().getNbPiles() - Partie::getInstance().getIndexPileActuelle();
    if (pilesRestantes < 0) pilesRestantes = 0; //securité

    //détection du support des emojis :
    bool supportEmojis = false;
    QFontDatabase fontDb;
    QStringList families = fontDb.families();
    QStringList emojiFonts = {
        "Noto Color Emoji", "Twitter Color Emoji", "Segoe UI Emoji",
        "JoyPixels", "OpenMoji", "Apple Color Emoji", "Symbola"
    };
    for (const QString &font : emojiFonts) {
        if (families.contains(font)) {
            supportEmojis = true;
            break;
        }
    }

    //choix des emojis en fonction du nb de piles restantes :
    QString couleurPile = (pilesRestantes <= 2) ? Theme::ROUGE_ERREUR.name() : Theme::ORANGE.name();
    QString iconePile = supportEmojis ? ((pilesRestantes <= 2) ? "⚠️" : "📚") : ((pilesRestantes <= 2) ? "!" : "P");

    QString couleurTitre = Theme::CUIVRE.name();

    labelPilesRestantes->setText(QString(
                                     "<div style='text-align: center; padding: 8px;'>"
                                     "   <span style='font-size: 12px; color: #734526;'>PILES RESTANTES</span><br>"
                                     "   <span style='font-family: \"Noto Color Emoji\", \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Twitter Color Emoji\", sans-serif; font-size: 24px;'>%1</span> "
                                     "   <span style='font-size: 28px; font-weight: bold; color: %2;'>%3</span>"
                                     "</div>"
                                     ).arg(iconePile).arg(couleurPile).arg(pilesRestantes));

    if (affichageResultatIA) { //si on est en mode ia
        btnRotation->hide();
        btnInversion->hide();
        btnValidation->hide();
        btnReglages->raise();

        //on cherche l'ia dans la liste des joueurs :
        IA* iaTrouvee = nullptr;
        auto it = Partie::getInstance().debutJoueurs();
        while(it != Partie::getInstance().finJoueurs()) {
            if ((*it)->estIA()) {
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

	//sinon mode joueur humain :
    else {
        Joueur* j = Partie::getInstance().getJoueurActuel();

		//affichage des infos du joueur :
        QString symbolePierres = supportEmojis ? "💎" : "P";
        QString symboleScore = supportEmojis ? "⭐" : "S";

        htmlTexteJoueur = QString(
                              "<div style='line-height: 150%; padding: 10px;'>"
                              "   <div style='text-align: center; margin-bottom: 15px;'>"
                              "      <span style='font-size: 13px; color: #734526; letter-spacing: 1px;'>TOUR DE</span><br>"
                              "      <span style='font-size: 22px; font-weight: bold; color: #dc8d55; text-shadow: 1px 1px 2px rgba(0,0,0,0.1);'>%1</span>"
                              "   </div>"
                              "   <div style='background: linear-gradient(135deg, rgba(220,141,85,0.15), rgba(220,141,85,0.05)); "
                              "               border-radius: 8px; padding: 12px; margin-top: 10px;'>"
                              "      <div style='display: flex; justify-content: space-between; margin-bottom: 8px;'>"
                              "         <span style='font-size: 14px; color: #734526;'>%2 Pierres</span>"
                              "         <span style='font-size: 18px; font-weight: bold; color: #dc8d55;'>%3</span>"
                              "      </div>"
                              "      <div style='display: flex; justify-content: space-between;'>"
                              "         <span style='font-size: 14px; color: #734526;'>%4 Score</span>"
                              "         <span style='font-size: 18px; font-weight: bold; color: #e67e22;'>%5</span>"
                              "      </div>"
                              "   </div>"
                              "</div>"
                              ).arg(QString::fromStdString(j->getNom()))
                              .arg(symbolePierres)
                              .arg(j->getPierres())
                              .arg(symboleScore)
                              .arg(j->getScore()->calculerScore());

        labelInfoJoueur->setText(htmlTexteJoueur);

        //activation des boutons utiles pour le joueur :
        btnRotation->setEnabled(indexTuileSelectionnee!=-1);
        btnRotation->show();
        btnInversion->setEnabled(indexTuileSelectionnee!=-1);
        btnInversion->show();
        btnValidation->setEnabled(previewActive);
        btnValidation->show();

		//dessin de la cité et de la prévisualisation que pour le joueur
        dessinerCite(j);
        dessinerPreview(j);
    }

    //dans les deux cas (humain/ia) on dessine le chantier
    dessinerChantier();
}


void MainWindow::dessinerCite(Joueur* joueur) {
    sceneCite->clear();
    Cite* cite = joueur->getCite();
    double taille = 30.0;

    for (auto it = cite->begin(); it != cite->end(); ++it) {
		//pour chaque case cite on recupère la position et l'hexagone
        Coord pos = it->first;
        Hexagone* hex = it->second;

		//propriétés de l'hexagone pour notre interface graphique
        double w = COEFF_X * taille;
        double h = (sqrt(3.) * taille);
        double pixelX = pos.x * w;
		double pixelY = pos.y * -h + abs((pos.x % 2)) * (h / 2); //calcul compliqué dû au décalage des colonnes impaires de la grille hexagonale


        QColor couleur = TuileItem::getTypeColor(hex->getType());
        int nbEtoiles = hex->estPlace() ? hex->getEtoiles() : 0;

        //création de l'hexagone
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

		//ajout de l'hexa à la scène
        sceneCite->addItem(item);

    }
}


void MainWindow::dessinerPreview(Joueur* j) {
    if (!previewActive || indexTuileSelectionnee == -1) return;

    // Récupération de la tuile sélectionnée
    const Chantier& chantier = Partie::getInstance().getChantier();
    auto it = chantier.begin(); 
    for(int i =0; i < indexTuileSelectionnee; i++) ++it;
    Tuile* t = *it;

    //création du "fantome" en utilisant TuileItem
    //on passe -1 en index car c'est un objet temporaire hors chantier
    TuileItem* ghost = new TuileItem(t, -1, TUILE_TAILLE);
    ghost->setSelection(true);
    ghost->setZValue(1000); // Toujours au-dessus de tout


	//caractéristiques de la grille hexagonale
    double taille = TUILE_TAILLE;
    double w = COEFF_X * taille;
    double h = sqrt(3.) * taille;

    //conversion des attributs hexagonaux aux valeurs de la scene
    double sceneX = previewX * w;
    double sceneY = previewY * -h + abs((previewX % 2)) * (h / 2);

    //on trouve le plus haut hexagone en dessous de la preview pour l'afficher au dessus
    int hmax = 0; //par défaut au sol
    Cite* cite = j->getCite();
    // On scanne les étages pour trouver la tuile la plus haute
    for (int z = 0; z < 20; z++) {
        if (!cite->estLibre({previewX, previewY, z})) {
            hmax = z + 1; //on se place donc à l'étage juste au-dessus
            break;       
        }
    }

    previewZ = hmax; //la preview sera au dessus de la tuile de hauteur max (hmax = z+1)

    //calcul pour que le pivot de la tuile (hexagone 0) soit celui sous la souris
    double sensInversion = (t->getInversion() ? 1.0 : -1.0) * w;
    double centrageX = -(sensInversion / 3.0); 

    //placement final
    if(hmax == 0) ghost->setPos(sceneX - centrageX, sceneY); //si la tuile est au sol on la pose sans décalage y
    else ghost->setPos(sceneX - centrageX, sceneY - 11.0); //sinon on le décale de la hauteur d'une tuile vers le haut

    //ajout de la preview à la scène
    sceneCite->addItem(ghost);
}


void MainWindow::dessinerChantier()
{
    sceneChantier->clear(); //on vide l'ancienne scène

    const Chantier& chantier = Partie::getInstance().getChantier();
    int index = 0;
    //valeurs arbitraires mises manuellement pour que le positionnement des hexas sur le chantier soit joli
    double yPos = 40; 
    double rayon = 20.0; 
    double xPos = 70; 

    
    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        Tuile* tuile = *it;

        TuileItem* item = new TuileItem(tuile, index, rayon);
        if (index == indexTuileSelectionnee) {
            item->setSelection(true); //si la tuile est selectionnée on l'assombrie
        } else {
            item->setSelection(false);
        }
        item->setPos(xPos, yPos); 
        yPos += 120; //espace entre les tuiles

        sceneChantier->addItem(item); //ajout de la tuile au chantier graphiquement

        //connexion clique/selection tuile
        connect(item, &TuileItem::clicked, this, [this, index]() {
            selectionnerTuileChantier(index);
        });


        //affichage du prix à droite de la tuile :
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

    //ajustement zone de scroll
    sceneChantier->setSceneRect(0, 0, 180, yPos);
}


void MainWindow::dessinerInterfaceIA(IA* ia) {
    sceneCite->clear();

    //reglage des polices
    QFont fontTitre; fontTitre.setPixelSize(24); fontTitre.setBold(true);
    QFont fontTexte; fontTexte.setPixelSize(16);
    QFont fontSousTitre; fontSousTitre.setPixelSize(18); fontSousTitre.setBold(true);
    QFont fontStats; fontStats.setPixelSize(16); fontStats.setBold(true);

    //titre
    QGraphicsTextItem* titre = sceneCite->addText("L'ILLUSTRE ARCHITECTE A JOUÉ");
    titre->setDefaultTextColor(Theme::ORANGE); // Au lieu de 14454101
    titre->setFont(fontTitre);
    titre->setPos(-titre->boundingRect().width() / 2, -260);

    //infos sur ce qu'a joué l'ia
    QString actionText = QString("Il a pris la tuile n°%1 du chantier.").arg(dernierIndexIA + 1);
    QGraphicsTextItem* info = sceneCite->addText(actionText);
    info->setDefaultTextColor(Theme::MARRON_FONCE);
    info->setFont(fontTexte);
    info->setPos(-info->boundingRect().width() / 2, -220);

    //cadre pour afficher les stats de l'ia
    double statsW = 260;
    double statsH = 80;
    QGraphicsRectItem* statsBg = sceneCite->addRect(0, 0, statsW, statsH, QPen(Theme::CUIVRE, 2), QBrush(Qt::white));
    statsBg->setPos(-statsW / 2, -160);

    //stats de l'ia :
    //pierres
    QGraphicsTextItem* pierres = sceneCite->addText(QString("Pierres : %1 ■").arg(ia->getPierres()));
    pierres->setDefaultTextColor(Theme::ORANGE);
    pierres->setFont(fontStats);
    pierres->setPos(-statsW/2 + (statsW - pierres->boundingRect().width())/2, -160 + 10);

    //score
    ia->getScore()->calculerScore();
    QGraphicsTextItem* score = sceneCite->addText(QString("Score Actuel : %1").arg(ia->getScore()->calculerScore()));
    score->setDefaultTextColor(Theme::MARRON_FONCE);
    score->setFont(fontStats);
    score->setPos(-statsW/2 + (statsW - score->boundingRect().width())/2, -160 + 45);

    //tuiles acquises
    QGraphicsTextItem* stashTitle = sceneCite->addText("SA COLLECTION :");
    stashTitle->setDefaultTextColor(Theme::CUIVRE);
    stashTitle->setFont(fontSousTitre);
    stashTitle->setPos(-stashTitle->boundingRect().width() / 2, -50);

    //paramètres des cadres autour des tuiles
    double cardW = 90; double cardH = 110; double space = 15;
    int cols = 6;
    double gridTotalWidth = (cols * cardW) + ((cols - 1) * space);
    double startX = -gridTotalWidth / 2;
    double currentX = startX;
    double currentY = 0;
    int count = 0;

    //on parcourt les tuiles de l'ia :
    for (auto it = ia->begin(); it != ia->end(); ++it) {
        
        //on crée leur cadre
        QGraphicsRectItem* card = new QGraphicsRectItem(0, 0, cardW, cardH);
        card->setBrush(QBrush(Theme::BEIGE_CARTE)); 
        card->setPen(QPen(Theme::CUIVRE, 3));      
        card->setPos(currentX, currentY);
        sceneCite->addItem(card);

       //on crée la tuile
        Tuile* t = *it;
        TuileItem* item = new TuileItem((*it), -1);
        if (t->getNbHexagones() == 4) { //cas tuile de depart
            item->setScale(0.45); //on réduit sa taille de base
            item->setPos(currentX + cardW/2, currentY + cardH/2 + 5);
        } else { //tuile normale
            item->setScale(0.55); //on réduit moins la taille des tuiles classiques (déjà plus petites)
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

    //bouton continuer
    QPushButton* btnLocal = new QPushButton("CONTINUER");
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

	//on utilise un proxy car un QPushButton ne peut être ajouté à une QGraphicsScene
	//QGraphicsScene n'accepte que des QGraphicsItem
	//le proxy est un QGraphicsItem cliquable qui simule donc le comportement d'un QPushButton
    QGraphicsProxyWidget* proxy = sceneCite->addWidget(btnLocal);
    proxy->setPos(-110, currentY + cardH + 40);
}


void MainWindow::afficherFinDePartie() {

    bool supportEmojis = false;

#ifdef Q_OS_WIN
    // Windows 10+ supporte les emojis colorés
    QOperatingSystemVersion current = QOperatingSystemVersion::current();
    if (current >= QOperatingSystemVersion::Windows10) {
        supportEmojis = true;
    }
#elif defined(Q_OS_MACOS)
    macOS supporte bien les emojis
    supportEmojis = true;
#elif defined(Q_OS_LINUX)
    // Sur Linux, on teste si une police emoji est installée
    QFontDatabase fontDb;
    QStringList families = fontDb.families();
    // Recherche de polices emoji courantes
    if (families.contains("Noto Color Emoji") ||
        families.contains("Twitter Color Emoji") ||
        families.contains("Segoe UI Emoji")) {
        supportEmojis = true;
#endif

    
    vector<Joueur*> joueursClasses;
    for (auto it = Partie::getInstance().debutJoueurs(); it != Partie::getInstance().finJoueurs(); ++it) {
        joueursClasses.push_back(*it);
    }

	//on trie les joueurs par score décroissant (et par nb de pierres en cas d'égalité)
	//on ne peut pas utiliser determinerGagnants car on veut le classement complet
	//or determinerGagnants ne renvoie que le premier (les premiers en cas d'égalité)
    sort(joueursClasses.begin(), joueursClasses.end(), [](Joueur* a, Joueur* b) {
        int scoreA = a->getScore()->calculerScore();
        int scoreB = b->getScore()->calculerScore();

        //si les scores sont différents, le plus grand gagne
        if (scoreA != scoreB) {
            return scoreA > scoreB;
        }
        //si égalité de score, celui qui a le plus de pierres gagne
        return a->getPierres() > b->getPierres();
        });

    //choix des symboles selon le support
    QStringList medailles;
    QStringList couleursMedailles;

    if (supportEmojis) {
        medailles = {"🥇", "🥈", "🥉", "🎖️"};
        couleursMedailles = {"inherit", "inherit", "inherit", "inherit"}; //pas besoin de couleur
    } else {
        medailles = {"★", "◆", "●", "○"};
        couleursMedailles = {"#FFD700", "#C0C0C0", "#CD7F32", "#B8860B"};
    }

    QString texteScores = "<div style='text-align: center; line-height: 180%;'>";

    for (size_t i = 0; i < joueursClasses.size(); ++i) {
        Joueur* j = joueursClasses[i];
        QString couleur = (i == 0) ? "#E67E22" : "#734526";
        QString bgColor = (i == 0) ? "rgba(230, 126, 34, 0.15)" : "rgba(115, 69, 38, 0.08)";
        QString couleurMedaille = couleursMedailles[min((int)i, 3)];

        int tailleMedaille = supportEmojis ? 20 : 24; // Emojis un peu plus petits

        texteScores += QString(
                           "<div style='background: %1; border-radius: 8px; padding: 12px; margin: 8px 0;'>"
                           "   <span style='font-size: %2px; color: %3;'>%4</span> "
                           "   <span style='font-size: 18px; color: %5; font-weight: bold;'>%6</span> : "
                           "   <span style='font-size: 22px; font-weight: bold; color: %5;'>%7 pts</span>"
                           "</div>"
                           ).arg(bgColor)
                           .arg(tailleMedaille)
                           .arg(couleurMedaille)
                           .arg(medailles[min((int)i, 3)])
                           .arg(couleur)
                           .arg(QString::fromStdString(j->getNom()))
                           .arg(j->getScore()->calculerScore());
    }
    texteScores += "</div>";

    // --- 2. TEXTE DU VAINQUEUR ---
    vector<Joueur*> gagnants = Partie::getInstance().determinerGagnants();
    QString texteVainqueur;

    if (gagnants.size() == 1) {
        QString symboleVictoire = supportEmojis ? "👑" : "♔";
        QString couleurSymbole = supportEmojis ? "inherit" : "#FFD700";

        texteVainqueur = QString(
                             "<div style='text-align: center; margin: 30px 0;'>"
                             "   <div style='font-size: 60px; margin-bottom: 10px; color: %1;'>%2</div>"
                             "   <div style='font-size: 18px; color: #734526; letter-spacing: 2px; margin-bottom: 8px;'>VICTOIRE DE</div>"
                             "   <div style='font-size: 32px; font-weight: bold; color: #dc8d55; "
                             "               text-shadow: 2px 2px 4px rgba(0,0,0,0.2);'>%3</div>"
                             "   <div style='font-size: 14px; color: #734526; margin-top: 10px; font-style: italic;'>"
                             "      L'architecte le plus prestigieux d'Akropolis !"
                             "   </div>"
                             "</div>"
                             ).arg(couleurSymbole)
                             .arg(symboleVictoire)
                             .arg(QString::fromStdString(gagnants[0]->getNom()).toUpper());
    } else {
        QString symboleEgalite = supportEmojis ? "🤝" : "⚖";
        QString couleurSymbole = supportEmojis ? "inherit" : "#dc8d55";

        texteVainqueur = QString(
                             "<div style='text-align: center; margin: 30px 0;'>"
                             "   <div style='font-size: 60px; margin-bottom: 10px; color: %1;'>%2</div>"
                             "   <div style='font-size: 28px; font-weight: bold; color: #dc8d55;'>ÉGALITÉ OLYMPIQUE !</div>"
                             "   <div style='font-size: 14px; color: #734526; margin-top: 10px;'>Les architectes partagent la gloire</div>"
                             "</div>"
                             ).arg(couleurSymbole)
                             .arg(symboleEgalite);
    }

    //création fenetre désignation gagnant :
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

    QLabel* lblVainqueur = new QLabel(texteVainqueur, frame);
    lblVainqueur->setObjectName("vainqueur");
    lblVainqueur->setAlignment(Qt::AlignCenter);
    lblVainqueur->setTextFormat(Qt::RichText);
    frameLayout->addWidget(lblVainqueur);

    QLabel* lblScores = new QLabel(texteScores, frame);
    lblScores->setAlignment(Qt::AlignCenter);
    lblScores->setTextFormat(Qt::RichText);
    frameLayout->addWidget(lblScores);

    //boutons :
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

	//connexions des boutons
    connect(btnMenu, &QPushButton::clicked, &overlay, &QDialog::accept);
    connect(btnQuitter, &QPushButton::clicked, this, &MainWindow::quitterJeu);

	//si on clique sur menu principal on revient au menu principal
    if (overlay.exec() == QDialog::Accepted) {
        stackedWidget->setCurrentWidget(pageMenuPrincipal);
        sceneCite->clear();
        sceneChantier->clear();
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //securités :
    if (indexTuileSelectionnee == -1) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    const Chantier& chantier = Partie::getInstance().getChantier();
    if (indexTuileSelectionnee >= chantier.getNbTuiles()) {
        QMainWindow::keyPressEvent(event);
        return;
    }

	//recupere la tuile sélectionnée
    auto it = chantier.begin();
    for (int i = 0; i < indexTuileSelectionnee; ++i) {
        ++it;
    }
    Tuile* t = *it;

    //gestion de la touche P (pivoter)
    if (event->key() == Qt::Key_P) {
        t->tourner();
        rotationCompteur = (rotationCompteur + 1) % 3;
        mettreAJourInterface();
    }

    //gestion de la touche I (inverser)
    else if (event->key() == Qt::Key_I) {
        t->inverser();
        inversionEtat = !inversionEtat;
        mettreAJourInterface();
    }

    //gestion de la touche Entrée (valider le placement)
    else if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && previewActive) {
        onValidationClicked(); 
    }
    else {
        //passe l'événement au parent pour les autres touches
        QMainWindow::keyPressEvent(event);
    }
}


void MainWindow::afficherInfoRaccourcisClavier()
{
	//fenêtre  d'affichage des raccourcis clavier
    QDialog dialog(this);
    dialog.setWindowTitle("Astuce : Raccourcis Clavier");
    dialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    dialog.setFixedSize(420, 280);

    //layout principal
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(15, 15, 15, 15);

    //création du cadre
    QFrame* frame = new QFrame(&dialog);
    frame->setProperty("class", "CadreConfig");
    QVBoxLayout* frameLayout = new QVBoxLayout(frame);
    frameLayout->setSpacing(15);

    //titre
    QLabel* titre = new QLabel("Astuce : Raccourcis Clavier", frame);
    titre->setAlignment(Qt::AlignCenter);
    titre->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #dc8d55;"
        );
    frameLayout->addWidget(titre);

    //explications :
    QLabel* texte = new QLabel(frame);
    texte->setAlignment(Qt::AlignCenter);
    texte->setWordWrap(true);
    texte->setText(
        "Vous pouvez utiliser les touches du clavier :<br><br>"
        "<span style='font-weight:bold; color:#dc8d55; font-size:16px;'>P</span>"
        " : Pivoter la tuile sélectionnée<br>"
        "<span style='font-weight:bold; color:#dc8d55; font-size:16px;'>I</span>"
        " : Inverser la tuile sélectionnée<br>"
        "<span style='font-weight:bold; color:#dc8d55; font-size:16px;'>Entrée</span>"
        " : Valider le placement de la tuile<br><br>"
        "Les boutons <b>Pivoter Tuile</b>, <b>Inverser Tuile</b> "
        "et <b>Valider Placement</b> restent disponibles."
        );
    texte->setStyleSheet(
        "color: #4E2E1E;"
        "font-size: 14px;"
        );
    frameLayout->addWidget(texte);

    //bouton OK
    QPushButton* btnOk = new QPushButton("OK, compris !", frame);
    btnOk->setProperty("class", "BoutonMenu");
    btnOk->setDefault(true);
    btnOk->setAutoDefault(true);

    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    frameLayout->addWidget(btnOk, 0, Qt::AlignCenter);

    layout->addWidget(frame);
    dialog.exec();
}
