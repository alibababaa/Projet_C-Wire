#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct NoeudAVL {
    int identifiant;
    long capacite;
    long consommation; 
    struct NoeudAVL* fg;
    struct NoeudAVL* fd;
    int hauteur;
} NoeudAVL;

typedef struct {
    int identifiant;
    long capacite;
    long consommationTotale;
} StationData;

// Fonctions principales de l'AVL
NoeudAVL* inserer(NoeudAVL* racine, int identifiant, long capacite, long consommation);
NoeudAVL* rechercher(NoeudAVL* racine, int identifiant);
void libererAVL(NoeudAVL* noeud);

// Fonctions utilitaires pour les résultats
StationData* avl_to_array(NoeudAVL* racine, int* count);
int comparer_par_capacite(const void* a, const void* b);

#endif
