#include "mainwindow.h"
#include "hexagoneitem.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGraphicsPolygonItem>
#include <cmath> // Pour cos/sin des hexagones

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 1. Configuration de la fenêtre
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // 2. Zone gauche : La Cité (Visuel)
    sceneCite = new QGraphicsScene(this);
    viewCite = new QGraphicsView(sceneCite);
    viewCite->setRenderHint(QPainter::Antialiasing); // Pour que ce soit joli
    mainLayout->addWidget(viewCite, 2); // Prend 2/3 de la place

    // 3. Zone droite : Contrôles et Chantier
    QVBoxLayout *sideLayout = new QVBoxLayout();
    labelInfoJoueur = new QLabel("En attente...", this);
    sideLayout->addWidget(labelInfoJoueur);

    QPushButton *btnRotation = new QPushButton("Pivoter Tuile", this);
    connect(btnRotation, &QPushButton::clicked, this, &MainWindow::onRotationClicked);
    sideLayout->addWidget(btnRotation);

    layoutChantier = new QVBoxLayout();
    // On ajoute un titre pour cette section
    sideLayout->addWidget(new QLabel("--- CHANTIER ---"));
    sideLayout->addLayout(layoutChantier); // On l'imbrique dans la colonne de droite

    // Ajout d'un "ressort" pour pousser le chantier vers le haut
    sideLayout->addStretch();

    // Ajout au layout principal
    mainLayout->addLayout(sideLayout, 1);

    // 4. Lancement du jeu via votre Singleton
    demarrerPartie();
}

void MainWindow::selectionnerTuileChantier(int index) {
    indexTuileSelectionnee = index;
    qDebug() << "Tuile selectionnee : " << index;
}

MainWindow::~MainWindow() {}

void MainWindow::demarrerPartie() {
    // Configuration hardcodée pour le test graphique (à remplacer par une boîte de dialogue)
    std::vector<std::string> noms = {"Joueur 1", "Joueur 2"};
    std::array<bool, 5> variantes = {false, false, false, false, false};

    Partie::getInstance().initialiser(2, noms, Partie::TuileCite::STANDARD, variantes, 0);

    mettreAJourInterface();
}

void MainWindow::mettreAJourInterface() {
    // Récupérer les infos du modèle
    Joueur* j = Partie::getInstance().getJoueurActuel();
    labelInfoJoueur->setText("Tour de : " + QString::fromStdString(j->getNom()) +
                             "\nPierres : " + QString::number(j->getPierres()) +
                             "\nScore : " + QString::number(j->getScore()->getTotal()));

    dessinerCite(j);
    dessinerChantier();
}

// Fonction utilitaire pour dessiner un hexagone
void ajouterHexagoneGraphique(QGraphicsScene* scene, double x, double y, QColor couleur, QString texte) {
    double r = 30.0; // Rayon de l'hexagone
    QPolygonF hexagone;
    for (int i = 0; i < 6; ++i) {
        double angle_deg = 60 * i - 30;
        double angle_rad = M_PI / 180 * angle_deg;
        hexagone << QPointF(x + r * cos(angle_rad), y + r * sin(angle_rad));
    }

    QGraphicsPolygonItem *item = scene->addPolygon(hexagone, QPen(Qt::black), QBrush(couleur));

    // Ajouter du texte (ex: niveau ou type)
    QGraphicsTextItem *txt = scene->addText(texte);
    txt->setDefaultTextColor(Qt::white);
    txt->setPos(x - 10, y - 10);
}


void MainWindow::onRotationClicked() {
    rotationActuelle = (rotationActuelle + 1) % 3;
    // Logique pour tourner la tuile sélectionnée visuellement
}

void MainWindow::onValidationClicked() {
    // Appeler Partie::getInstance().actionPlacerTuile(...)
    mettreAJourInterface();
}


QColor getTypeColor(TypeQuartier t) {
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


void MainWindow::dessinerCite(Joueur* joueur) {
    sceneCite->clear();
    Cite* cite = joueur->getCite();

    double taille = 30.0; // Rayon visuel d'un hexagone

    // --- UTILISATION DE L'ITÉRATEUR CITE ---
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        // L'itérateur renvoie une paire : first = Coord, second = Hexagone*
        Coord pos = it->first;
        Hexagone* hex = it->second;

        // 1. Calcul des coordonnées pixels (Conversion Hex -> 2D)
        // Formule "Pointy topped" standard ajustée
        double w = sqrt(3.0) * taille;
        double h = 2.0 * taille * 0.75;

        double pixelX = pos.x * w + (pos.y % 2) * (w / 2.0); // Décalage pour hexagones
        // Note : Votre système de coordonnées peut varier, il faudra peut-être ajuster cette formule
        // selon la logique précise de vos vecteurs voisins dans "cite.cpp"
        double pixelY = pos.y * h;

        // Effet de hauteur (Z) : on décale vers le haut pour simuler la 3D
        pixelY -= (pos.z * 10.0);

        // 2. Récupération de la couleur via l'objet Hexagone
        QColor couleur = getTypeColor(hex->getType());

        // 3. Création de l'objet graphique
        HexagoneItem* item = new HexagoneItem(pixelX, pixelY, taille, couleur, pos.x, pos.y, pos.z);

        // 4. Ajout texte (Niveau / Étoiles)
        if (hex->getEtoiles() > 0) {
            QGraphicsTextItem* txt = sceneCite->addText(QString("*%1").arg(hex->getEtoiles()));
            txt->setPos(pixelX - 10, pixelY - 10);
            txt->setZValue(pos.z + 0.1); // Texte au-dessus de la tuile
        }

        sceneCite->addItem(item);
    }
}

void MainWindow::dessinerChantier() {
    // 1. Nettoyer le layout précédent (supprimer les anciens boutons)
    QLayoutItem *child;
    while ((child = layoutChantier->takeAt(0)) != 0) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    const Chantier& chantier = Partie::getInstance().getChantier();
    int index = 0;

    // Utilisation de l'itérateur Chantier comme demandé
    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        Tuile* tuile = *it;

        QPushButton* btnTuile = new QPushButton(this);
        btnTuile->setCheckable(true); // Pour pouvoir le laisser "enfoncé"

        // Création d'un texte descriptif simple pour la tuile
        QString desc = QString("Tuile #%1 (Prix: %2)").arg(index).arg(tuile->getPrix());
        btnTuile->setText(desc);

        // Connexion : quand on clique, on appelle la méthode de sélection
        // Note : on capture 'index' par copie [index]
        connect(btnTuile, &QPushButton::clicked, [this, index]() {
            this->selectionnerTuileChantier(index);
        });

        layoutChantier->addWidget(btnTuile);
        index++;
    }
}
