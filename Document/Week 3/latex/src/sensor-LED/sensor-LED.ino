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
  float lux = min(1, lightMeter.readLightLevel() / 75);
  float inv_lux =   (1 - lux);
  int brightness = inv_lux * 255;

  Serial.print(lux);
  Serial.print(" ");
  Serial.println(inv_lux);
  // iterate over the pins:
  for (int thisPin = highestPin; thisPin <= highestPin;
    thisPin++) {
    analogWrite(thisPin, brightness);
  }
    for (int thisPin = lowestPin; thisPin <= highestPin;
    thisPin++) {
    analogWrite(thisPin, brightness);
  }
  delay(200);
}
