#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include "partie.h"
#include "tuileitem.h"
#include "ia.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void afficherMenuJeu();
    void afficherMenuConfig(); // Nouveau slot pour afficher la page config
    void mettreAJourVisibiliteConfig();
    void validerConfiguration(); // Slot pour lancer la partie depuis la config
    void resetConfiguration(); // Slot pour réinitialiser la config
    void afficherMenuRegles();
    void quitterJeu();
    void mettreAJourInterface();
    void onRotationClicked();
    void onInversionClicked();
    void onValidationClicked();
    void selectionnerTuileChantier(int index);
    void afficherFinDePartie();   // Nouveau : pour afficher les résultats
    void onContinuerIAClicked();

private:
    // Widgets pour le menu principal
    QStackedWidget *stackedWidget;
    QWidget *pageMenuPrincipal;
    QWidget *pageRegles;
    QWidget *pageJeu;

    // Widgets pour la page de configuration
    QWidget *pageConfig;
    QButtonGroup *groupeNbJoueurs;
    QGroupBox *groupeIA;
    QButtonGroup *groupeNiveauIA;
    std::vector<QLineEdit*> champsNomsJoueurs;
    std::vector<QCheckBox*> checkBoxesVariantes;
    QCheckBox *checkModeAugmente;
    void initialiserPageConfiguration();



    // Widgets pour la page de jeu
    QGraphicsScene *sceneCite;
    QGraphicsView *viewCite;
    QLabel *labelInfoJoueur;
    QGraphicsScene *sceneChantier;
    QGraphicsView *viewChantier;
    QPushButton *btnRotation;
    QPushButton *btnInversion;
    QPushButton *btnValidation;
    QLabel *labelPilesRestantes;

    // État de la partie
    int indexTuileSelectionnee;
    int rotationCompteur; // Nouveau : pour suivre le nombre de rotations
    bool inversionEtat;   // Nouveau : pour suivre l'état d'inversion

    //preview tuile avant validation placement
    bool previewActive;
    int previewX;
    int previewY;
    int previewZ;
    void dessinerPreview();

    //IA
    bool affichageResultatIA;
    int dernierIndexIA;
    void dessinerInterfaceIA(IA* ia);

    // Méthodes d'affichage
    void initialiserPageMenuPrincipal();
    void initialiserPageRegles();
    void initialiserPageJeu();
    void dessinerCite(Joueur* joueur);
    void dessinerChantier();
    QColor getTypeColor(TypeQuartier t);
    void traiterClicPlateau(QPointF positionScene);
};

#endif // MAINWINDOW_H
