if (branch_conf[index].inverted_mode == 1) {
          // If the inverted mode is set to on then a form
          // of light dance is performed between the branch and the next one
          // the dance is performed as follows:
          //      with the base_brightness obtained from before,
          //      the branch and the next one change their brightness
          //      values such that the sum of their brightnesses are
          //      always equal to base_brightness. e.g.
          //      (base_brightness / 2, base_brightness / 2)
          //      (base_brightness / 2 + 1, base_brightness / 2 - 1)
          //      (base_brightness / 2 + 2, base_brightness / 2 - 2)
          //      ...
  
          int range = min(base_brightness / 2, 255 - base_brightness / 2);
          int nxt_branch_pin = get_next_branch(branch_pin);
          
          // A dynamic parameter that changes each time the loop runs
          // we truncate the parameter in [-range, +range].
          int i = inverted_mode_counter % (range + range + 1) - range;
          
          // The values are set such that their sum is equal to
          // `base_brightness`
          analogWrite(branch_pin, base_brightness / 2 + i);
          analogWrite(nxt_branch_pin, base_brightness / 2 - i);
          inverted_mode_counter++;
 }
