import re
import numpy as np

from env import (
    CPU_LOG_PATH,
    MEMORY_LOG_PATH,
    IO_LOG_PATH
)

data_line_re = r"^\d(\s).*$"
time_line_re = r"\d*\.\d* sec"
data_line_format = ["Core", "CPU", "Avg_MHz", "Busy%", "Bzy_MHz", 
                    "TSC_MHz", "IPC", "IRQ", "NMI", "SMI", 
                    "POLL%", "C1%", "C2%", "C3%", 
                    "CorWatt", "PkgWatt","Duration"]
experiments = { "CPU" : CPU_LOG_PATH,
                "MEMORY" : MEMORY_LOG_PATH,
                "IO" : IO_LOG_PATH}

# On initialise les colonnes avec d'abord les noms des types de données
columns_to_stack = [np.array(data_line_format)]
header_names = ["DataType"]

# On force l'ordre d'itération pour s'assurer que les colonnes soient toujours dans le même ordre (CPU, MEM, IO)
experiment_order = ["CPU", "MEMORY", "IO"]

# main loop :
for experiment_type in experiment_order:
    log_path = experiments.get(experiment_type)
    data_lists = [[] for _ in range(len(data_line_format))]
    has_data = False

    if log_path is not None:
        try:
            with open(log_path, "r") as f:
                for line in f:
                    if re.match(data_line_re, line):
                        has_data = True
                        # Nettoyer la ligne et séparer les valeurs
                        values = line.strip().split()
                        # Ajouter chaque valeur à la bonne liste
                        for idx, value in enumerate(values):
                            try:
                                data_lists[idx].append(float(value))
                            except ValueError:
                                data_lists[idx].append(np.nan)
                    elif re.match(time_line_re, line):
                        has_data = True
                        values = line.strip().split()
                        
                        try:
                            time_val = float(values[0])
                            data_lists[-1].append(time_val)
                        except ValueError:
                            data_lists[-1].append(np.nan)
                        
        except FileNotFoundError:
            print(f"Warning: File not found {log_path}")

    header_names.append(experiment_type)

    if has_data:
        # Conversion finale en np.array
        data_array = np.array(data_lists)
        # Calcul de la moyenne par ligne (axis=1) pour obtenir un vecteur colonne
        means = np.nanmean(data_array, axis=1)
        # On ajoute ce vecteur aux colonnes à assembler (arrondi à 2 décimales)
        columns_to_stack.append(np.round(means, 2))
    else:
        # Si pas de données, on remplit une colonne de NaN
        columns_to_stack.append(np.full(len(data_line_format), np.nan))

# Création du tableau final en empilant les colonnes
# Le résultat sera un tableau de chaînes de caractères car la première colonne contient du texte
final_table = np.column_stack(columns_to_stack)

print("\t".join(header_names))
print(final_table)

# Export en CSV
csv_filename = "resultats_moyens.csv"
# fmt='%s' est nécessaire car le tableau contient des chaînes de caractères
# comments='' empêche numpy d'ajouter un '#' au début de la ligne d'en-tête
np.savetxt(csv_filename, final_table, delimiter=",", header=",".join(header_names), fmt='%s', comments='')

print(f"\nDonnées exportées dans {csv_filename}")