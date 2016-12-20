#include "common2.h"
//SELECT SETUP
//#define DOORTALK
#define SMART_ENTRY


//-----------------------------//

#ifdef PROJECT_NAME
#define HEADER "PROJECT_NAME\x00"
#define WATCHDOG_ON
#define ETH_SERVICE_ON
#define LDR_IR_SERVICE_ON
#define SOCKET_SERVICE_ON
#endif


#ifdef DOORTALK
//#define WATCHDOG_ON
#define ETH_SERVICE_ON
#define LDR_IR_SERVICE_ON
#define SOCKET_SERVICE_ON

//watchdog service
	#define WD_SERVICE_NAME "ntek_service_beta"
	#define APP_NAME			"com.ntek.servertalk"
	#define RUN_APP				"com.ntek.servertalk.utils/com.ntek.servertalk.utils.MainActivity" //app to be run by am command
	#define WATCHDOG_TIMER                  30		//in secs

//ethernet service
	DTDRIVERS_DATA dtdrivers_data;
	#define ETH_DRIVER_DATA_STRUCT dtdrivers_data
	#define CMD_ETHERNET_ON				ETHERNET_ON
	#define CMD_ETHERNET_OFF				ETHERNET_OFF
	#define ETH_DRV_NAME		"/dev/DoorTalk_Drivers"

//ldr ir service
	#define LDR_IR_DRV_NAME					"/dev/DoorTalk_Drivers"
	#define LDR_IR_ADC_INPUT_PIN      	_IOW('S',0x0c,unsigned long)
	#define LDR_IR_ADC_PORT            		6
	#define LDR_IR_DEVICE_NAME         	"/dev/s5pc110_sfr"

	DTDRIVERS_DATA dtdrivers_data;
	SFR_DATA sfr_data;
	#define IR_DRV_DATA_STRUCT	dtdrivers_data
	#define IR_SFR_DATA_STRUCT	sfr_data
	#define CMD_IR_TIMER_SET		IR_TIMER_SET
	#define IR_LEVEL_1						35
	#define IR_LEVEL_2						57
	#define IR_LEVEL_3						70
	#define IR_ON_LEVEL					3686
	#define IR_OFF_LEVEL				3522
	#define IR_MAX_REFLECTION	450

//socket_service
	#define HEADER "DOORTALK\x00"

#endif

#ifdef SMART_ENTRY

//#define WATCHDOG_ON
#define ETH_SERVICE_ON
//#define LDR_IR_SERVICE_ON
#define SOCKET_SERVICE_ON

//watchdog service
	#define SERVICE_NAME "ntek_service_beta"
	#define APP_NAME			"com.ntek.servertalk"
	#define RUN_APP				"com.ntek.servertalk.utils/com.ntek.servertalk.utils.MainActivity" //app to be run by am command
	#define WATCHDOG_TIMER                  30		//in secs

//ethernet service
	DTDRIVERS_DATA dtdrivers_data;
	#define ETH_DRIVER_DATA_STRUCT  dtdrivers_data
	#define CMD_ETHERNET_ON				ETH_CONNECTED
	#define CMD_ETHERNET_OFF				ETH_DISCONNECTED
	#define ETH_DRV_NAME		"/dev/ntek_eth_dev"

//ldr ir service
	#define LDR_IR_DRV_NAME					"/dev/smart_entry_driver"
	#define LDR_IR_ADC_INPUT_PIN      	_IOW('S',0x0c,unsigned long)
	#define LDR_IR_ADC_PORT            		6
	#define LDR_IR_DEVICE_NAME         	"/dev/rk3188_sfr"
	
	DTDRIVERS_DATA dtdrivers_data;
	SFR_DATA sfr_data;
	#define IR_DRV_DATA_STRUCT	dtdrivers_data
	#define IR_SFR_DATA_STRUCT	sfr_data
	#define CMD_IR_TIMER_SET		IR_TIMER_SET
	#define IR_LEVEL_1						35
	#define IR_LEVEL_2						57
	#define IR_LEVEL_3						70
	#define IR_ON_LEVEL					3686
	#define IR_OFF_LEVEL				3522
	#define IR_MAX_REFLECTION	450

//socket_service
	#define HEADER "DOORTALK\x00"

#endif
/*
#ifndef HEADER
	#define HEADER "NTEK\x00"
#endif
*/
