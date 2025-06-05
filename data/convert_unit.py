import numpy as np
import pandas as pd
import math 

def convert_units(df):
    # Facteurs de conversion
    G_TO_MS2 = 9.80665  # 1 G = 9.80665 m/s^2
    DEG_TO_RAD = math.pi / 180  # 1 degré = pi/180 radians
    
    # Appliquer les conversions
    for column in df.columns:
        if column.startswith('acc'):
            #print(f"Avant conversion ({column}): {df[column].head()}")
            df[column] = df[column] * G_TO_MS2
            #print(f"Après conversion ({column}): {df[column].head()}")
        elif column.startswith('gy'):
            #print(f"Avant conversion ({column}): {df[column].head()}")
            df[column] = df[column] * DEG_TO_RAD
            #print(f"Après conversion ({column}): {df[column].head()}")
        else :
            print(f"La colonne {column} n'a pas été convertie.")

    return df

data_set_0_val_manquante_path = 'dataset_complet__sans_valeurs_manquantes_.csv'
data_set_val_manquante_path = 'dataset_complet__avec_valeurs_manquantes_.csv'
sep=","

data_set_0_val_manquante = pd.read_csv(data_set_0_val_manquante_path, sep=sep, engine='python')
data_set_val_manquante = pd.read_csv(data_set_val_manquante_path, sep=sep, engine='python')
# Convertir les unités
data_set_0_val_manquante_converted = convert_units(data_set_0_val_manquante)
data_set_val_manquante_converted = convert_units(data_set_val_manquante)

# Sauvegarder le dataset converti
data_set_0_val_manquante_converted.to_csv('data_set_0_val_manquante_converted.csv', index=False)
data_set_val_manquante_converted.to_csv('data_set_val_manquante_converted.csv', index=False)

print("La conversion des unités a été effectuée et les données ont été sauvegardées dans 'dataset_convert.csv'.")