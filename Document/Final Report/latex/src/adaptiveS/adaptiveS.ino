if (branch_conf[index].brightness_mode == 1){
          // adaptive brightness mode
          // In this mode the brightness is adaptively set
          // according to the value received from the sensor
          
          float sensitivity_level_ratio = 
            (- branch_conf[index].sensitivity + 100) * 1.0 / 200;
          int sensitivity_level = 
            sensitivity_level_min + sensitivity_level_ratio *
              (sensitivity_level_max - sensitivity_level_min);
          brightness_ratio =
            1 - min(1, lightMeter.readLightLevel() / sensitivity_level);      
}
