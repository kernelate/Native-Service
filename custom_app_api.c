#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "project_header.h"
#include "custom_app_api.h"

int asterisk_reload_config_files(){
	system("/usr/sbin/asterisk -x reload");
	return 1;
}