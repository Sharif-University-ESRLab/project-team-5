else {
          // static brightness mode 
          // In this mode the brightness ratio is set only according to the input provided in the brightness
          // meter of the UI. This value is static and independent of the changes in the environment's lighting.
          brightness_ratio = (branch_conf[index].brightness + 100) * 1.0 / 200;
}
