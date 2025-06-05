#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Arduino_LED_Matrix.h"

// Création d'une instance du capteur
Adafruit_MPU6050 mpu;
ArduinoLEDMatrix matrix;

// Seuils de détection
#define G_THRESHOLD 12.0             // Accélération en m/s² (≈ 1.5g)
#define ROTATION_THRESHOLD 0.08      // Rotation en rad/s
#define FALL_DURATION_THRESHOLD 0.2  // Durée minimale d'une chute en secondes

// Fréquence d'échantillonnage
#define SAMPLING_RATE 0.01          // 100 Hz

// Variables globales
bool fall_detected = false;
unsigned long fall_start_time = 0;

const uint32_t happy[] = {
    0x19819,
    0x80000001,
    0x81f8000
};
const uint32_t heart[] = {
    0x3184a444,
    0x44042081,
    0x100a0040
};

void setup() {
  Serial.begin(9600);
  Serial.println("Initialisation du MPU6050...");
  matrix.begin();
  matrix.loadFrame(happy);

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
  // Lire les données du capteur
  matrix.loadFrame(happy);
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  // Extraire les valeurs d'accélération (m/s²) et de rotation (rad/s)
  float ax = accel.acceleration.x;
  float ay = accel.acceleration.y;
  float az = accel.acceleration.z;

  float gx = radians(gyro.gyro.x);  // Conversion en rad/s
  float gy = radians(gyro.gyro.y);
  float gz = radians(gyro.gyro.z);

    // Envoyer les données via le port série (format CSV)
  Serial.print(ax); Serial.print("ax, ");
  Serial.print(ay); Serial.print("ay, ");
  Serial.print(az); Serial.print("az, ");
  Serial.print(gx); Serial.print("gx, ");
  Serial.print(gy); Serial.print("gy, ");
  Serial.print(gz); Serial.println("gz");

  delay(1); // Pause de 12ms entre chaque lecture
}