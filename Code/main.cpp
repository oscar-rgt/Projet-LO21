#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // On lance la fenêtre principale
    MainWindow w;
    w.show();

    return a.exec();
}
