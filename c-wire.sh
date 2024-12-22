#!/usr/bin/env bash

# =======================
# Fonction d'affichage de l'aide
# =======================
function show_help {
    echo "Usage: $0 -f <data_file> -s <station_type> -c <consumer_type> [-i <central_id>] [-o <output_file>] [-h]"
    echo "  -f  chemin du fichier CSV d'entrée"
    echo "  -s  type de station: hvb | hva | lv"
    echo "  -c  type de consommateur: comp | indiv | all"
    echo "  -i  identifiant spécifique d'une centrale (optionnel)"
    echo "  -o  fichier de sortie (optionnel) sinon utilise un nom par défaut"
    echo "  -h  afficher cette aide"
    echo ""
    echo "Restrictions :"
    echo "  hvb : consommateur = comp uniquement (pas all, pas indiv)"
    echo "  hva : consommateur = comp uniquement"
    echo "  lv  : consommateur = comp, indiv, ou all"
    echo ""
    echo "Exemples:"
    echo "$0 -f input/data.csv -s hvb -c comp -o tests/hvb_comp.csv"
}

# =======================
# Fonction d'erreur critique
# =======================
function erreurCritique {
    echo "Erreur : $1"
    show_help
    echo "Durée du traitement : 0.0 sec"
    exit 1
}

# =======================
# Initialisation des variables
# =======================
input_file=""
station_type=""
consumer_type=""
central_id=""
output_file=""
help=false

# =======================
# Lecture des options
# =======================
while getopts "f:s:c:i:o:h" opt; do
    case $opt in
        f) input_file=$OPTARG ;;
        s) station_type=$OPTARG ;;
        c) consumer_type=$OPTARG ;;
        i) central_id=$OPTARG ;;
        o) output_file=$OPTARG ;;
        h) help=true ;;
        *)
            echo "Option invalide : -$OPTARG"
            show_help
            exit 1
            ;;
    esac
done

if $help; then
    show_help
    exit 0
fi

# =======================
# Vérification des options obligatoires
# =======================
[ -z "$input_file" ] && erreurCritique "L'option -f (fichier d'entrée) est obligatoire."
[ -z "$station_type" ] && erreurCritique "L'option -s (type de station) est obligatoire."
[ -z "$consumer_type" ] && erreurCritique "L'option -c (type de consommateur) est obligatoire."

# Vérification cohérence des choix station/consommateur
if [ "$station_type" = "hvb" ] && { [ "$consumer_type" = "all" ] || [ "$consumer_type" = "indiv" ]; }; then
    erreurCritique "Pour hvb, seuls les consommateurs comp sont autorisés."
fi

if [ "$station_type" = "hva" ] && { [ "$consumer_type" = "all" ] || [ "$consumer_type" = "indiv" ]; }; then
    erreurCritique "Pour hva, seuls les consommateurs comp sont autorisés."
fi

# =======================
# Vérification du fichier d'entrée
# =======================
if [ ! -f "$input_file" ]; then
    erreurCritique "Le fichier d'entrée $input_file est introuvable."
fi

# =======================
# Vérification / Compilation du programme C
# =======================
if [ ! -x codeC/program ]; then
    echo "Programme C introuvable ou non exécutable. Compilation en cours avec 'make'..."
    (cd codeC && make)
    if [ $? -ne 0 ]; then
        erreurCritique "Échec de la compilation avec 'make'."
    fi
    echo "Compilation réussie."
fi

