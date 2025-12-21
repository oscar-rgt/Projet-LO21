# 🏛️ Akropolis - Projet LO21

> Adaptation numérique du jeu de société **Akropolis** développée en C++ avec le framework Qt.
> Ce projet a été réalisé dans le cadre de l'unité d'enseignement LO21 (Programmation et Conception Orientées Objet).

## 👥 Auteurs

* **Rouillé Louane**
* **Ridoux Valentin**
* **Rogelt Oscar**
* **Mogis Noémie**
* **Deboudt Jeanne**

---

## 📋 Description

Dans **Akropolis**, vous incarnez des architectes de la Grèce antique. Votre objectif est de construire la cité la plus prestigieuse en agençant judicieusement vos tuiles (Quartiers, Places, Casernes, etc.) et en exploitant la hauteur pour multiplier vos points.

Ce projet propose une implémentation complète des règles officielles, incluant :
* Une interface graphique **2D**
* Le calcul automatique des scores et des ressources (Pierre).
* Une **Intelligence Artificielle (IA)** pour le mode solo.

## ✨ Fonctionnalités

### Modes de Jeu
* **Solo vs IA** : Affrontez une IA capable de choisir stratégiquement ses tuiles.
* **Multijoueur local** : Jouez jusqu'à 4 joueurs sur la même machine.
* **Variantes** : Support des règles avancées et des extensions (ex: Tuiles augmentées).

### Interfaces
* **Interface Graphique (GUI)** : Une expérience visuelle riche utilisant `QGraphicsView`.
* **Interface Console (CLI)** : Une version textuelle complète pour les puristes ou le débogage.

### Système Technique
* **Sauvegarde & Chargement** : Possibilité de sauvegarder une partie en cours et de la reprendre plus tard.
* **Architecture Robuste** : Conception orientée objet stricte utilisant des design patterns (Singleton, Factory, State/Strategy pour l'IA).

---

## 🚀 Installation et Lancement

### Prérequis
* **Qt 6.x** (Modules `Core`, `Gui`, `Widgets`)
* Un compilateur C++ compatible C++17 (MinGW, MSVC, Clang)
* **Qt Creator** (Recommandé)

### Compilation
1. Clonez ce dépôt ou téléchargez les sources.
2. Ouvrez le fichier projet `Code/Akropolis.pro` avec **Qt Creator**.
3. Configurez le kit de compilation (Desktop Qt 6.x).
4. Lancez la compilation (Build) et l'exécution (Run).

*(Alternativement, vous pouvez utiliser `qmake` en ligne de commande)*

---

## 🎮 Commandes (Interface Graphique)

| Action | Commande |
| :--- | :--- |
| **Sélectionner une tuile** | Clic Gauche sur les tuiles du chantier |
| **Poser la tuile** | Clic Gauche sur le plateau |
| **Tourner la tuile** | Bouton `Pivoter` ou touche `P` du clavier |
| **Inverser la tuile** | Bouton `Inverser` ou touche `I` du clavier |
| **Valider le placement de la tuile** | Bouton `Valider` ou touche `Entrée` du clavier |

---

## 🏗️ Architecture Logicielle

Ce projet suit une architecture en couches séparant strictement la logique métier de l'interface utilisateur.

* **Modèle (`Partie`, `Cite`, `Tuile`)** :
    * Indépendant de l'interface (aucune dépendance à Qt GUI).
    * La classe `Partie` est un **Singleton** qui orchestre le jeu.
    * La `Cite` gère la logique spatiale 3D via un système de coordonnées `{x, y, z}`.

* **Vues (`MainWindow`, `JeuConsole`)** :
    * Implémentent le pattern **Model-View-Delegate**.
    * `MainWindow` gère à la fois l'affichage (`QGraphicsScene`) et les interactions utilisateur (Events), agissant comme un contrôleur de vue.

* **Design Patterns utilisés** :
    * **Singleton** : Pour l'accès global à l'instance de `Partie`.
    * **Héritage Multiple** : Pour les objets graphiques (`TuileItem` hérite de `QObject` et `QGraphicsItemGroup`).
    * **Iterators** : Pour le parcours des collections personnalisées.

---

## 📝 État du projet

* [x] Moteur de jeu complet (Règles, 3D, Scores)
* [x] Interface Graphique Qt fonctionnelle
* [x] Interface Console
* [x] IA basique/avancée
* [x] Sauvegarde/Chargement