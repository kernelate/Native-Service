
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include "watchdog_Api.h"
#include "project_header.h"


#ifdef WATCHDOG_ON
int wtimer=0;
pthread_t wtmr_pid;

void kill_command(char * app_to_kill){
	int app_pid=0;
	char app_name[50]="none";
	char directory[50]=" ";
	int n_dir=5;
	FILE *input_file, *fopen();
	while(strcmp(app_name,app_to_kill)!=0 &&app_pid<65535){
		app_pid++;
		sprintf(directory,"/proc/%d/cmdline",app_pid);
		input_file = fopen(directory,"r");//tries to open file
		if(input_file==NULL){
			//input file not found => no usb2eth connected
			//printf("file not found\n");
		}
		else{
			fgets (app_name , 50 , input_file); //gets "up" until usb2eth or ethernet is disconected
			fclose(input_file);
			printf("%d\t%s\n",app_pid,app_name);
		}		
	}
	if(app_pid<65535){
		kill(app_pid,SIGTERM);
		printf("~~~~KILL THE APP~~~~\n");
	}
	else{
		printf("-------APP NOT FOUND-------\n");
	}
}

void* thread_watchdog(void*args){
	printf("software watchdog running\n");
	write_klog(WD_SERVICE_NAME,"SOFTWARE_WATCHDOG_RUNNING");

	while(1){
		while(wtimer<WATCHDOG_TIMER){
			printf("WATCHDOG: %d\n",wtimer);
			wtimer++;
			sleep(1);
		}
		kill_command(APP_NAME);			
//*
		char buffer[100];
		sprintf(buffer,"am start -a android.intent.action.MAIN -n %s",RUN_APP);
		system(buffer);
		wtimer=0;
//*/
	}
	return NULL;
}

void reset_watchdog_timer(){
	wtimer=0;
	printf("SOCKET SERVICE: watchdog cleared\n");
}

int watchdog_service(){
	pthread_create(&wtmr_pid, NULL,&thread_watchdog, NULL);	//watchdog 
	return 1;
}
#endif
