#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <errno.h>
#include <netinet/in.h>
#if defined(__GLIBC__) && __GLIBC__ >=2 && __GLIBC_MINOR__ >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>
#else
#include <sys/types.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <pthread.h>
#include <fcntl.h>
#include <math.h>
#include <sys/ioctl.h>
#include <dirent.h>
//#include <dirent.h>

#include "connectivity.h"
#include "socket_server_api.h"
#include "project_header.h"

#define ETH_ON 1
#define ETH_OFF 0

pthread_t eth_pid;
char net_add[100];
int ethernet_on = 0;
int fdeth = 0;

#ifdef DOORTALK

#endif

#ifdef SMART_ENTRY

#endif

void set_dns(char* dns1, char* dns2)
{

    char buffer[50];
    sprintf(buffer, "setprop net.dns1 %s ", dns1);
    system(buffer);
    sprintf(buffer, "setprop net.dns2 %s ", dns2);
    system(buffer);
}

int set_ip(char *iface_name, char *ip_addr, char *net_mask)
{
    if (!iface_name)
        return -1;

    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in sin;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        fprintf(stderr, "Could not get socket.\n");
        return -1;
    }

    /* get interface name */
    strncpy(ifr.ifr_name, iface_name, IFNAMSIZ);

    /* Read interface flags */
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0)
    {
        fprintf(stderr, "ifdown: shutdown ");
        perror(ifr.ifr_name);
        return -1;
    }

    /*
     * Expected in <net/if.h> according to
     * "UNIX Network Programming".
     */
#ifdef ifr_flags
#define IRFFLAGS       ifr_flags
#else   /* Present on kFreeBSD */
#define IRFFLAGS       ifr_flagshigh
#endif

    // If interface is down, bring it up
    if (!(ifr.IRFFLAGS & IFF_UP))
    {
        fprintf(stdout, "Device is currently down..setting up.-- %u\n", ifr.IRFFLAGS);
        ifr.IRFFLAGS |= IFF_UP;
        if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0)
        {
            fprintf(stderr, "ifup: failed ");
            perror(ifr.ifr_name);
            return -1;
        }
    }

    sin.sin_family = AF_INET;

    // Convert IP from numbers and dots to binary notation
    inet_aton(ip_addr, &sin.sin_addr.s_addr);
    memcpy(&ifr.ifr_addr, &sin, sizeof (struct sockaddr));

    // Set interface address
    if (ioctl(sockfd, SIOCSIFADDR, &ifr) < 0)
    {
        fprintf(stderr, "Cannot set IP address. ");
        perror(ifr.ifr_name);
        return -1;
    }

    inet_aton(net_mask, &sin.sin_addr.s_addr);
    memcpy(&ifr.ifr_addr, &sin, sizeof (struct sockaddr));

    // Set interface address
    if (ioctl(sockfd, SIOCSIFNETMASK, &ifr) < 0)
    {
        fprintf(stderr, "Cannot set netmask. ");
        perror(ifr.ifr_name);
        return -1;
    }
#undef IRFFLAGS		
    return 0;
}

void get_net_add(char* ip_addr, char* net_mask)
{
    char* netmask;
    char* ipaddr;
    netmask = net_mask;
    ipaddr = ip_addr;
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 1;
    char buffer[2];
    int net1 = 0;
    int net2 = 0;
    int net3 = 0;
    int net4 = 0;
    int mask1 = 0;
    int mask2 = 0;
    int mask3 = 0;
    int mask4 = 0;
    while (i < 4)
    {
        if (netmask[j] == '.' || netmask[j] == '\x0')
        {
            i++;
            switch (i)
            {
                case 1:
                    while ((j - k + 1) > l)
                    {

                        sprintf(buffer, "%c", netmask[j - l]);
                        mask1 += atoi(buffer) * pow(10, (l - 1));
                        l++;
                    }

                    break;
                case 2:
                    while (l < (j - k + 1))
                    {
                        sprintf(buffer, "%c", netmask[j - l]);
                        mask2 += atoi(buffer) * pow(10, (l - 1));
                        ;
                        l++;
                    }
                    break;
                case 3:
                    while (l < (j - k + 1))
                    {
                        sprintf(buffer, "%c", netmask[j - l]);
                        mask3 += atoi(buffer) * pow(10, (l - 1));
                        ;
                        l++;
                    }
                    break;
                case 4:
                    while (l < (j - k + 1))
                    {
                        sprintf(buffer, "%c", netmask[j - l]);
                        mask4 += atoi(buffer) * pow(10, (l - 1));
                        l++;
                    }
                    break;
            }
            k = j;
        }
        j++;
        l = 1;
    }
    printf("%d.%d.%d.%d\n", mask1, mask2, mask3, mask4);
    i = 0;
    j = 0;
    k = 0;
    l = 1;
    while (i < 4)
    {
        if (ipaddr[j] == '.' || ipaddr[j] == '\x0')
        {
            i++;
            switch (i)
            {
                case 1:
                    while ((j - k + 1) > l)
                    {
                        sprintf(buffer, "%c", ipaddr[j - l]);
                        net1 += atoi(buffer) * pow(10, (l - 1));
                        l++;
                    }
                    break;
                case 2:
                    while (l < (j - k + 1))
                    {
                        sprintf(buffer, "%c", ipaddr[j - l]);
                        net2 += atoi(buffer) * pow(10, (l - 1));
                        ;
                        l++;
                    }

                    break;
                case 3:
                    while (l < (j - k + 1))
                    {
                        sprintf(buffer, "%c", ipaddr[j - l]);
                        net3 += atoi(buffer) * pow(10, (l - 1));
                        ;
                        l++;
                    }

                    break;
                case 4:
                    while (l < (j - k + 1))
                    {
                        sprintf(buffer, "%c", ipaddr[j - l]);
                        net4 += atoi(buffer) * pow(10, (l - 1));
                        l++;
                    }

                    break;
            }
            k = j;
        }
        j++;
        l = 1;
    }
    printf("network address: %s\n", ipaddr);
    printf("here\n");
    printf("netmask: %d.%d.%d.%d\n", net1, net2, net3, net4);
    printf("ip address: %d.%d.%d.%d\n", net1, net2, net3, net4);
    net1 = net1&mask1;
    net2 = net2&mask2;
    net3 = net3&mask3;
    net4 = net4&mask4;
    sprintf(net_add, "%d.%d.%d.%d", net1, net2, net3, net4);
    printf("network address: %s\n", net_add);
}

