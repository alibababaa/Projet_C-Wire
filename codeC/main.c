#include "avl_tree.h"


int main(int argc, char** argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <filtered_csv> <station_type> <consumer_type> [central_id]\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    const char* station_type = argv[2];
    const char* consumer_type = argv[3];

    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    NoeudAVL* racine = NULL;
    char line[1024];

    while (fgets(line, sizeof(line), f)) {
        char* tokens[8];
        int i=0;
        char* p=strtok(line, ";");
        while (p && i<8) {
            tokens[i++] = p;
            p = strtok(NULL, ";");
        }
        if (i<8) continue;

        char* hvb = tokens[1];
        char* hva = tokens[2];
        char* lv  = tokens[3];
        char* capacity_str = tokens[6];
        char* load_str = tokens[7];

        long capacity = (capacity_str && capacity_str[0] != '-') ? atol(capacity_str) : 0;
        long load = (load_str && load_str[0] != '-') ? atol(load_str) : 0;

        int station_id = 0;
        if (strcmp(station_type, "hvb") == 0 && hvb && hvb[0] != '-') {
            station_id = atoi(hvb);
        } else if (strcmp(station_type, "hva") == 0 && hva && hva[0] != '-') {
            station_id = atoi(hva);
        } else if (strcmp(station_type, "lv") == 0 && lv && lv[0] != '-') {
            station_id = atoi(lv);
        }

        if (capacity > 0 && load == 0) {
            // C'est une station
            racine = inserer(racine, station_id, capacity, 0);
        } else if (load > 0) {
            // C'est un consommateur
            NoeudAVL* noeud = rechercher(racine, station_id);
            if (noeud) {
                noeud->consommation += load;
            } else {
                racine = inserer(racine, station_id, 0, load);
            }
        }
    }

    fclose(f);

    int count = 0;
    StationData* array = avl_to_array(racine, &count);
    if (!array && count > 0) {
        libererAVL(racine);
        fprintf(stderr, "Erreur: impossible de convertir l'AVL en tableau.\n");
        return 1;
    }

    qsort(array, count, sizeof(StationData), comparer_par_capacite);

    // Affichage de l'en-tête (corrigé)
    // On place chaque argument du printf sur une nouvelle ligne pour plus de clarté
    printf("%s:%s:%s\n",
        ((strcmp(station_type,"hvb")==0) ? "Station HVB" :
         ((strcmp(station_type,"hva")==0) ? "Station HVA" : "Station LV")),
        "Capacité",
        ((strcmp(consumer_type,"comp")==0) ? "Consommation (entreprises)" :
         ((strcmp(consumer_type,"indiv")==0) ? "Consommation (particuliers)" : "Consommation (tous)"))
    );

    for (int j = 0; j < count; j++) {
        printf("%d:%ld:%ld\n", array[j].identifiant, array[j].capacite, array[j].consommationTotale);
    }

    free(array);
    libererAVL(racine);
    return 0;
}
