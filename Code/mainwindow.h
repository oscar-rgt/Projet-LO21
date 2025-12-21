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

using namespace std;

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
    //Navigation et menus
    void afficherMenuJeu();
    void afficherMenuConfig();              
    void mettreAJourVisibiliteConfig();     
    void validerConfiguration();            
    void resetConfiguration();              
    void afficherMenuRegles();
    void quitterJeu();
    void afficherInfoRaccourcisClavier();

	//Moteur de jeu
    void mettreAJourInterface();            
    void onRotationClicked();
    void onInversionClicked();
    void onValidationClicked();
    void selectionnerTuileChantier(int index);
    void onContinuerIAClicked();

    
    void afficherFinDePartie();             
               

    //Système (pause et sauvegarde)
    void onReglagesClicked();               
    void onSauvegarderClicked();            
    void onChargerPartieClicked();

private:
    //Wigets :
    //pages
    QStackedWidget *stackedWidget;
    QWidget *pageMenuPrincipal;
    QWidget *pageRegles;
    QWidget *pageJeu;
    QWidget *pageConfig;
    QWidget *pagePrecedente;

    //configuration jeu
    QButtonGroup *groupeNbJoueurs;
    QGroupBox *groupeIA;
    QButtonGroup *groupeNiveauIA;
    vector<QLineEdit*> champsNomsJoueurs;
    vector<QCheckBox*> checkBoxesVariantes;
    QCheckBox *checkModeAugmente;
    QString htmlTexteJoueur;

    //Cité et chantier
    QGraphicsScene *sceneCite;
    QGraphicsView *viewCite;
    QGraphicsScene *sceneChantier;
    QGraphicsView *viewChantier;

    //boutons
    QPushButton *btnRotation;
    QPushButton *btnInversion;
    QPushButton *btnValidation;
    QPushButton *btnReglages;

	//labels
    QLabel* labelInfoJoueur;
    QLabel* labelPilesRestantes;

    //gestion couleur
    QColor getTypeColor(TypeQuartier t);

    //attributs :
    //tuile
    int indexTuileSelectionnee;
    int rotationCompteur;
    bool inversionEtat;

    //previsualisation
    bool previewActive;
    int previewX;
    int previewY;
    int previewZ;

    //ia
    bool affichageResultatIA;
    int dernierIndexIA;

	//methodes :
    //initialisations
    void initialiserPageMenuPrincipal();
    void initialiserPageRegles();
    void initialiserPageConfiguration();
    void initialiserPageJeu();

    //affichage
    void dessinerCite(Joueur* joueur);
    void dessinerChantier();
    void dessinerPreview(Joueur* j);
    void dessinerInterfaceIA(IA* ia);

	//interactions
    void traiterClicPlateau(QPointF positionScene);
};

#endif
