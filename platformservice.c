#define SERVICE_NAME "platform_service_beta"
#define VERSION "v160609" 
#define DATE_REVISED "06/09/16"
#define LOGCAT 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <linux/kernel.h>
#include <netinet/if_ether.h>
#include "socket_server_api.h"
#include "common.h"
#include "wpa.h"
#include "wifi_ap.h"
#include "connectivity.h"
#include "ldr_ir.h"
#include "watchdog_Api.h"
#include "project_header.h"
#include "android_ui_Api.h"
#include "file_operation.h"
#include "custom_app_api.h"
#include "platformservice.h"

#define DEBUG_LEVEL     1
#ifndef HEADER
#define HEADER "NTEK\x00"
#endif

#if LOGCAT
#include "cutils/log.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "RECOVERY_MONITOR", __VA_ARGS__))
#endif

#define DPRINTK(level,fmt,args...) do{ if(level < DEBUG_LEVEL)\
    printk(KERN_INFO "%s [%s ,%d]: " fmt "\n",DEV_NAME,__FUNCTION__,__LINE__,##args);\
}while(0)

/* MESSAGE PATTERN TX
 *  DOORTALK:ACK:WPA:FAIL:END                           //failed creating file
 *  DOORTALK:ACK:WPA:OK:END                             //successfullycreated file
 */
/*MESSAGE PATTERN RX
 *  DOORTALK:CMD1:PARAM1,2,3:END
 */

#define MAX_MSG_TX                  500
#define MAX_MSG_RX                  500
#define MAX_MSG_CMD                 100
#define EXPECTED_PARAMS             10
 
int f_runService = 1;
char msg_socket_buff[MAX_MSG_RX];
char msg_cmd[MAX_MSG_CMD];
char msg_param[EXPECTED_PARAMS][MAX_MSG_RX];
int a = 0;
int b = 0;
char target_path[MAX_MSG_RX];
int f_burnCmd = 0;
char header[25];

char* available_commands[] = {
    "CMD_CREATE_WPA",                       //0
    "CMD_WIFI_AP_ON",                       //1
    "CMD_WIFI_AP_OFF",                      //2
    "CMD_IF_DYNAMIC",                       //3
    "CMD_IF_STATIC",                        //4
    "CMD_WDT_CLR",                          //5
    "CMD_DNS",                              //6
    "CMD_RESET_HW",                         //7
    "CMD_DUMPSYS",                          //8
    "CMD_ETH_UP",                           //9
    "CMD_ETH_DOWN",                         //10
    "CMD_RECOVERY",                         //11
    "CMD_IR_CONTROL",                       //12
    "CMD_ADB_WIFI_ENABLE",                  //13
    "CMD_ADB_WIFI_DISABLE",                 //14
    "CMD_ANDROID_NOTIFICATION_BAR_ON",      //15
    "CMD_ANDROID_NOTIFICATION_BAR_OFF",     //16
    "CMD_CREATE_FILE",                      //17
    "CMD_APPEND_FILE",                      //18
    "CMD_MOVE_FILE",                        //19
    "CMD_COPY_FILE",                        //20
    "CMD_RELOAD_ASTERISK",                  //21
};

int main(int argc, char* argv[])
{

    int ret;
    printf("hello\n");
    display_version();

#ifdef WATCHDOG_ON
    watchdog_service();
#endif

#ifdef ETH_SERVICE_ON
    eth0_service();
#endif

#ifdef LDR_IR_SERVICE_ON
    ldr_ir_service();
#endif

#ifdef SOCKET_SERVICE_ON
    start_socket_service();
    while (f_runService)
    {
        //*Socket message handling part
        wait_socket_session();
        rcv_msg(msg_socket_buff);
        msg_handler(msg_socket_buff);
        close_socket_session();
    }
    stop_socket_service();
#endif

#ifndef SOCKET_SERVICE_ON
    while (1)
    {
    }
#endif

    return 0;
}

