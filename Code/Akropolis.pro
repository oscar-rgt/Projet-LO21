QT += widgets

HEADERS += \
    chantier.h \
    cite.h \
    configjeu.h \
    gestionnairejoueurs.h \
    hexagone.h \
    hexagoneitem.h \
    ia.h \
    joueur.h \
    main.h \
    main.h \
    mainwindow.h \
    partie.h \
    pile.h \
    save.h \
    score.h \
    tuileitem.h \
    tuiles.h \
    coord.h

SOURCES += \
    chantier.cpp \
    cite.cpp \
    configjeu.cpp \
    gestionnairejoueurs.cpp \
    hexagone.cpp \
    hexagoneitem.cpp \
    ia.cpp \
    joueur.cpp \
    main.cpp \
    mainwindow.cpp \
    partie.cpp \
    pile.cpp \
    save.cpp \
    score.cpp \
    tuileitem.cpp \
    tuiles.cpp \
    coord.cpp

RESOURCES += \
    ressources.qrc

win32: RC_ICONS =images/Untitled.ico
