/*
  Lora Send And Receive
  This sketch demonstrates how to send and receive data with the MKR WAN 1300/1310 LoRa module.
  This example code is in the public domain.
*/

#include <MKRWAN.h>
#include <Arduino.h>
#include "GNSS-L86-M33-SOLDERED.h"

LoRaModem modem;

// Uncomment if using the Murata chip as a module
// LoRaModem modem(Serial1);

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
#include "arduino_secrets.h"
String appEui = appEui;
String appKey = appKey;

// Définir les broches RX et TX pour le module GNSS
#define GNSS_RX 13  // Broche RX (n'importe quelle broche, ici 3 pour l'exemple)
#define GNSS_TX 14  // Broche TX (n'importe quelle broche, ici 1 pour l'exemple)

// Créer un objet pour la bibliothèque GNSS
GNSS gps(GNSS_TX, GNSS_RX);

// Variable pour suivre le dernier affichage des données GNSS
unsigned long lastGnssDisplay = 0;

// Déclarations des fonctions
void setup();
void loop();
void displayInfo();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }

  gps.begin();
  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independent of this setting, the modem will
  // not allow sending more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
}

void loop() {

  // Si des données sont reçues sur l'UART du GNSS
    while (gps.gnssSerial->available() > 0) {
        // Lire un caractère brut depuis le module GNSS
        char c = gps.gnssSerial->read();
        
        // Passer les données au décodeur GNSS
        if (gps.encode(c)) {
            // Si 500 ms se sont écoulées, afficher les nouvelles données décodées
            if ((unsigned long)(millis() - lastGnssDisplay) > 500UL) {
                lastGnssDisplay = millis();
                sendGPSviaLora();
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

void sendGPSviaLora() {
    if (gps.location.isValid()) {
          int err;
        modem.beginPacket();
        String gpsLocation = "lat : " + gps.location.lat() + "; long : " + gps.location.lng();
        Serial.println("message GPS : " + gpsLocation);
        modem.print(gpsLocation);
        err = modem.endPacket(true);
        if (err > 0) {
          Serial.println("Message sent correctly!");
        } else {
          Serial.println("Error sending message : ");
        }
    } else {
        Serial.println(gps.location.lat(), 6);
        Serial.print(F("INVALIDE"));
    }
}