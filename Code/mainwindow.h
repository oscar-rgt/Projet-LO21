#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "partie.h" // On inclut votre moteur de jeu

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots pour les interactions boutons
    void onRotationClicked();
    void onValidationClicked();

    // Slot pour gérer le clic sur une tuile du chantier
    //void onTuileChantierClicked(int index);

private:
    void demarrerPartie();
    void mettreAJourInterface();
    void dessinerCite(Joueur* joueur);


    void dessinerChantier();
    void selectionnerTuileChantier(int index);

    // Widgets de l'interface
    QGraphicsScene *sceneCite; // zone où on dessine la cité
    QGraphicsView *viewCite;   // vue de la cité

    QVBoxLayout *layoutChantier;
    QLabel *labelInfoJoueur;   // affichage infos joueur

    // État temporaire pour la sélection
    int indexTuileSelectionnee = -1;
    int rotationActuelle = 0;
};

#endif // MAINWINDOW_H
