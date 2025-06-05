import serial
import csv
import time

# Configuration du port série
arduino_port = "/dev/tty.usbmodem34B7DA6371802"  # Remplacez par le port série de votre Arduino (par exemple COM3 sous Windows ou /dev/ttyUSB0 sous Linux/Mac)
baud_rate = 115200  # Assurez-vous que le taux de transmission est le même que celui défini dans l'Arduino

NB_SAMPLE = 200  # Nombre d'échantillons à lire à chaque fois
#NB_SAMPLE = 286  # Nombre d'échantillons à lire à chaque fois
SAMPLE_RATE = 0.03  # Taux d'échantillonnage en Hz
#SAMPLE_RATE = 0.021  # Taux d'échantillonnage en Hz
RECORD_DURATION = 6  # Durée d'enregistrement en secondes

# Ouvrir la connexion série
ser = serial.Serial(arduino_port, baud_rate)
time.sleep(2)  # Attendre que la connexion série soit établie

# Nom du fichier CSV où les données seront enregistrées
filename = "../data/ours/output-dataset.csv"

# Ouvrir le fichier CSV en mode écriture
with open(filename, mode='w', newline='') as file:
    writer = csv.writer(file)
        
    # Écrire l'en-tête du fichier CSV
    header = ["label"]
    for i in range(NB_SAMPLE):
        header += [f"acc_x{i}", f"acc_y{i}", f"acc_z{i}", f"gy_x{i}", f"gy_y{i}", f"gy_z{i}"]
    writer.writerow(header)

    print("Enregistrement des données...")

    # Lire les données envoyées par l'Arduino et les enregistrer dans le fichier CSV
    data_buffer = []
    start_time = time.time()
    recording_started = False

    while time.time() - start_time < RECORD_DURATION:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()  # Lire une ligne de données
            if line:
                # Diviser la ligne en valeurs et vérifier la première valeur
                data = line.split(',')
                if data[0].startswith("acc_x :"):
                    # Supprimer les préfixes et ajouter les valeurs au buffer
                    cleaned_data = [value.split(':')[1] for value in data]
                    data_buffer.append(cleaned_data)
                    print(f"Enregistrement des données: {cleaned_data}")
            time.sleep(SAMPLE_RATE)  # Attendre 30 ms avant de prendre le prochain échantillon

    label = 0

    # Écrire les données dans le fichier CSV
    for i in range(0, len(data_buffer), 200):
        row = [label]
        for j in range(200):
            if i + j < len(data_buffer):
                row.extend(data_buffer[i + j])
            else:
                row.extend([0] * 6)  # Remplir avec des zéros si les données sont insuffisantes
        writer.writerow(row)

    print("Fin de l'enregistrement pour cette période de 6 secondes.")