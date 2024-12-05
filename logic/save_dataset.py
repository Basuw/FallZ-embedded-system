import serial
import csv
import time

# Configuration du port série
arduino_port = "COM3"  # Remplacez par le port série de votre Arduino (par exemple COM3 sous Windows ou /dev/ttyUSB0 sous Linux/Mac)
baud_rate = 9600  # Assurez-vous que le taux de transmission est le même que celui défini dans l'Arduino

# Ouvrir la connexion série
ser = serial.Serial(arduino_port, baud_rate)
time.sleep(2)  # Attendre que la connexion série soit établie

# Nom du fichier CSV où les données seront enregistrées
filename = "data.csv"

# Ouvrir le fichier CSV en mode écriture
with open(filename, mode='w', newline='') as file:
    writer = csv.writer(file)
    
    # Écrire l'en-tête du fichier CSV
    writer.writerow(["Time", "AccelX", "AccelY", "AccelZ"])

    print("Enregistrement des données...")

    # Lire les données envoyées par l'Arduino et les enregistrer dans le fichier CSV
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()  # Lire une ligne de données
            if line:
                # Diviser la ligne en valeurs et les écrire dans le fichier CSV
                data = line.split(',')
                writer.writerow(data)
                print(f"Enregistrement des données: {data}")

                # Sortir après avoir enregistré suffisamment de données (par exemple, après 10 lignes)
                if len(data) == 4:
                    break

    print("Fin de l'enregistrement.")
    ser.close()