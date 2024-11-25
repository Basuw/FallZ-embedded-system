#include <DFRobot_BMP3XX.h>

DFRobot_BMP388_I2C sensor(&Wire, sensor.eSDOVDD);
//DFRobot_BMP390L_I2C sensor(&Wire, sensor.eSDOVDD);

#define CALIBRATE_ABSOLUTE_DIFFERENCE
#define SEUIL_CHUTE 0.3 

float last_altitude;

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

void loop()
{
  /* Directly read the currently measured pressure data, unit: pa */
  /*
  float Pressure = sensor.readPressPa();
  Serial.print("Pressure : ");
  Serial.print(Pressure);
  Serial.println(" Pa");
  */

  /* Read altitude, unit: m */
  float diff;
  float altitude = sensor.readAltitudeM();

 /*
  Serial.print("Altitude : ");
  Serial.print(altitude);
  Serial.println(" m");
*/
  diff = last_altitude - altitude;
  // Serial.println(diff);
  
  if(diff > SEUIL_CHUTE)
  {
    Serial.println("chute");
  }
    
  delay(100);
  last_altitude = altitude;
  
}