# =======================
# Création/réinitialisation des répertoires
# =======================
mkdir -p tmp || erreurCritique "Impossible de créer le dossier tmp."
mkdir -p graphs
mkdir -p tests
rm -f tmp/*

# =======================
# Mesure du temps de traitement
# =======================
start_time=$(date +%s.%N)

# =======================
# Filtrage du fichier CSV
# =======================
separator=";"
filtered_file="tmp/filtered.csv"

awk -F"$separator" -v st="$station_type" -v ct="$consumer_type" -v cid="$central_id" '
BEGIN { OFS=";" }
NR == 1 {
    if (index($1,"Power plant") !=0 ) { next }
}
{
    station_hvb = ($2 != "-" && $2 != "")
    station_hva = ($3 != "-" && $3 != "")
    station_lv  = ($4 != "-" && $4 != "")

    comp_present = ($5 != "-" && $5 != "")
    indiv_present= ($6 != "-" && $6 != "")
    capacity     = ($7 != "-" && $7 != "") ? $7 : ""
    load         = ($8 != "-" && $8 != "") ? $8 : ""

    if (cid != "" && $1 != cid) { next }

    if (st == "hvb") {
        if (station_hvb && capacity != "" && (load == "")) {
            print $0
        } else if (station_hvb && comp_present && load != "") {
            if (ct == "comp") print $0
        }
    } else if (st == "hva") {
        if (station_hva && capacity != "" && load == "") {
            print $0
        } else if (station_hva && comp_present && load != "") {
            if (ct == "comp") print $0
        }
    } else if (st == "lv") {
        if (station_lv && capacity != "" && load == "") {
            print $0
        } else if (station_lv && load != "") {
            if (ct == "comp" && comp_present) print $0
            else if (ct == "indiv" && indiv_present) print $0
            else if (ct == "all") print $0
        }
    }
}
' "$input_file" > "$filtered_file"

if [ ! -s "$filtered_file" ]; then
    echo "Aucune donnée filtrée trouvée pour les paramètres spécifiés."
    end_time=$(date +%s.%N)
    elapsed_time=$(echo "$end_time - $start_time" | bc)
    echo "Durée du traitement : $elapsed_time sec"
    exit 0
fi

# =======================
# Exécuter le programme C
# =======================
if [ -n "$central_id" ]; then
    codeC/program "$filtered_file" "$station_type" "$consumer_type" "$central_id" > tmp/result.csv
else
    codeC/program "$filtered_file" "$station_type" "$consumer_type" > tmp/result.csv
fi

if [ $? -ne 0 ]; then
    echo "Erreur lors de l'exécution du programme C."
    end_time=$(date +%s.%N)
    elapsed_time=$(echo "$end_time - $start_time" | bc)
    echo "Durée du traitement : $elapsed_time sec"
    exit 1
fi

# =======================
# Détermination du nom du fichier final
# =======================
final_name="${station_type}_${consumer_type}"
[ -n "$central_id" ] && final_name="${final_name}_${central_id}"

if [ -z "$output_file" ]; then
    output_file="tests/${final_name}.csv"
fi

# Tri par capacité (on suppose la capacité est en 2ème colonne)
header=$(head -n1 tmp/result.csv)
tail -n +2 tmp/result.csv | sort -t":" -k2n > tmp/result_sorted.csv

echo "$header" > "$output_file"
cat tmp/result_sorted.csv >> "$output_file"

# Si lv_all, générer lv_all_minmax.csv
if [ "$station_type" = "lv" ] && [ "$consumer_type" = "all" ]; then
    tail -n +2 "$output_file" | awk -F":" '{
        diff=$2-$3;
        print $1":"$2":"$3":"diff
    }' | sort -t":" -k4n > tmp/lv_all_diff.csv

    minmax_file="tests/lv_all_minmax.csv"
    echo "Station:Capacité:Consommation:Diff" > "$minmax_file"
    head -n 10 tmp/lv_all_diff.csv >> "$minmax_file"
    tail -n 10 tmp/lv_all_diff.csv >> "$minmax_file"

    # Séparer les postes les plus chargés et les moins chargés
    pluscharge_file="tmp/10pluscharge.csv"
    moinscharge_file="tmp/10moinscharge.csv"

    head -n 10 tmp/lv_all_diff.csv > "$pluscharge_file"
    tail -n 10 tmp/lv_all_diff.csv > "$moinscharge_file"

    # Inverser les valeurs pour les barres vers le haut (valeurs négatives doivent être positives)
    awk -F":" 'BEGIN {OFS=":"} {if ($4 < 0) $4=-$4; print $0}' "$pluscharge_file" > tmp/10pluscharge_adjusted.csv
    awk -F":" 'BEGIN {OFS=":"} {if ($4 < 0) $4=-$4; print $0}' "$moinscharge_file" > tmp/10moinscharge_adjusted.csv

    # Générer le graphique avec gnuplot
    if command -v gnuplot &>/dev/null; then
        gnuplot <<EOF
set terminal png size 800,600
set output "graphs/lv_all_minmax.png"
set datafile separator ":"
set title "Top 10 Plus Chargés et Moins Chargés LV Stations"
set xlabel "Stations"
set ylabel "Diff (Capacité-Consommation)"
set style data histograms
set style fill solid border -1
set boxwidth 0.9
set xtics rotate by -45

# Graphique pour les 10 plus chargés (vert) et les 10 moins chargés (rouge)
plot "tmp/10pluscharge_adjusted.csv" using 4:xtic(1) title '10 Plus Chargés' lc rgb "green", \
     "tmp/10moinscharge_adjusted.csv" using 4:xtic(1) title '10 Moins Chargés' lc rgb "red"
EOF
        echo "Graphique généré : graphs/lv_all_minmax.png"
    else
        echo "Gnuplot non installé, pas de graphique généré."
    fi
fi # Fermeture du dernier bloc 'if'

# Fin du traitement
end_time=$(date +%s%N)
elapsed_time=$(echo "scale=3; ($end_time - $start_time) / 1000000000" | bc)

# Si le temps est trop court, définissez une durée minimale
if (( $(echo "$elapsed_time < 0.001" | bc -l) )); then
    elapsed_time="0.001"
fi

echo "Traitement terminé."
echo "Durée du traitement : $elapsed_time sec"
