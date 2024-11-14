#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>
#include "GNSS-L86-M33-SOLDERED.h" // Include L86-L33 GNSS Library

// Définir les broches RX et TX pour le module GNSS
#define GNSS_RX 2  // Broche RX (n'importe quelle broche, ici 3 pour l'exemple)
#define GNSS_TX 2  // Broche TX (n'importe quelle broche, ici 1 pour l'exemple)

// Créer un objet pour la bibliothèque GNSS
GNSS gps(GNSS_TX, GNSS_RX);

// Variable pour suivre le dernier affichage des données GNSS
unsigned long lastGnssDisplay = 0;

// Déclarations des fonctions
void setup();
void loop();
void displayInfo();

// Fonction setup - exécutée une fois au démarrage
void setup() {
    // Initialiser la communication série pour le moniteur série à 115200 bauds
    Serial.begin(115200);  // Utilisez un baud rate plus élevé si nécessaire

    // Initialiser la communication avec le module GNSS via Serial (TX0/RX0)
    gps.begin(); // Le GPS va utiliser les broches définies par default (TX0 et RX0)
}

// Fonction loop - exécutée en boucle
void loop() {
    // Si des données sont reçues sur l'UART du GNSS, lire et envoyer chaque caractère à la bibliothèque
    while (gps.gnssSerial->available() > 0) {
        // Si quelque chose est correctement décodé, afficher les nouvelles données.
        if (gps.encode(gps.gnssSerial->read())) {
            // Vérifier si 500 millisecondes se sont écoulées depuis le dernier affichage des données.
            if ((unsigned long)(millis() - lastGnssDisplay) > 500UL) {
                // Capturer un nouveau timestamp.
                lastGnssDisplay = millis();

                // Afficher les nouvelles données.
                displayInfo();
            }
        }
    }

    // Pas de données dans les 5 premières secondes du démarrage ? Vérifier les câbles !
    if (millis() > 5000 && gps.charsProcessed() < 10) {
        Serial.println(F("Aucun GPS détecté : vérifier les câbles."));
        while (true) {
            // Un délai est nécessaire pour certains chips comme l'ESP8266.
            delay(10);
        }
    }
}

// Fonction pour afficher les données décodées de la bibliothèque GNSS
void displayInfo() {
    // Afficher la latitude et la longitude GPS. Si aucune donnée valide, afficher un message d'erreur.
    Serial.print(F("Emplacement: "));
    if (gps.location.isValid()) {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
    } else {
        Serial.print(F("INVALIDE"));
    }

    // Afficher la date et l'heure. Si aucune donnée valide, afficher un message d'erreur.
    Serial.print(F("  Date/Heure: "));
    if (gps.date.isValid()) {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
    } else {
        Serial.print(F("INVALIDE"));
    }

    Serial.print(F(" "));
    if (gps.time.isValid()) {
        if (gps.time.hour() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
    } else {
        Serial.print(F("INVALIDE"));
    }

    // Aller à la nouvelle ligne, prêt à afficher de nouvelles données.
    Serial.println();
}