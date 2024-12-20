/**Gestion du flux electriques 
 Description
 Cette application permet de gérer le flux electrique dans un pays faisant 1/4 de la superficie de la france métropôlitaine, 
y compris l'affichage des station de distibution,des centrales et des consommateurs, leurs capacités de production et leurs consommatoion. 
L'objectif est de centraliser et faciliter l'accès à ces informations pour les différents utilisateurs potentiel.

 Installation

Prérequis
Avoir linux installé sur votre ordinateur 

 Étapes d'installation
Intégrer le dépôt 
Installez les dépendances 
Compiler le code à l’aide de la fonction make dans votre terminal  
Exécuter le code à l’aide de la fonction ./c-wire
  Fonctionnalités
..........................
..........................
..........................

Auteurs et reconnaissance
AGBOTA Yann/ FELLAH Ilyes /YILDIZ Ali-ulas - Créateurs et développeurs principaux
Remerciements particuliers à tous les contributeurs du projet
**/
# EnergyFlow Project

## Description
Ce projet implémente un arbre AVL pour gérer et afficher des informations sur des stations et leur consommation énergétique. L'arbre AVL est équilibré dynamiquement pour assurer une recherche efficace.

## Fonctionnalités
- **Insertion** : Ajoute une station avec sa consommation.
- **Affichage** : Parcours infixe pour afficher les stations par ordre alphabétique.
- **Libération** : Gestion correcte de la mémoire dynamique.
- **Limitation de mémoire** : Prévention des dépassements de mémoire grâce à `setrlimit`.

## Prérequis
- **Compilateur** : GCC avec support de la norme C99.
- **Bibliothèques** :
  - `string.h` pour la gestion des chaînes.
  - `stdlib.h` pour l'allocation mémoire.
  - `stdio.h` pour les entrées/sorties.

## Installation
1. Clonez ou téléchargez le projet.
2. Décompressez le fichier ZIP si nécessaire :
   ```bash
   unzip EnergyFlow-main.zip
   cd EnergyFlow-main
