#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;
const int lowestPin = 2;

const int highestPin = 7;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();
  
  // set pins 2 through 7 as outputs:
  for (int thisPin = lowestPin; thisPin <= highestPin;
    thisPin++) {
    pinMode(thisPin, OUTPUT);

  }
}

void loop() {
  float lux = min(1, lightMeter.readLightLevel() / 500);
  float inv_lux = pow((1 - lux), 4);
  int brightness = inv_lux * 255;

  // iterate over the pins:
  for (int thisPin = lowestPin; thisPin <= highestPin;
    thisPin++) {
    analogWrite(thisPin, brightness);
  }
  delay(200);
}