int msg_handler(char* msgbuff)
{
    time_t t = time(NULL);
    struct tm tm;
    char buff[MAX_MSG_TX];
    int i = 0;
    int pid;
    int portno = 0;
    memset(buff, 0, MAX_MSG_TX);
    memset(target_path, 0, MAX_MSG_TX);

    if (msg_parser(msgbuff) == -1){
        return -1;
    }
    printf("%s\n", msg_cmd);

    while (i < MAX_MSG_TX)
    {
        if (strcmp(available_commands[i], msg_cmd) == 0)
        {
            break;
        } else
        {
            i++;
        }
    }

    switch (i)
    {
        case 0:
            wpa_supplicant_reset(msg_param[1], msg_param[2], msg_param[3], msg_param[4]);
            reply_msg("WIFI_HISTORY", "CLEAR");
            break;
        case 1:
            wifi_ap_off();
            reply_msg("WIFI_AP_ON", "OK");
            break;
        case 2:
            wifi_ap_off();
            reply_msg("WIFI_AP_OFF", "OK");
            break;
        case 3:
            interface_dynamic(msg_param[1]);
            reply_msg("IF_DYNAMIC", "OK");
            break;
        case 4:
            interface_static(msg_param[1], msg_param[2], msg_param[3], msg_param[4]);
            reply_msg("IF_STATIC", "OK");
            break;
#ifdef WATCHDOG_ON
        case 5:
            reset_watchdog_timer();
            reply_msg("WDT", "CLEAR");
            break;
#endif
        case 6:
            set_dns(msg_param[1], msg_param[2]);
            reply_msg("DNS", "SET");
            break;
        case 7:
            reply_msg("HW", "REBOOT");
            sync();
            system("reboot");
            break;
        case 8:
            reply_msg("DUMP", "SYS");
            dumpsystem();
            break;
#ifdef ETH_SERVICE_ON
        case 9:
            reply_msg("ETH", "UP");
            set_eth_service(1);
            system("ifconfig eth0 up");
            break;
        case 10:
            reply_msg("ETH", "DOWN");
            set_eth_service(0);
            system("ifconfig eth0 down");
            break;
#endif
        case 11:
            reply_msg("HW", "RECOVERY");
            sync();
            system("reboot recovery");
            break;
#ifdef LDR_IR_SERVICE_ON
        case 12://IR
            reply_msg("IR", "CONTROL");
            a = atoi(msg_param[1]);
            b = atoi(msg_param[2]);
            set_ldr_values(a, b);
            break;
#endif
        case 13:
            portno = atoi(msg_param[1]);
            if (portno < 65536 && portno > 100)
            {
                reply_msg("ADB_WIFI", "ENABLE");
                adb_wifi_enable(portno);
            } else
            {
                reply_msg("ADB_WIFI_PORT=5555", "ENABLE");
                adb_wifi_enable(5555);
            }
            break;
        case 14: //ADB WIFI OFF
            reply_msg("ADB_WIFI", "DISABLE");
            adb_wifi_disable();
            break;
#ifdef  ANDROID_UI_ON
        case 15: //NOTIFICATION BAR ON
            reply_msg("NOTIF_BAR", "ENABLE");
            set_notification_bar(1);
            break;
        case 16: //NOTIFICATION BAR OFF
            reply_msg("NOTIF_BAR", "DISABLE");
            set_notification_bar(0);
            break;
#endif
        case 17: //CREATE_FILE
            create_file(msg_param[1], msg_param[2]);
            reply_msg("FILE", "CREATED");
            break;
        case 18: //APPEND_FILE
            append_file(msg_param[1], msg_param[2]);
            reply_msg("CONTENT", "APPENDED");
            break;
        case 19: //MOVE_FILE
            move_file(msg_param[1], msg_param[2]);
            reply_msg("FILE", "MOVED");
            break;
        case 20: //COPY_FILE
            copy_file(msg_param[1], msg_param[2]);
            reply_msg("FILE", "COPIED");
            break;
        case 21: //RELOAD_ASTERISK
            asterisk_reload_config_files();
            reply_msg("ASTERISK","RELOADED");
            break;
        default:
            reply_msg("INVALID", "MSG");
            return -1;
        }
        return 0;
}

