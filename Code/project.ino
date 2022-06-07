/*
   -- smart chandelier --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.8 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.11.1 or later version;
     - for iOS 1.9.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "smart_chandelier"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 283 bytes
  { 255,10,0,0,0,20,1,16,31,1,130,0,5,75,53,23,167,4,160,20,
  59,43,5,2,26,2,0,35,22,22,11,120,26,31,31,79,78,0,79,70,
  70,0,3,130,4,22,22,12,232,26,4,160,20,66,43,5,2,26,1,3,
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
  
// this structure defines all the variables and events of the control interface 
struct {

    // input variables
  int8_t sensitivity; // =-100..100 slider position 
  uint8_t power; // =1 if switch ON and =0 if OFF 
  uint8_t branch; // =0 if select position A, =1 if position B, =2 if position C, ... 
  int8_t brightness; // =-100..100 slider position 
  uint8_t inverted_mode; // =1 if button pressed, else =0 
  int16_t min_brightness;  // 32767.. +32767 
  int16_t max_brightness;  // 32767.. +32767 
  uint8_t brightness_mode; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;

#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter; // BH17050 light sensor
const int branch_base = 6; // first branch PWM pin
const int branch_num = 2; // number of branchs
const int sensitivity_level_min = 10;
const int sensitivity_level_max = 150;
int inverted_mode_counter = 0;
int nxt_branch_brightness;

// This struct contains configurations for each of the branches
struct Config{
  int8_t sensitivity; // =-100..100 slider position 
  uint8_t power; // =1 if switch ON and =0 if OFF 
  int8_t brightness; // =-100..100 slider position 
  uint8_t inverted_mode; // =1 if button pressed, else =0 
  int16_t min_brightness;  // 32767.. +32767 
  int16_t max_brightness;  // 32767.. +32767 
  uint8_t brightness_mode; // =1 if switch ON and =0 if OFF 
};
Config branch_conf[branch_num];

// This boolean array indicates whether we want to use the configurations
// for the branch or not. For example, when branch_conf[i].inverted_mode is set to '1'
// this means that the next branch will be set by the previous branch; thereby, we need
// not set the brightness of this branch and the previous setting will handle it. 
bool conf_activated[branch_num];

void setup() 
{
    RemoteXY_Init ();
    Wire.begin();
    lightMeter.begin();
    // setup PWM control pins of all branches 
    for (int branch_pin = branch_base; branch_pin < branch_base + branch_num; branch_pin++) {
      pinMode(branch_pin , OUTPUT);
    }
    // Activate all the configurations
    // and setup default configurations per each branch
    for (int i = 0; i < branch_num; i++) {
      conf_activated[i] = 1;
      branch_conf[i].power = 0;
      branch_conf[i].sensitivity = 0;
      branch_conf[i].brightness = 0;
      branch_conf[i].inverted_mode = 0;
      branch_conf[i].min_brightness = 0;
      branch_conf[i].max_brightness = 255;
    }
}

// Given branch `branch_pin` it gives us the next branch pin
int get_next_branch(int branch_pin) {
  int offset = branch_pin - branch_base;
  int nxt = (offset + 1) % branch_num;
  return branch_base + nxt;
}

void loop() 
{ 
  RemoteXY_Handler ();
  int setup_branch_index = RemoteXY.branch;

  // Setup the current branch according to the current input of the UI
  branch_conf[setup_branch_index].sensitivity = RemoteXY.sensitivity;
  branch_conf[setup_branch_index].power = RemoteXY.power;
  branch_conf[setup_branch_index].brightness = RemoteXY.brightness;
  branch_conf[setup_branch_index].inverted_mode = RemoteXY.inverted_mode;
  branch_conf[setup_branch_index].min_brightness = RemoteXY.min_brightness;
  branch_conf[setup_branch_index].max_brightness = RemoteXY.max_brightness;
  branch_conf[setup_branch_index].brightness_mode = RemoteXY.brightness_mode;

  // Setup whether the config is activated or not for the pins
  // (This comes to use when we set a pin to inverted mode)
  for (int i = 0; i < branch_num; i++) {
    int prv = (i - 1 + branch_num) % branch_num;
    if (branch_conf[prv].inverted_mode == 1) {
      conf_activated[i] = 0;
    } else {
      conf_activated[i] = 1;
    }
  }

  // For each branch we will setup their brightnesses according to their configuration
  
  for (int branch_pin = branch_base; branch_pin < branch_base + branch_num; branch_pin++) {
      int index = branch_pin - branch_base;

      // This means that the brightness for this branch is set by another pin
      // for example in the inverted mode
      if (conf_activated[index] == 0) {
        continue;
      }
      
      int brightness = 0;
      float brightness_ratio = 0.0;
  
      if (branch_conf[index].power == 1){
      // branch power switch is ON
    
        if (branch_conf[index].brightness_mode == 1){
          // adaptive brightness mode
          // In this mode the brightness is adaptively set according to the value received from the sensor
          
          float sensitivity_level_ratio = (- branch_conf[index].sensitivity + 100) * 1.0 / 200;
          int sensitivity_level = sensitivity_level_min + sensitivity_level_ratio * (sensitivity_level_max - sensitivity_level_min);
          brightness_ratio = 1 - min(1, lightMeter.readLightLevel() / sensitivity_level);      
        } else {
          // static brightness mode 
          // In this mode the brightness ratio is set only according to the input provided in the brightness
          // meter of the UI. This value is static and independent of the changes in the environment's lighting.
          brightness_ratio = (branch_conf[index].brightness + 100) * 1.0 / 200;
        }

        // Set up minimum and maximum brightness and a base brightness according to the ratio obtained from the previous parts
        int min_brightness = (branch_conf[index].min_brightness > 255)? 255 : branch_conf[index].min_brightness;
        int max_brightness = (branch_conf[index].max_brightness > 255)? 255 : branch_conf[index].max_brightness;
        int base_brightness = min_brightness + brightness_ratio * (max_brightness - min_brightness);
      
        if (branch_conf[index].inverted_mode == 1) {
          // If the inverted mode is set to on then a form of light dance is performed between the branch and the next one
          // the dance is performed as follows:
          //      with the base_brightness obtained from before,
          //      the branch and the next one change their brightness
          //      values such that the sum of their brightnesses are always
          //      equal to base_brightness. e.g.
          //      (base_brightness / 2, base_brightness / 2)
          //      (base_brightness / 2 + 1, base_brightness / 2 - 1)
          //      (base_brightness / 2 + 2, base_brightness / 2 - 2)
          //      ...
  
          int range = min(base_brightness / 2, 255 - base_brightness / 2);
          int nxt_branch_pin = get_next_branch(branch_pin);
          
          // A dynamic parameter that changes each time the loop runs
          // we truncate the parameter in [-range, +range].
          int i = inverted_mode_counter % (range + range + 1) - range;
          
          // The values are set such that their sum is equal to `base_brightness`
          analogWrite(branch_pin, base_brightness / 2 + i);
          analogWrite(nxt_branch_pin, base_brightness / 2 - i);
          inverted_mode_counter++;
          
        } else {
          // If the inverted mode is set to off then the brighness is merely static
          analogWrite(branch_pin , base_brightness);
        }
      } else {
        // If the power is off then the branch does not emit any light
        analogWrite(branch_pin, 0);
      }
  }
  
}
