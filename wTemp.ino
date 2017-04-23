// Demo the quad alphanumeric display LED backpack kit
// scrolls through every character, then scrolls Serial
// input onto the display

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <math.h>
#include <SPI.h>
#include <WiFi101.h>

#define VBATPIN A7

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

char ssid[] = "aplus";     //  your network SSID (name)
char pass[] = "ignoreplease!";  // your network password
int status = WL_IDLE_STATUS;     // the WiFi radio's status

void setup() {
  WiFi.setPins(8,7,4,2);
  Serial.begin(9600);
  alpha4.begin(0x70);
  alpha4.clear();
  alpha4.writeDisplay();
  
  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  
}


char displaybuffer[4] = {' ', ' ', ' ', ' '};

void loop() {

  
  double sensorValue1 = thermister(A1);
  displayTemp('a', sensorValue1);
  Serial.println();
  delay(5000);
  double sensorValue2 = thermister(A2);
  displayTemp('b', sensorValue2);
  Serial.println();
  delay(5000);
}

void displayTemp(char label, double sensorValue) {
  String val = String(sensorValue);
  alpha4.writeDigitAscii(0, label);
  alpha4.writeDigitAscii(1, val[0]);
  alpha4.writeDigitAscii(2, val[1]);
  alpha4.writeDigitAscii(3, val[2]);
  alpha4.writeDisplay();
}

float readResistance(int port) {
  int adc = analogRead(port);
  Serial.print("adc: "); Serial.println(adc);
  float volts = (float)adc / (1023.0 / 3.3);
  Serial.print("volts: "); Serial.println(volts);
  float resistance = 9600.0 / ((1023/(float)adc) - 1);
  Serial.print("resistance: "); Serial.println(resistance);
  return resistance;
}

// SH equation co-efficients for TX-1001X
// 0.0007343140544,0.0002157437229,0.0000000951568577
// https://tvwbb.com/showthread.php?69233-Thermoworks-TX-1001X-OP-amp-TX-1003X-AP-Probe-Steinhart-Hart-Coefficients
double thermister(int port) {
  float resistance = readResistance(port);
  // See http://en.wikipedia.org/wiki/Thermistor for explanation of formula
  double tempK = 1 / 
    (
      (0.0007343140544) + 
      (0.0002157437229 * log(resistance)) + 
      (0.0000000951568577 * pow(log(resistance), 3))
    );
  Serial.print("tempK: "); Serial.println(tempK);
  
  // Convert Kelvin to Celcius
  double tempC = tempK - 273.15;
  Serial.print("tempC: "); Serial.println(tempC);

  // Convert Celcius to Fahrenheit
  double tempF = (tempC * 1.8) + 32;
  Serial.print("tempF: "); Serial.println(tempF);         
  return tempF;
}

//int readVcc() {
//  float measuredvbat = analogRead(VBATPIN);
//  measuredvbat *= 2;    // we divided by 2, so multiply back
//  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
//  measuredvbat /= 1024; // convert to voltage
//  
//  Serial.print("VBat: " ); Serial.println(measuredvbat);
//  if (measuredvbat < 3.2) {
//    return 0;
//  }
//  if (measuredvbat >= 3.2 && measuredvbat < 3.4) {
//    return 1;
//  }
//  if (measuredvbat >= 3.4 && measuredvbat < 3.7) {
//    return 2;
//  }
//  return 3;
//}