void interface_static(char* interface, char* ip_addr, char* net_mask, char* gateway)
{
    //set_ip(interface,ip_addr,net_mask);
    char buffer[100];

    get_net_add(ip_addr, net_mask);
    printf("%s\n", net_add);

    sprintf(buffer, "busybox route del -net %s netmask %s", net_add, net_mask); //del old route with same net and netmask
    system(buffer);
    sprintf(buffer, "busybox ifconfig %s %s netmask %s up", interface, ip_addr, net_mask); // set up
    system(buffer);
    sprintf(buffer, "busybox route add -net %s netmask %s gw %s", net_add, net_mask, gateway); //adds new route with new gateway
    system(buffer);
}

void interface_dynamic(char* interface)
{
    /*
    int pid;
    pid=fork();
    if(pid==0){
            close_socket_server_session();
            //printf("\n*********\n");
            execl ("/system/bin/dhcpcd","/system/bin/dhcpcd","-d",interface, NULL);  // should run /system/bin/dhcpcd -d eth0
            //printf("\n*********\n");
    }
    //*/
    //*
    char buffer[50];
    sprintf(buffer, "/system/bin/dhcpcd -d  %s ", interface);
    system(buffer);
    system("setprop net.dns1 8.8.8.8");
    //*/
}
#ifdef 			ETH_SERVICE_ON

void set_eth_service(int set)
{
    ethernet_on = set;
    sleep(2);
}

void eth_keypress(int eth_stat)
{
    char broadcast[500];
    if (eth_stat == 1)
    {
#ifdef ETH_BROADCAST_ON
        sprintf(broadcast, ETH_BROADCAST, "true");
        printf("%s\n",broadcast);
        system(broadcast);
#endif
#ifndef ETH_BROADCAST_ON

        fdeth = open(ETH_DRV_NAME, O_RDWR);
        if (ioctl(fdeth, CMD_ETHERNET_ON, &ETH_DRIVER_DATA_STRUCT) < 0)
        {
            printf("ioctl() Failed\n");
        } else
        {
            printf("ioctl() success\n");
        }
        close(fdeth);
#endif

    } else
    {
        if (eth_stat == 0)
        {
#ifdef ETH_BROADCAST_ON
            sprintf(broadcast, ETH_BROADCAST, "false");
            printf("%s\n",broadcast);
            system(broadcast);
#endif
#ifndef ETH_BROADCAST_ON
            fdeth = open(ETH_DRV_NAME, O_RDWR);
            if (ioctl(fdeth, CMD_ETHERNET_OFF, &ETH_DRIVER_DATA_STRUCT) < 0)
            {
                printf("ioctl() Failed\n");
            } else
            {
                printf("ioctl() success\n");
            }
            close(fdeth);
#endif
        }
    }

}

