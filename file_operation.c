#include "project_header.h"
#include "file_operation.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void create_file(char * content, char * path_two){
	char cmd[255];
	memset(cmd,0,255);
	sprintf(cmd,"echo \"%s\" > %s",content,path_two);
	system(cmd);
	sprintf(cmd,"chmod 777 %s",path_two);
	system(cmd);
}

void append_file(char * content, char * path_two){
	char cmd[255];
	memset(cmd,0,255);
	sprintf(cmd,"echo \"%s\" >> %s",content,path_two);
	system(cmd);
	sprintf(cmd,"chmod 777 %s",path_two);
	system(cmd);
}

void move_file(char * path_one, char * path_two){
	char cmd[255];
	memset(cmd,0,255);
	sprintf(cmd,"mv %s %s",path_one,path_two);
	system(cmd);	
}

void copy_file(char * path_one, char * path_two){
	char cmd[255];
	memset(cmd,0,255);

	sprintf(cmd,"cp %s %s",path_one,path_two);
	system(cmd);	
}