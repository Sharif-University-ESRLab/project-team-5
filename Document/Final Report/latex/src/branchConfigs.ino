
// This struct contains configurations
// for each of the branches
struct Config{
  int8_t sensitivity; 
  // =-100..100 slider position 
  uint8_t power;
   // =1 if switch ON and =0 if OFF 
  int8_t brightness; 
  // =-100..100 slider position 
  uint8_t inverted_mode; 
  // =1 if button pressed, else =0 
  int16_t min_brightness; 
  // 32767.. +32767 
  int16_t max_brightness; 
  // 32767.. +32767 
  uint8_t brightness_mode;
  // =1 if switch ON and =0 if OFF 
};
Config branch_conf[branch_num];