void* thread_eth_checker(void*arg)
{
    struct DIR * eth_handler;
    FILE *input_file, *fopen();
    char operstate[5] = "down";
    int pid;
    int sockfd;
    int ip_flag=0;
    struct ifreq ifr;
    struct sockaddr_in sin;
    printf("platform_eth0_service:START\n");
    system("/system/bin/dhcpcd -d eth0");
    while (1)
    {

        if (ethernet_on == 1)
        {
            printf("platform_eth0_service:ethservice on\n");
            eth_handler = opendir("/sys/class/net/eth0");
            while (eth_handler == 0 && ethernet_on == 1)
            {
                //this will run if not open
                printf("platform_eth0_service:eth0 not found\n");
                sleep(2);
                eth_handler = opendir("/sys/class/net/eth0");
            }

            if (ethernet_on == 1)
            {
                closedir(eth_handler);
                printf("platform_eth0_service: eth adaptor found\n");
                set_ip("eth0", "0.0.0.0", "255.255.255.255"); //force up
            }

            while (strcmp(operstate, "down") == 0 && ethernet_on == 1)
            {
                input_file = fopen("/sys/class/net/eth0/operstate", "r"); //tries to open file

                if (input_file == NULL)
                {
                    //input file not found => no usb2eth connected
                    printf("nteak_eth0_service:file not found\n");
                    strcpy(operstate, "up");
                } else
                {
                    fgets(operstate, 5, input_file); //gets "up" until usb2eth or ethernet is disconected
                    fclose(input_file);
                }

                printf("from down to operstate: %s\n", operstate);
                sleep(2);
            }
            //*
            ip_flag=check_ip();
            while (ethernet_on == 1 && ip_flag==0)
            {
                printf("platform_eth0_service: ethcable detected\n");
                printf("platform_eth0_service: eth0 no ip\n");
                printf("============a=============\n");
                printf("O_O PLATFORM DEBUGGING\n");
                printf("=========================\n");
                system("/system/bin/dhcpcd -d eth0");
                sleep(5);
                ip_flag=check_ip();
            }
            if(ethernet_on==1 && ip_flag==1){
            	eth_keypress(1);
            }
            //*/
            /*
            if (ethernet_on == 1)
            {
                printf("platform_eth0_service: ethcable detected\n");
                system("/system/bin/dhcpcd -d eth0");
                sleep(5);
                eth_keypress(1);
            }
            //*/

            while (strncmp(operstate, "up", 2) == 0 && ethernet_on == 1)
            {
                //continuous reading till down
                input_file = fopen("/sys/class/net/eth0/operstate", "r"); //tries to open file

                if (input_file == NULL)
                {
                    //input file not found => no usb2eth connected
                    printf("file not found\n");
                    strcpy(operstate, "down");
                } else
                {
                    fgets(operstate, 5, input_file); //gets "up" until usb2eth or ethernet is disconected
                    fclose(input_file);
                }

                printf("from up to operstate: %s\n", operstate);
                sleep(2);
            }

            if (ethernet_on == 1)
            {
                //remove ip_address
                printf("platform_eth0_service: ethcable removed/not detected\n");
                system("/system/bin/dhcpcd -d eth0");
                //system("setprop net.dns1 8.8.8.8");
                sleep(5);
                eth_keypress(0);
                strcpy(operstate, "down");
            }

        } else
        {
            //		system("busybox ifconfig eth0 down");
            printf("platform_eth0_service:ethservice off\n");
            sleep(2);
        }
    }
    printf("thread closed\n");
}

void eth0_service(void)
{
    int ret;
    printf("platform_eth0_service\n");
    ret = pthread_create(&eth_pid, NULL, &thread_eth_checker, NULL);
    printf("pthread_create return value:\t%d\n", ret);
}

#endif

void adb_wifi_enable(int portno)
{
    char buffer[100];
    sprintf(buffer, "setprop service.adb.tcp.port %d ", portno);
    system(buffer);
    sprintf(buffer, "stop adbd");
    system(buffer);
    sprintf(buffer, "start adbd");
    system(buffer);
}

void adb_wifi_disable(void)
{
    char buffer[100];
    sprintf(buffer, "setprop service.adb.tcp.port -1 ");
    system(buffer);
    sprintf(buffer, "stop adbd");
    system(buffer);
    sprintf(buffer, "start adbd");
    system(buffer);
}

int check_ip()
{
    int ret_val = 0;
    char ip_cmd[500];
    FILE *ip_file, *fopen();
    char temp[10];
    memset(ip_cmd,'0',500);
    sprintf(ip_cmd, "busybox ifconfig eth0 | grep \"inet addr\" | busybox wc -l > /data/temp_determine_ip_alive");
    system(ip_cmd);
    ip_file = fopen(ETH_PATH_FILE, "r"); //tries to open file
    if (ip_file == NULL)
    {
        printf("file not found\n");
    } else
    {
        temp[0]=fgetc(ip_file);
        printf("============b=============\n");
        printf("ip file = %c\n",temp[0]);
        printf("=========================\n");    
        fclose(ip_file);
    }
    if (strncmp(temp, "1", 1) == 0)
    {
        ret_val = 1; //ip exist
    } else
    {
        ret_val = 0; //ip doest not exist
    }
    //sprintf(ip_cmd, "rm /data/temp_determine_ip_alive");
    system(ip_cmd);
    return ret_val;
}
