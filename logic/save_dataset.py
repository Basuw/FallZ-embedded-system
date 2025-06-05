import serial
import time
import pandas as pd
from scipy.stats import skew, kurtosis

# Configuration du port série
arduino_port = "/dev/tty.usbmodem34B7DA6371802"  # Remplacez par votre port série
baud_rate = 115200

# Durée d'un enregistrement
recording_duration = 6  # en secondes

# Initialisation de la connexion série
ser = serial.Serial(arduino_port, baud_rate)
time.sleep(2)  # Attendre que la connexion soit établie

def read_data():
    """Lit une ligne de données depuis le port série."""
    line = ser.readline().decode('utf-8').strip()
    data = {}
    parts = line.split(', ')
    for part in parts:
        key, value = part.split(' :')
        data[key] = float(value)
    return data

def calculate_statistics(data):
    """Calcule les statistiques pour les données fournies."""
    stats = {}
    for axis in ['x', 'y', 'z']:
        for sensor in ['acc', 'gy']:
            key = f"{sensor}_{axis}"
            stats[f"{key}_min"] = min(data[key])
            stats[f"{key}_max"] = max(data[key])
            stats[f"{key}_skewness"] = skew(data[key])
            stats[f"{key}_kurtosis"] = kurtosis(data[key])
    return stats

def main():
    # Initialisation du DataFrame pour sauvegarder les données
    columns = [
        'label', 'acc_x_min', 'acc_y_min', 'acc_z_min', 'acc_x_max', 'acc_y_max', 'acc_z_max',
        'acc_x_skewness', 'acc_y_skewness', 'acc_z_skewness', 'acc_x_kurtosis', 'acc_y_kurtosis', 'acc_z_kurtosis',
        'gy_x_min', 'gy_y_min', 'gy_z_min', 'gy_x_max', 'gy_y_max', 'gy_z_max',
        'gy_x_skewness', 'gy_y_skewness', 'gy_z_skewness', 'gy_x_kurtosis', 'gy_y_kurtosis', 'gy_z_kurtosis'
    ]
    df = pd.DataFrame(columns=columns)

    while True:
        # Initialisation des listes pour stocker les données
        data = {
            'acc_x': [], 'acc_y': [], 'acc_z': [],
            'gy_x': [], 'gy_y': [], 'gy_z': []
        }

        start_time = time.time()
        while time.time() - start_time < recording_duration:
            sample = read_data()
            for key in data.keys():
                data[key].append(sample[key])

        # Calcul des statistiques
        stats = calculate_statistics(data)

        # Demander si une chute a eu lieu
        label = int(input("Y a-t-il eu une chute ? (1 pour oui, 0 pour non) : "))
        stats['label'] = label

        # Ajouter les statistiques au DataFrame
        df = pd.concat([df, pd.DataFrame([stats])], ignore_index=True)

        # Sauvegarder les données dans un fichier CSV
        df.to_csv('../data/data.csv', index=False)

        # Demander si on continue
        continue_recording = input("Voulez-vous continuer les enregistrements ? (o/n) : ")
        if continue_recording != 'o':
            break

if __name__ == "__main__":
    main()
