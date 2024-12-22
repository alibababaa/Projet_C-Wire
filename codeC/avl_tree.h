#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


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
static int max(int a, int b);
static int hauteur(NoeudAVL* n);
static NoeudAVL* creerNoeud(int identifiant, long capacite, long consommation);
static int facteurEquilibre(NoeudAVL* n);
static NoeudAVL* rotationDroite(NoeudAVL* y);
static NoeudAVL* rotationGauche(NoeudAVL* x);
static NoeudAVL* equilibrer(NoeudAVL* noeud);
NoeudAVL* inserer(NoeudAVL* racine, int identifiant, long capacite, long consommation);
NoeudAVL* rechercher(NoeudAVL* racine, int identifiant);
void libererAVL(NoeudAVL* noeud);
static void count_nodes(NoeudAVL* n, int* compteur);
static void avl_to_array_infixe(NoeudAVL* racine, StationData* tab, int* index);
StationData* avl_to_array(NoeudAVL* racine, int* count);
int comparer_par_capacite(const void* a, const void* b);


// Fonctions utilitaires pour les graphiques
void erreurCritique(const char* message);
void process_csv(const char* filename, const char* output_filename);


#endif
