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
