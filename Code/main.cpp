#if __has_include(<QApplication>) //si le jeu est lancé sur QTCreator :
#include <QApplication>
#define has_QApp 1
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // On lance la fenêtre principale QT
    MainWindow w;
    w.showMaximized();

    return a.exec();
}
#else //si le jeu est lancé sur un IDE classique :
#include "jeuConsole.h"
int main(int argc, char* argv[]) {
    JeuConsole().lancer();
}
#endif