void write_klog(const char *tag, char *message)
{
    int fd;
    char buffer[4096];

    fd = open("/dev/kmsg", O_RDWR);
    if (fd < 0)
    {
        printf("problem opening /dev/kmsg\n");
        return;
    }
    strcpy(buffer, tag);
    strcat(buffer, " : ");
    strncat(buffer, message, sizeof (buffer) - 1);
    write(fd, buffer, strlen(buffer) + 1);
    write(fd, "\n", 1);
    close(fd);
}

void display_version()
{
    char buffer[100];
    // when run on terminal
    sprintf(buffer, "%s %s - %s %s for  %s\n", SERVICE_NAME, VERSION, __DATE__, __TIME__,PROJECT);
    printf("%s", buffer);
    //dmesg
    write_klog(SERVICE_NAME, PROJECT);
    write_klog(SERVICE_NAME, __DATE__);
    write_klog(SERVICE_NAME, __TIME__);
    //android logcat
#if LOGCAT
    LOGI(buffer);
#endif
    strcpy(header, HEADER);
    //find_device();
}

void dumpsystem()
{
    int record = 1;
    char buffer[100];
    char fname[50];
    while (record <= 5)
    {
        sprintf(fname, "/data/dumpstat_%d.txt", record);
        if (access(fname, F_OK) != -1)
        {
            record++;
        } else
        {
            break;
        }
    }
    if (record > 5)
    {
        system("cat /data/dumpstat_2.txt > /data/dumpstat_1.txt");
        system("cat /data/dumpstat_3.txt > /data/dumpstat_2.txt");
        system("cat /data/dumpstat_4.txt > /data/dumpstat_3.txt");
        system("cat /data/dumpstat_5.txt > /data/dumpstat_4.txt");
        system("dumpsys > /data/dumpstat_5.txt");
    } else
    {
        sprintf(buffer, "dumpsys > /data/dumpstat_%d.txt", record);
        system(buffer);
    }
}

void start_socket_service()
{
    init_socket_server();
    set_socket_param();
}

void wait_socket_session()
{
    wait_socket_server_session();
}

void rcv_msg(char* msgbuff)
{
    memset(msg_socket_buff, 0, MAX_MSG_RX);
    get_socket_message(msgbuff);
}

int reply_msg(char* reply1, char* reply2)
{
    char reply_buff[MAX_MSG_TX];
    memset(reply_buff, 0, MAX_MSG_TX);
    sprintf(reply_buff, "PLATFORM:ACK:%s:%s:END", reply1, reply2);
    send_socket_message(reply_buff);
    return 0;
}

void close_socket_session()
{
    close_socket_server_session();
}

void stop_socket_service()
{
    end_socket_server();
}

void clear_params(void)
{
    memset(msg_cmd, 0, MAX_MSG_CMD);
    int i = 0;
    for (i = 0; i < EXPECTED_PARAMS; i++)
    {
        memset(msg_param[i], 0, MAX_MSG_RX);
    }
}


int msg_parser(char* msgbuff)
{
    int i;
    int ret = 0;
    char* buff;
    clear_params();
    buff = strtok(msgbuff, ":");
    if (strncmp(header, buff, strlen(header)) != 0)
    {
        goto error1;
    }
    //parse string message
    for (i = 0; i < EXPECTED_PARAMS; i++)
    {
        // TODO: modify this part for dynamic parameters
        buff = strtok(NULL, ":");
        if (strncmp("END", buff, 3) == 0)
        {
            i = EXPECTED_PARAMS;
            break;
        }
        if (i == 0)
        {
            strcpy(msg_cmd, buff);
        } else
        {
            if (i < EXPECTED_PARAMS)
            {
                strcpy(msg_param[i], buff);
            } else
            {
                goto error1;
            }
        }
    }

    //check if message is valid
    printf("comparing command for validity");
    for (i = 0; available_commands[i] != NULL; i++)
    {
        printf("%s comparing to %s \n", msg_cmd, available_commands[i]);
        printf(".");
        if (strncmp(msg_cmd, available_commands[i], strlen(msg_cmd)) == 0)
        {
            printf("\nVALID\n");
            return 0;
        }
    }
    printf("\n");
error1:
    //DPRINTF("Invalid msg\n");
    //printf("INVALID\n");
    return -1;
}