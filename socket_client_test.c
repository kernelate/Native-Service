/*  NOTE:
 *      This is sample socket client only, not 
 *      inteneded to be used together with socket app
 *      native service.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_BUFF        255

#define TARGET_ADDR     "127.0.0.1"

#define TARGET_PORT     5001



#define CMD_WPA					"PLATFORM:CMD_CREATE_WPA:platform:aaaa1112:WPA-PSK:0:END" //WPA 
#define CMD_WPA2				"PLATFORM:CMD_CREATE_WPA:platform:aaaa1112:WPA-PSK:1:END" //WPA 
#define CMD_WIFI_AP_ON			"PLATFORM:CMD_WIFI_AP_ON:END"
#define CMD_WIFI_AP_OFF			"PLATFORM:CMD_WIFI_AP_OFF:END"
#define CMD_IF_DYNAMIC			"PLATFORM:CMD_IF_DYNAMIC:eth0:END"
#define CMD_IF_STATIC			"PLATFORM:CMD_IF_STATIC:eth0:192.168.2.140:255.255.255.0:192.168.2.1:END"
#define CMD_WDOG_RESET			"PLATFORM:CMD_WDT_CLR:END"
#define CMD_WIFI_STATIC			"PLATFORM:CMD_IF_STATIC:wlan0:192.168.2.180:255.255.255.0:192.168.2.1:END"
#define CMD_DNS_SET				"PLATFORM:CMD_DNS:8.8.8.8:8.8.8.8:END"
#define CMD_RESET_HW			"PLATFORM:CMD_RESET_HW:END"
#define CMD_DUMPSYS				"PLATFORM:CMD_DUMPSYS:END"
#define CMD_ETH_UP				"PLATFORM:CMD_ETH_UP:END"
#define CMD_ETH_DOWN			"PLATFORM:CMD_ETH_DOWN:END"
#define CMD_FAIL				"NOT_PLATFORM:FAIL:END"
#define CMD_RECOVERY			"PLATFORM:CMD_RECOVERY:END"
#define CMD_IR_CONTROL			"PLATFORM:CMD_IR_CONTROL:25:40:END"
#define CMD_NOTIF_ON			"PLATFORM:CMD_ANDROID_NOTIFICATION_BAR_ON:END"
#define CMD_NOTIF_OFF			"PLATFORM:CMD_ANDROID_NOTIFICATION_BAR_OFF:END"
#define CMD_ABDWIFI_ON			"PLATFORM:CMD_ADB_WIFI_ENABLE:5555:END"
#define CMD_ABDWIFI_OFF			"PLATFORM:CMD_ADB_WIFI_DISABLE:END"
#define CMD_CREATE_FILE			"PLATFORM:CMD_CREATE_FILE:HELLO:/data/12345:END"
#define CMD_APPEND_TO_FILE		"PLATFORM:CMD_APPEND_FILE:APPENDED:/data/12345:END"
#define CMD_COPY_FILE			"PLATFORM:CMD_COPY_FILE:/data/12345:/data/data/:END"
#define CMD_MOVE_FILE			"PLATFORM:CMD_MOVE_FILE:/data/12345:/data/testing:END"
#define CMD_ETH_UP_NTEK			"DOORTALK:CMD_ETH_UP:END" 

void print_help()
{
	printf("1 - %s\n", CMD_WPA);
	printf("2 - %s\n", CMD_WPA2);
	printf("3 - %s\n", CMD_WIFI_AP_ON);
	printf("4 - %s\n", CMD_WIFI_AP_OFF);
	printf("5 - %s\n", CMD_IF_DYNAMIC);
	printf("6 - %s\n", CMD_IF_STATIC);
	printf("7 - %s\n",CMD_WDOG_RESET);
	printf("8 - %s\n", CMD_WIFI_STATIC);
	printf("9 - %s\n",CMD_DNS_SET);
	printf("10 - %s\n",CMD_RESET_HW);
	printf("11 - %s\n",CMD_DUMPSYS);
	printf("12 - %s\n",CMD_ETH_UP);
	printf("13 - %s\n",CMD_ETH_DOWN);
	printf("14 - %s\n",CMD_RECOVERY);
	printf("15 - %s\n",CMD_IR_CONTROL);
	printf("16 - %s\n",CMD_NOTIF_ON);
	printf("17 - %s\n",CMD_NOTIF_OFF);
	printf("18 - %s\n",CMD_ABDWIFI_ON);
	printf("19 - %s\n",CMD_ABDWIFI_OFF);
	printf("20 - %s\n",CMD_CREATE_FILE);
	printf("21 - %s\n",CMD_APPEND_TO_FILE);
	printf("22 - %s\n",CMD_COPY_FILE);
	printf("23 - %s\n",CMD_MOVE_FILE);
	printf("99- %s\t\t\\\\testing of an INVALID COMMAND\n",CMD_FAIL);
	printf("255 - %s\n",CMD_ETH_UP_NTEK);
}

int main(int argc, char *argv[])
{
	int choice = 0;
	int socketfd;
	struct sockaddr_in serv_addr; 
	char rcvBuff[MAX_BUFF];

	if(argc != 2)
	{
		print_help();
		exit(0);
	}
	choice = atoi(argv[1]);

	//prepare socket 
	memset(rcvBuff, 0, MAX_BUFF);
	if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) <0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(TARGET_PORT);       //port no
    
	if(inet_pton(AF_INET, TARGET_ADDR, &serv_addr.sin_addr)<=0)
	{
		printf("\n inet_pton error occured\n");
        	return 1;
	} 
    
	// start socket connection
	if(connect(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return 1;
	} 
	switch(choice)
	{
		case 1:
			write(socketfd, CMD_WPA, strlen(CMD_WPA));
			break;
		case 2:
			write(socketfd, CMD_WPA2, strlen(CMD_WPA2));
			break;
        case 3:
			write(socketfd, CMD_WIFI_AP_ON, strlen(CMD_WIFI_AP_ON));
			break;
        case 4:
			write(socketfd, CMD_WIFI_AP_OFF, strlen(CMD_WIFI_AP_OFF));
			break;
		case 5:
			write(socketfd, CMD_IF_DYNAMIC, strlen(CMD_IF_DYNAMIC));
			break;
		case 6:
			write(socketfd, CMD_IF_STATIC, strlen(CMD_IF_STATIC));
			break;
		case 7:
			write(socketfd, CMD_WDOG_RESET, strlen(CMD_WDOG_RESET));
			break;
		case 8:
			write(socketfd, CMD_WIFI_STATIC, strlen(CMD_WIFI_STATIC));
			break;
		case 9:
			write(socketfd, CMD_DNS_SET, strlen(CMD_DNS_SET));
			break;
		case 10:
			write(socketfd, CMD_RESET_HW, strlen(CMD_RESET_HW));
			break;
		case 11:
			write(socketfd, CMD_DUMPSYS, strlen(CMD_DUMPSYS));
			break;
		case 12:
			write(socketfd, CMD_ETH_UP, strlen(CMD_ETH_UP));
			break;
		case 13:
			write(socketfd, CMD_ETH_DOWN, strlen(CMD_ETH_DOWN));
			break;
		case 14:
			write(socketfd, CMD_RECOVERY, strlen(CMD_RECOVERY));
			break;
		case 15:
			write(socketfd, CMD_IR_CONTROL, strlen(CMD_IR_CONTROL));
			break;
		case 16:
			write(socketfd, CMD_NOTIF_ON, strlen(CMD_NOTIF_ON));
			break;
		case 17:
			write(socketfd, CMD_NOTIF_OFF, strlen(CMD_NOTIF_OFF));
			break;
		case 18:
			write(socketfd, CMD_ABDWIFI_ON, strlen(CMD_ABDWIFI_ON));
			break;
		case 19:
			write(socketfd, CMD_ABDWIFI_OFF, strlen(CMD_ABDWIFI_OFF));
			break;
		case 20:
			write(socketfd, CMD_CREATE_FILE, strlen(CMD_CREATE_FILE));
			break;
		case 21:
			write(socketfd, CMD_APPEND_TO_FILE, strlen(CMD_APPEND_TO_FILE));
			break;
		case 22:
			write(socketfd, CMD_COPY_FILE, strlen(CMD_COPY_FILE));
			break;
		case 23:
			write(socketfd, CMD_MOVE_FILE, strlen(CMD_MOVE_FILE));
			break;
		case 99:
			write(socketfd,CMD_FAIL,strlen(CMD_FAIL));
			break;
		case 255:
			write(socketfd,CMD_ETH_UP_NTEK,strlen(CMD_ETH_UP_NTEK));
			break;
		default:
			printf("Invalid option\n");
			print_help();
			return -1;
	}
	
	while ((read(socketfd, rcvBuff, sizeof(rcvBuff)-1)) > 0)
	{
		if(fputs(rcvBuff, stdout) == EOF)
		{
			printf("\n Error : Fputs error\n");
		}
	}
	return 0;
	printf("\n");
	close(socketfd);
}
