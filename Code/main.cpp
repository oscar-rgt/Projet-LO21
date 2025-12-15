#if __has_include(<QApplication>)
#include <QApplication>
#define has_QApp 1
#include <QApplication>
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // On lance la fenêtre principale
    MainWindow w;
    w.showMaximized();

    return a.exec();
}
#else
#include "jeuConsole.h"
int main(int argc, char* argv[]) {
    JeuConsole().lancer();
}
#endif