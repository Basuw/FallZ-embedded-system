#include <DFRobot_BMP3XX.h>

DFRobot_BMP388_I2C sensor(&Wire, sensor.eSDOVDD);
//DFRobot_BMP390L_I2C sensor(&Wire, sensor.eSDOVDD);

#define CALIBRATE_ABSOLUTE_DIFFERENCE
#define SEUIL_CHUTE 0.1  // Seuil d'altitude pour détecter une chute (en mètres)
#define FALL_THRESHOLD 3  // Nombre de mesures consécutives
#define WINDOW_SIZE 10

float altitude_buffer[WINDOW_SIZE];
int buffer_index = 0;
float last_altitude = 0;
int fall_counter = 0;

void setup(void)
{
  Serial.begin(9600);

  int rslt;
  while( ERR_OK != (rslt = sensor.begin()) ){
    if(ERR_DATA_BUS == rslt){
      Serial.println("Data bus error!!!");
    }else if(ERR_IC_VERSION == rslt){
      Serial.println("Chip versions do not match!!!");
    }
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   * 6 commonly used sampling modes that allows users to configure easily, mode:
   *      eUltraLowPrecision, Ultra-low precision, suitable for monitoring weather (lowest power consumption), the power is mandatory mode.
   *      eLowPrecision, Low precision, suitable for random detection, power is normal mode
   *      eNormalPrecision1, Normal precision 1, suitable for dynamic detection on handheld devices (e.g on mobile phones), power is normal mode.
   *      eNormalPrecision2, Normal precision 2, suitable for drones, power is normal mode.
   *      eHighPrecision, High precision, suitable for low-power handled devices (e.g mobile phones), power is in normal mode.
   *      eUltraPrecision, Ultra-high precision, suitable for indoor navigation, its acquisition rate will be extremely low, and the acquisition cycle is 1000 ms.
   */
  while( !sensor.setSamplingMode(sensor.eUltraPrecision) ){
    Serial.println("Set samping mode fail, retrying....");
    delay(3000);
  }

  delay(100);
  #ifdef CALIBRATE_ABSOLUTE_DIFFERENCE

  // Need to edit current altitude to match the location, 540 meters in Wenjiang District of Chengdu (China)
  // It will be an issue if the sensor is moved to a different location without updating the altitude
  if( sensor.calibratedAbsoluteDifference(540.0) ){
    Serial.println("Absolute difference base value set successfully!");
  }
  #endif

  /* Get the sampling frequency of the current measurement mode, unit: Hz */
  float sampingPeriodus = sensor.getSamplingPeriodUS();
  float sampingFrequencyHz = 1000000 / sampingPeriodus;
  Serial.print("samping frequency : ");
  Serial.print(sampingFrequencyHz);
  Serial.println(" Hz");

  Serial.println();
  delay(1000);
}

void addToBuffer(float value) {
  altitude_buffer[buffer_index] = value;
  buffer_index = (buffer_index + 1) % WINDOW_SIZE;
}

float getAverageAltitude() {
  float sum = 0;
  for (int i = 0; i < WINDOW_SIZE; i++) {
    sum += altitude_buffer[i];
  }
  return sum / WINDOW_SIZE;
}

void loop() {
  float altitude = sensor.readAltitudeM();  // Lire l'altitude actuelle
  addToBuffer(altitude);
  float smoothed_altitude = getAverageAltitude();

  float diff = last_altitude - smoothed_altitude;

  if (diff > SEUIL_CHUTE) {
    fall_counter++;
    if (fall_counter >= FALL_THRESHOLD) {
      Serial.println("Chute détectée !");
      fall_counter = 0;
    }
  } else {
    fall_counter = 0;
  }

  delay(100);
  last_altitude = smoothed_altitude;
}
