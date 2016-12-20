#include <stdio.h>
#include <string.h>
#include "project_header.h"
#include "android_ui_Api.h"

#ifdef ANDROID_UI_ON
void set_notification_bar(int enable){
	if(enable==1){
		system(NOTIF_BAR_ENABLE);
	}
	if(enable==0){
		system(NOTIF_BAR_DISABLE);
	}	
}

#endif
