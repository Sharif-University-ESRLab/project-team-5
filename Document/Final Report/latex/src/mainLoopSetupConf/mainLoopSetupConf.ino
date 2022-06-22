RemoteXY_Handler ();
int setup_branch_index = RemoteXY.branch;

// Setup the current branch according 
// to the current input of the UI
branch_conf[setup_branch_index].sensitivity = \
		 RemoteXY.sensitivity;
branch_conf[setup_branch_index].power = \
		 RemoteXY.power;
branch_conf[setup_branch_index].brightness = \
		 RemoteXY.brightness;
branch_conf[setup_branch_index].inverted_mode = \
		 RemoteXY.inverted_mode;
branch_conf[setup_branch_index].min_brightness = \
 		 RemoteXY.min_brightness;
branch_conf[setup_branch_index].max_brightness = \
		 RemoteXY.max_brightness;
branch_conf[setup_branch_index].brightness_mode = \
		 RemoteXY.brightness_mode;

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
