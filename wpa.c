
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "wpa.h"
#include "socket_server_api.h"

int wpa_supplicant_reset(char* ipaddress,char* password,char* key_mgmt,char* is_hidden){
	/*
	int pid;
	pid=fork();
	if(pid==0){
		close_socket_server_session();
	        execl ("/system/bin/wpa_supplicant.sh", "/system/bin/wpa_supplicant.sh", ipaddress,password,key_mgmt,is_hidden, NULL);
	        //execl ("/system/bin/dt_services.sh", "/system/bin/dt_services.sh","create_wpa", ipaddress,password,key_mgmt, NULL);
	}
	//*/
	//*
	char buffer[50];
	sprintf(buffer,"/system/bin/wpa_supplicant.sh %s %s %s %s", ipaddress,password,key_mgmt,is_hidden);
	system(buffer);
	//*/
	return 0;
}
