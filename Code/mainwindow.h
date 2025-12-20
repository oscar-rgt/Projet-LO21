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
    void keyPressEvent(QKeyEvent *event) override;


private slots:
    // --- 1. Navigation & Menus ---
    void afficherMenuJeu();
    void afficherMenuConfig();              // Nouveau slot pour afficher la page config
    void mettreAJourVisibiliteConfig();     // Gestion dynamique du formulaire config
    void validerConfiguration();            // Lancer la partie
    void resetConfiguration();              // Réinitialiser le formulaire
    void afficherMenuRegles();
    void quitterJeu();
    void afficherInfoRaccourcisClavier();

    // --- 2. Moteur de Jeu & Rendu ---
    void mettreAJourInterface();            // Rafraîchissement global
    void onRotationClicked();
    void onInversionClicked();
    void onValidationClicked();
    void selectionnerTuileChantier(int index);

    // --- 3. Fin de Partie & IA ---
    void afficherFinDePartie();             // Affiche l'overlay de fin
    void onContinuerIAClicked();            // Passe l'écran de résumé IA

    // --- 4. Système (Pause & Sauvegarde) ---
    void onReglagesClicked();               // Pop-up Pause
    void onSauvegarderClicked();            // Sauvegarde
    void onChargerPartieClicked();

private:
    // =============================================================
    // 1. WIDGETS & PAGES
    // =============================================================

    // Gestionnaire de pages
    QStackedWidget *stackedWidget;
    QWidget *pageMenuPrincipal;
    QWidget *pageRegles;
    QWidget *pageJeu;
    QWidget *pageConfig;
    QWidget *pagePrecedente; // Mémoire de navigation

    // Page Configuration
    QButtonGroup *groupeNbJoueurs;
    QGroupBox *groupeIA;
    QButtonGroup *groupeNiveauIA;
    std::vector<QLineEdit*> champsNomsJoueurs;
    std::vector<QCheckBox*> checkBoxesVariantes;
    QCheckBox *checkModeAugmente;
    QString htmlTexteJoueur;

    // Page Jeu (Interface)
    QGraphicsScene *sceneCite;
    QGraphicsView *viewCite;
    QGraphicsScene *sceneChantier;
    QGraphicsView *viewChantier;

    // Contrôles Jeu
    QLabel *labelInfoJoueur;
    QLabel *labelPilesRestantes;
    QPushButton *btnRotation;
    QPushButton *btnInversion;
    QPushButton *btnValidation;
    QPushButton *btnReglages;

    // =============================================================
    // 2. ÉTAT DU JEU
    // =============================================================

    // Sélection & Manipulation
    int indexTuileSelectionnee;
    int rotationCompteur;
    bool inversionEtat;

    // Prévisualisation
    bool previewActive;
    int previewX;
    int previewY;
    int previewZ;

    // État IA
    bool affichageResultatIA;
    int dernierIndexIA;

    // =============================================================
    // 3. MÉTHODES D'INITIALISATION
    // =============================================================
    void initialiserPageMenuPrincipal();
    void initialiserPageRegles();
    void initialiserPageConfiguration();
    void initialiserPageJeu();

    // =============================================================
    // 4. MÉTHODES DE DESSIN & LOGIQUE INTERNE (Helpers)
    // =============================================================

    // Ces fonctions d'affichent
    void dessinerCite(Joueur* joueur);
    void dessinerChantier();
    void dessinerPreview(Joueur* j);
    void dessinerInterfaceIA(IA* ia);

    // Utilitaires
    void traiterClicPlateau(QPointF positionScene);
    QColor getTypeColor(TypeQuartier t);
};

#endif // MAINWINDOW_H
