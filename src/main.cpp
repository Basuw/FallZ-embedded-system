#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Arduino_LED_Matrix.h"

// Création d'une instance du capteur
Adafruit_MPU6050 mpu;

// Fréquence d'échantillonnage
//#define SAMPLING_RATE 0.021          // 48 Hz
#define SAMPLING_RATE 0.012         // 48real Hz

// Variables globales
bool fall_detected = false;
unsigned long fall_start_time = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Initialisation du MPU6050...");

  // Initialiser le MPU6050
  if (!mpu.begin(0x69)) {
    Serial.println("Erreur : impossible de détecter le MPU6050 !");
    while (1);
  }
  Serial.println("MPU6050 initialisé avec succès !");

  // Configurer les plages du capteur
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);  // Plage ±8g
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);       // Plage ±500°/s
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);    // Bande passante du filtre
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // m/s^2
  Serial.print("acc_x :"); Serial.print(a.acceleration.x);
  Serial.print(", acc_y :"); Serial.print(a.acceleration.y);
  Serial.print(", acc_z :"); Serial.print(a.acceleration.z);
  // rad/s
  Serial.print(", gy_x :"); Serial.print(g.gyro.x);
  Serial.print(", gy_y :"); Serial.print(g.gyro.y);
  Serial.print(", gy_z :"); Serial.println(g.gyro.z);


  delay(SAMPLING_RATE);
}