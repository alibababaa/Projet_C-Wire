# Projet C-Wire - Gestion du Flux Électrique

## Description 

Cette application permet de gérer le flux électrique dans un pays représentant environ un quart de la superficie de la France métropolitaine. Elle inclut des fonctionnalités pour :

  Afficher les stations de distribution, les centrales électriques et les consommateurs.

  Visualiser les capacités de production et de consommation énergétique.

  Centraliser et simplifier l'accès à ces informations pour différents utilisateurs potentiels.

  L'objectif principal est d'analyser les données de distribution d'énergie et de détecter des situations de surproduction ou de sous-production, tout en identifiant la proportion d'énergie consommée par les entreprises et les particuliers.

## Fonctionnalités

Filtrage : Extraction des données pertinentes à partir d'un fichier CSV massif contenant des informations sur la distribution énergétique.

Calcul : Utilisation d'un arbre AVL pour effectuer des calculs efficaces sur les capacités et consommations.

Affichage des résultats : Génération de fichiers de sortie triés et structurés.

Création de graphiques : Génération optionnelle de graphiques pour visualiser les résultats.

Messages d'erreur : Aide intégrée pour corriger les erreurs de commande ou de paramètres.

## Prérequis

Système d'exploitation : Linux.

Compilateur : GCC avec support de la norme C99.


Bibliothèques nécessaires :

string.h pour la gestion des chaînes.

stdlib.h pour l'allocation mémoire.

stdio.h pour les entrées/sorties.

Dépendances externes : GnuPlot 

## Installation

Téléchargez le projet depuis le dépôt GitHub.

Décompressez le fichier ZIP si nécessaire :

unzip EnergyFlow-main.zip
cd EnergyFlow-main

Donnez les permissions d'exécution au script Shell :

chmod +x c-wire.sh

## Utilisation

Pour exécuter le programme, utilisez la commande suivante :

./c-wire.sh -f $TRAJET/vers/le/fichier/d'entrée/à/traiter -s letypedestation -c letypedeconsommateur -o $TRAJET/vers/le/fichier/de sortie/filtré

Exemple : ./c-wire.sh -f $HOME/Documents/Projet_C-Wire/input/c-wire_v00.dat -s hva -c comp -o $HOME/Documents/Projet_C-Wire/tests/output.csv

## Options disponibles :

-f : Chemin vers le fichier CSV d'entrée contenant les données de distribution énergétique.

-s : Type de station à traiter :

hvb : Stations haute tension de type B.

hva : Stations haute tension de type A.

lv : Postes basse tension.

-c : Type de consommateur à analyser :

comp : Consommateurs entreprises.

indiv : Consommateurs particuliers.

all : Tous les consommateurs.

-o : Chemin vers le fichier de sortie filtré.

## Exemple d'exécution :

./c-wire.sh -f /home/user/input/data.csv -s hva -c comp -o /home/user/output/hva_comp.csv

## Aide intégrée :

Si la commande d'exécution est incorrecte, un message d'erreur détaillé s'affichera, précisant les options attendues.
Pour afficher l'aide manuellement :

./c-wire.sh -h

Structure des fichiers générés

Fichiers finaux (résultats) : Générés dans le dossier tests ou dans l'emplacement spécifié via -o. Ils incluent des fichiers comme output.csv, lv_all_minmax.csv.

Fichiers temporaires : Stockés dans le dossier tmp pour un usage intermédiaire. Supprimés après l'exécution.

Graphiques : Si demandés, ils seront sauvegardés dans le dossier graphs.


## Notes supplémentaires:

Le projet est conçu pour fonctionner avec un fichier CSV volumineux contenant plusieurs millions de lignes. Assurez-vous que votre environnement dispose des ressources nécessaires.

L'application effectue des vérifications rigoureuses sur les données d'entrée et retourne un code d'erreur en cas de problème.

Toutes les allocations dynamiques sont libérées avant la fin de l'exécution pour garantir une gestion optimale de la mémoire.

## Auteurs

Yann AGBOTA

Ilyes FELLAH

Ali-ulas YILDIZ
