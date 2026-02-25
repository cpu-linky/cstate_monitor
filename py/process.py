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
                    "CorWatt", "PkgWatt","Duration", "Duration_Std"]
experiments = { "CPU" : CPU_LOG_PATH,
                "MEMORY" : MEMORY_LOG_PATH,
                "IO" : IO_LOG_PATH}

# On initialise les colonnes avec d'abord les noms des types de données
# Note: On garde data_line_format pour l'affichage, mais on utilisera une taille adaptée pour le parsing
# Les métriques parsées vont jusqu'à 'PkgWatt', ensuite c'est 'Duration' qui est traité à part
RAW_METRICS_COUNT = 16 # De Core à PkgWatt
columns_to_stack = [np.array(data_line_format)]
header_names = ["DataType"]

# On force l'ordre d'itération pour s'assurer que les colonnes soient toujours dans le même ordre (CPU, MEM, IO)
experiment_order = ["CPU", "MEMORY", "IO"]

# main loop :
for experiment_type in experiment_order:
    log_path = experiments.get(experiment_type)
    data_lists = [[] for _ in range(RAW_METRICS_COUNT + 1)]
    has_data = False

    if log_path is not None:
        try:
            with open(log_path, "r") as f:
                for line in f:
                    if re.match(data_line_re, line):
                        has_data = True
                        # Nettoyer la ligne et séparer les valeurs
                        values = line.strip().split()
                        # Ajouter chaque valeur à la bonne liste (seulement les métriques standards)
                        for idx, value in enumerate(values):
                            if idx < RAW_METRICS_COUNT:
                                try:
                                    data_lists[idx].append(float(value))
                                except ValueError:
                                    data_lists[idx].append(np.nan)
                    elif re.match(time_line_re, line):
                        has_data = True
                        values = line.strip().split()
                        
                        try:
                            time_val = float(values[0])
                            # La durée est stockée à l'index RAW_METRICS_COUNT (16)
                            data_lists[RAW_METRICS_COUNT].append(time_val)
                        except ValueError:
                            data_lists[RAW_METRICS_COUNT].append(np.nan)
                        
        except FileNotFoundError:
            print(f"Warning: File not found {log_path}")

    header_names.append(experiment_type)

    if has_data:    
        means = []
        # 1. Moyennes des métriques standards
        for i in range(RAW_METRICS_COUNT):
            if data_lists[i]:
                means.append(np.nanmean(data_lists[i]))
            else:
                means.append(np.nan)
                
        # 2. Moyenne et Ecart-type de la durée
        duration_data = data_lists[RAW_METRICS_COUNT]
        if duration_data:
            duration_mean = np.nanmean(duration_data)
            duration_std = np.nanstd(duration_data)
        else:
            duration_mean = np.nan
            duration_std = np.nan
            
        means.append(duration_mean)
        means.append(duration_std)
        
        # On ajoute ce vecteur aux colonnes à assembler
        columns_to_stack.append(np.round(means, 3))
    else:
        columns_to_stack.append(np.full(len(data_line_format), np.nan))

final_table = np.column_stack(columns_to_stack)

print("\t".join(header_names))
print(final_table)

# Export en CSV
csv_filename = "resultats_moyens.csv"
np.savetxt(csv_filename, final_table, delimiter=",", header=",".join(header_names), fmt='%s', comments='')

print(f"\nDonnées exportées dans {csv_filename}")