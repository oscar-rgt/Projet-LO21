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
#include "hexagoneitem.h"

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
    void onValidationClicked();
    void selectionnerTuileChantier(int index);

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
    QPushButton *btnValidation;

    // État de la partie
    int indexTuileSelectionnee;
    int rotationActuelle;

    // Méthodes d'affichage
    void initialiserPageMenuPrincipal();
    void initialiserPageRegles();
    void initialiserPageJeu();
    void dessinerCite(Joueur* joueur);
    void dessinerChantier();
    QColor getTypeColor(TypeQuartier t);
};

#endif // MAINWINDOW_H
