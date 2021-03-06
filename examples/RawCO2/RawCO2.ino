/* 
  Raw CO2:
  Using the raw value competantly requires insight into the technology
  and science behind how such algorithms are produced. However, it
  can still be extremely useful to have a rough value as a 'sanity check'
  against MHZ19 calibration failure. This is because the raw is 
  not affect by span/range/zero/temperature.

  This can be achieved by plotting the Raw value vs CO2 ppm over a
  broad range when conditions are idealistic. This can be versus
  its own calibrated PPM.

  Using this relationship, a trend can be produced (an exponetial 
  trend is ideal) which can be applied to the raw value. 

  While this is useful, it's important to remember that temperature
  and pressure both have an effect on PPM and that changes to these
  parameters will not reflect in your trend (although the difference
  is often very small). 
 */

#include "MHZ19.h"
#include <Arduino.h>
#include <SoftwareSerial.h>                                // Remove if using HardwareSerial

#define RX_PIN 10                                          // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN 11                                          // Tx pin which the MHZ19 Rx pin is attached to
#define BAUDRATE 9600                                      // Native to the sensor (do not change)

MHZ19 myMHZ19;                                             // Constructor for MH-Z19 class
SoftwareSerial mySerial(RX_PIN, TX_PIN);                   // Constructor for Stream class *change for HardwareSerial, i.e. ESP32 ***

//HardwareSerial mySerial(1);                              // ESP32 Example 

unsigned long getDataTimer = 0;

void setup()
{
  Serial.begin(9600);

  mySerial.begin(BAUDRATE);                               // Begin Stream with MHZ19 baudrate

  //mySerial.begin(BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN); // ESP32 Example 
  
  myMHZ19.begin(mySerial);                                // *Important, Pass your Stream reference
}

void loop()
{
  if (millis() - getDataTimer >= 2000)
  {

    double adjustedCO2 = myMHZ19.getCO2Raw();

    Serial.println("----------------");
    Serial.print("Raw CO2: ");
    Serial.println(adjustedCO2, 0);

    adjustedCO2 = 6.60435861e+15 * exp(-8.78661228e-04 * adjustedCO2);      // Exponential equation for Raw & CO2 relationship

    Serial.print("Adjusted CO2: ");
    Serial.print(adjustedCO2, 2);
    Serial.println(" ppm");

    getDataTimer = millis();

  }
}
