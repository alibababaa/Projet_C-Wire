#include "avl_tree.h"


void erreurCritique(const char* message) {
    fprintf(stderr, "Erreur : %s\n", message);
    exit(1);
}

// Suppression du paramètre station_type si inutile
void process_csv(const char* filename, const char* output_filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        erreurCritique(strerror(errno));
    }

    NoeudAVL* racine = NULL;
    char ligne[1024];

    while (fgets(ligne, sizeof(ligne), file)) {
        char* tokens[8];
        int i=0;
        char* p=strtok(ligne, ";");
        while(p && i<8) {
            tokens[i++]=p;
            p=strtok(NULL,";");
        }
        if (i<8) continue;

        int identifiant = atoi(tokens[0]);
        long capacite = (tokens[6] && tokens[6][0] != '-') ? atol(tokens[6]) : 0;
        long consommation = (tokens[7] && tokens[7][0] != '-') ? atol(tokens[7]) : 0;

        if (capacite > 0 && consommation == 0) {
            racine = inserer(racine, identifiant, capacite, 0);
        } else if (consommation > 0) {
            NoeudAVL* noeud = rechercher(racine, identifiant);
            if (noeud) {
                noeud->consommation += consommation;
            } else {
                racine = inserer(racine, identifiant, 0, consommation);
            }
        }
    }

    fclose(file);

    int count=0;
    StationData* stations = avl_to_array(racine, &count);
    if (!stations && count>0) {
        libererAVL(racine);
        erreurCritique("Impossible de convertir l'AVL en tableau");
    }

    qsort(stations, count, sizeof(StationData), comparer_par_capacite);

    FILE* fichierSortie = fopen(output_filename, "w");
    if (!fichierSortie) {
        libererAVL(racine);
        free(stations);
        erreurCritique("Erreur création fichier de sortie");
    }

    fprintf(fichierSortie, "Identifiant:Consommation Totale (kWh)\n");
    for (int i = 0; i < count && i < 10; i++) {
        fprintf(fichierSortie, "%d:%ld\n", stations[i].identifiant, stations[i].consommationTotale);
    }

    fclose(fichierSortie);
    libererAVL(racine);
    free(stations);
}
