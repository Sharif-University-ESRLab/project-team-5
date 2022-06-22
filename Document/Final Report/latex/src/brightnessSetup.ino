  // For each branch we will setup their brightnesses according
  // to their configuration
  
  for (int branch_pin = branch_base; branch_pin <
      branch_base + branch_num; branch_pin++) {
      int index = branch_pin - branch_base;

      // This means that the brightness for this branch
      // is set by another pin
      // for example in the inverted mode
      if (conf_activated[index] == 0) {
        continue;
      }
      
      int brightness = 0;
      float brightness_ratio = 0.0;
              ............
   }
