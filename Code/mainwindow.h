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
#include "partie.h"
#include "tuileitem.h"
#include "ia.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void afficherMenuJeu();
    void afficherMenuRegles();
    void quitterJeu();
    void demarrerPartie();
    void mettreAJourInterface();
    void onRotationClicked();
    void onInversionClicked();
    void onValidationClicked();
    void selectionnerTuileChantier(int index);
    void afficherFinDePartie();   // Nouveau : pour afficher les résultats

private:
    // Widgets pour le menu principal
    QStackedWidget *stackedWidget;
    QWidget *pageMenuPrincipal;
    QWidget *pageRegles;
    QWidget *pageJeu;

    // Widgets pour la page de jeu
    QGraphicsScene *sceneCite;
    QGraphicsView *viewCite;
    QLabel *labelInfoJoueur;
    QGraphicsScene *sceneChantier;
    QGraphicsView *viewChantier;
    QPushButton *btnRotation;
    QPushButton *btnInversion;
    QPushButton *btnValidation;
    QPushButton* btnPasserTour;

    // État de la partie
    int indexTuileSelectionnee;
    int rotationCompteur; // Nouveau : pour suivre le nombre de rotations
    bool inversionEtat;   // Nouveau : pour suivre l'état d'inversion

    // Méthodes d'affichage
    void initialiserPageMenuPrincipal();
    void initialiserPageRegles();
    void initialiserPageJeu();
    void dessinerCite(Joueur* joueur);
    void dessinerChantier();
    QColor getTypeColor(TypeQuartier t);
    void onPasserTourClicked();
};

#endif // MAINWINDOW_H
