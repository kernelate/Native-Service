
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "wifi_ap.h"
#include "socket_server_api.h"

int wifi_ap_on(char* ipaddress,char* password,char* key_mgmt){
	/*	
	int pid;
	pid=fork();
	//printf("after fork\n");
	if(pid==0){
		//printf("before exec...\n");
		close_socket_server_session();
        	execl ("/system/bin/wifi_ap_enable.sh","/system/bin/wifi_ap_enable.sh", NULL);   
	        //execl ("/system/bin/dt_services.sh", "/system/bin/dt_services.sh","ap_enable", NULL);
		//printf("before exec\n");
	}
	
	//*/
	/*
	system("/system/bin/wifi_ap_enable.sh");
	//*/
	//*
	system("busybox ifconfig wlap0 192.168.50.1");
	system("busybox ifconfig wlap0 up");
	system("/system/bin/dnsmasq --no-daemon --no-poll -no-resolv --dhcp-range=192.168.50.5,192.168.50.99,1h");
	//*/

	return 0;
}

int wifi_ap_off(char* ipaddress,char* password,char* key_mgmt){
	/*
	int pid;
	pid=fork();
	//printf("after fork\n");
	if(pid==0){
		//printf("before exec...\n");
		close_socket_server_session();
       		execl ("/system/bin/wifi_ap_disable.sh","/system/bin/wifi_ap_disable.sh", NULL);
	        //execl ("/system/bin/dt_services.sh", "/system/bin/dt_services.sh","ap_disable", NULL);
		//printf("before exec\n");
	}
	//*/
	/*
	system("/system/bin/wifi_ap_disable.sh");
	//*/
	//*
	system("stop wpa_supplicant");
	system("insmod /system/wifi/wlan.ko");
	system("busybox killall -9 hostapd");
	system("busybox killall -9 dnsmasq");
	//*/
	return 0;
}
