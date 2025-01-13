#include <MKRWAN.h>
#include <Adafruit_Sensor.h>
#include <TinyGPS++.h>
#include <Arduino.h>
#include "arduino_secrets.h"

LoRaModem modem;

// Créer un objet pour la bibliothèque GNSS
TinyGPSPlus gps;

// Variables pour les données GPS
float latitude, longitude, altitude;

unsigned long previousMillis = 0;  // Temps du dernier envoi
const long interval = 60000;  // Intervalle de 60 secondes (en millisecondes)

// Déclarations des fonctions
void setup();
void loop();
void sendGPSviaLora(TinyGPSPlus gps);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);

  while (!Serial);
  while (!Serial1);
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

  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independent of this setting, the modem will
  // not allow sending more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
}

void loop() {
    unsigned long currentMillis = millis();
  // Lecture des données GPS
    while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
    
    // Si on a une lecture complète du GPS
    if (currentMillis - previousMillis >= interval && gps.location.isUpdated()) {
        previousMillis = currentMillis;
        sendGPSviaLora(gps);
    }
  }
}

void sendGPSviaLora(TinyGPSPlus gps) {
    if (gps.location.isValid()) {
          int err;
        modem.beginPacket();
        String gpsLocation = "lat : " + String(gps.location.lat(), 6) + "; long : " + String(gps.location.lng(), 6);
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