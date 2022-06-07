// this structure defines all the variables 
// and events of the control interface 
struct {

    // input variables
  int8_t sensitivity; 
  // =-100..100 slider position 
  uint8_t power; 
  // =1 if switch ON and =0 if OFF 
  uint8_t branch; 
  // =0 if select position A, =1 if position B, =2 
  // if position C, ... 
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

    // other variable
  uint8_t connect_flag;  
  // =1 if wire connected, else =0 

} RemoteXY;
