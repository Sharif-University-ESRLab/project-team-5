//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <Wire.h>
#include <BH1750.h>
#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "smart_chandelier"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377

BH1750 lightMeter;
const int lowestPin = 2;

const int highestPin = 3;

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 283 bytes
  { 255,10,0,0,0,20,1,16,31,1,130,0,5,75,53,23,167,4,160,20,
  59,43,5,2,26,2,0,35,22,22,11,120,26,31,31,79,78,0,79,70,
  70,0,3,130,4,22,22,12,232,26,4,176,20,66,43,5,2,26,1,3,
  3,42,23,11,218,31,73,110,118,101,114,116,101,100,32,77,111,100,101,0,
  7,53,8,89,19,7,2,26,36,7,53,36,89,19,7,2,26,36,2,1,
  30,44,30,8,2,26,31,31,65,100,97,112,116,105,118,101,0,83,116,97,
  116,105,99,0,129,0,7,5,51,6,1,83,109,97,114,116,32,32,67,104,
  97,110,100,101,108,105,101,114,0,129,0,10,18,11,3,242,66,114,97,110,
  99,104,0,129,0,42,18,9,3,242,80,111,119,101,114,0,129,0,33,39,
  25,3,242,66,114,105,103,104,116,110,101,115,115,32,77,111,100,101,0,129,
  0,3,60,15,3,242,83,101,110,115,105,116,105,118,105,116,121,0,129,0,
  3,67,16,3,242,66,114,105,103,104,116,110,101,115,115,0,129,0,13,77,
  35,4,242,66,114,105,103,104,116,110,101,115,115,32,77,97,114,103,105,110,
  0,129,0,14,84,7,4,242,77,73,78,0,129,0,41,84,9,4,242,77,
  65,88,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t sensitivity; // =-100..100 slider position 
  uint8_t power; // =1 if switch ON and =0 if OFF 
  uint8_t branch; // =0 if select position A, =1 if position B, =2 if position C, ... 
  int8_t brightness; // =-100..100 slider position 
  uint8_t inverted_mode; // =1 if button pressed, else =0 
  int16_t min_brightness;  // 32767.. +32767 
  int16_t max_brightness;  // 32767.. +32767 
  uint8_t mode; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////



void setup() 
{
  RemoteXY_Init (); 
  
  
  // TODO you setup code
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();
  
  for (int thisPin = lowestPin; thisPin <= highestPin;
    thisPin++) {
    pinMode(thisPin, OUTPUT);

  }
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  // do not call delay()

  int sensitivity = (RemoteXY.sensitivity + 100) * 140 / 200 + 10;

  float lux = min(1, lightMeter.readLightLevel() / sensitivity);
  float inv_lux =   (1 - lux);
  int brightness;

  if (power == 0) {
    brightness = 0;
  }
  else {
    if (mode == 1) {// adaptive
      brightness = inv_lux * 255;
    }
    else { // static
      brightness = (RemoteXY.brightness + 100) * 255 / 200;
    }
  }

  if (brightness > RemoteXY.max_brightness) {
    brightness = RemoteXY.max_brightness;
  }

  if (brightness < RemoteXY.min_brightness) {
    brightness = RemoteXY.min_brightness;
  }
  
  Serial.print(lux);
  Serial.print(" ");
  Serial.println(inv_lux);
  // iterate over the pins:
  if (RemoteXY.branch == 1) {
    analogWrite(lowestPin, brightness);
  }
  else if (RemoteXY.branch == 0) {
    analogWrite(highestPin, brightness);
  }
  // delay(200); ?? 


}
