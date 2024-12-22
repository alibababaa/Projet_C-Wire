#include "avl_tree.h"


static int max(int a, int b) { return a > b ? a : b; }

static int hauteur(NoeudAVL* n) {
    return n ? n->hauteur : 0;
}

static NoeudAVL* creerNoeud(int identifiant, long capacite, long consommation) {
    NoeudAVL* n = malloc(sizeof(NoeudAVL));
    if (!n) exit(1);  // Échec allocation mémoire
    n->identifiant = identifiant;
    n->capacite = capacite;
    n->consommation = consommation;
    n->fg = n->fd = NULL;
    n->hauteur = 1;
    return n;
}

static int facteurEquilibre(NoeudAVL* n) {
    return n ? hauteur(n->fd) - hauteur(n->fg) : 0;
}

static NoeudAVL* rotationDroite(NoeudAVL* y) {
    NoeudAVL* x = y->fg;
    NoeudAVL* T2 = x->fd;
    x->fd = y;
    y->fg = T2;
    y->hauteur = max(hauteur(y->fg), hauteur(y->fd)) + 1;
    x->hauteur = max(hauteur(x->fg), hauteur(x->fd)) + 1;
    return x;
}

static NoeudAVL* rotationGauche(NoeudAVL* x) {
    NoeudAVL* y = x->fd;
    NoeudAVL* T2 = y->fg;
    y->fg = x;
    x->fd = T2;
    x->hauteur = max(hauteur(x->fg), hauteur(x->fd)) + 1;
    y->hauteur = max(hauteur(y->fg), hauteur(y->fd)) + 1;
    return y;
}

static NoeudAVL* equilibrer(NoeudAVL* noeud) {
    int eq = facteurEquilibre(noeud);
    if (eq < -1 && facteurEquilibre(noeud->fg) <= 0)
        return rotationDroite(noeud);
    if (eq < -1 && facteurEquilibre(noeud->fg) > 0) {
        noeud->fg = rotationGauche(noeud->fg);
        return rotationDroite(noeud);
    }
    if (eq > 1 && facteurEquilibre(noeud->fd) >= 0)
        return rotationGauche(noeud);
    if (eq > 1 && facteurEquilibre(noeud->fd) < 0) {
        noeud->fd = rotationDroite(noeud->fd);
        return rotationGauche(noeud);
    }
    return noeud;
}

NoeudAVL* inserer(NoeudAVL* racine, int identifiant, long capacite, long consommation) {
    if (racine == NULL) return creerNoeud(identifiant, capacite, consommation);

    if (identifiant < racine->identifiant)
        racine->fg = inserer(racine->fg, identifiant, capacite, consommation);
    else if (identifiant > racine->identifiant)
        racine->fd = inserer(racine->fd, identifiant, capacite, consommation);
    else {
        // Station déjà présente : on met à jour la consommation
        racine->consommation += consommation;
        return racine;
    }

    racine->hauteur = 1 + max(hauteur(racine->fg), hauteur(racine->fd));

    return equilibrer(racine);
}

NoeudAVL* rechercher(NoeudAVL* racine, int identifiant) {
    if (!racine || racine->identifiant == identifiant) return racine;
    if (identifiant < racine->identifiant) return rechercher(racine->fg, identifiant);
    return rechercher(racine->fd, identifiant);
}

void libererAVL(NoeudAVL* noeud) {
    if (noeud) {
        libererAVL(noeud->fg);
        libererAVL(noeud->fd);
        free(noeud);
    }
}

// Nouvelle fonction pour compter les noeuds
static void count_nodes(NoeudAVL* n, int* compteur) {
    if (!n) return;
    (*compteur)++;
    count_nodes(n->fg, compteur);
    count_nodes(n->fd, compteur);
}

// Conversion de l'AVL en tableau
static void avl_to_array_infixe(NoeudAVL* racine, StationData* tab, int* index) {
    if (!racine) return;
    avl_to_array_infixe(racine->fg, tab, index);
    tab[*index].identifiant = racine->identifiant;
    tab[*index].capacite = racine->capacite;
    tab[*index].consommationTotale = racine->consommation;
    (*index)++;
    avl_to_array_infixe(racine->fd, tab, index);
}

StationData* avl_to_array(NoeudAVL* racine, int* count) {
    int compteur = 0;

    // Compter les noeuds dans l'arbre
    count_nodes(racine, &compteur);

    StationData* tab = NULL;
    if (compteur > 0) {
        tab = malloc(sizeof(StationData) * compteur);
        if (!tab) exit(1);  // Échec allocation mémoire
        int idx = 0;
        avl_to_array_infixe(racine, tab, &idx);
    }
    *count = compteur;
    return tab;
}

int comparer_par_capacite(const void* a, const void* b) {
    const StationData* A = (const StationData*)a;
    const StationData* B = (const StationData*)b;
    if (A->capacite < B->capacite) return -1;
    if (A->capacite > B->capacite) return 1;
    return 0;
}
