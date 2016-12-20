#define PROJECT "JUANTIME"

#define JUANTIME

#ifdef JUANTIME
//service list
//#define WATCHDOG_ON
#define ETH_SERVICE_ON
//#define LDR_IR_SERVICE_ON
#define SOCKET_SERVICE_ON
#endif 

#ifdef JUANTIME
typedef struct {
	int special_function_register;
	int value;
} JTDRIVERS_DATA;
	JTDRIVERS_DATA jtdrivers_data;
typedef struct {
	int special_function_register;
	int value;
} SFR_DATA;
	#define SFR_WRITE	        	_IOW('p', 1, SFR_DATA)
	#define SFR_READ	        	_IOR('p', 2, SFR_DATA)
	#define DUMP_SPI			_IOW('p', 3, SFR_DATA)
	#define RANDOM_TEST			_IOWR('p',100, SFR_DATA)
	#define VA_GPIO_BASE			0xfd500000
#endif


#ifdef JUANTIME

#ifdef WATCHDOG_ON
//watchdog service
	#define WD_SERVICE_NAME 		"platformservice_beta"
	#define APP_NAME			"com.ntek.servertalk"
	#define RUN_APP				"com.ntek.servertalk.utils/com.ntek.servertalk.utils.MainActivity" //app to be run by am command
	#define WATCHDOG_TIMER                  30		//in secs
#endif


#ifdef ETH_SERVICE_ON
//ethernet service
	#define ETHERNET_ON			_IOW('j', 1, JTDRIVERS_DATA)
	#define ETHERNET_OFF			_IOW('j', 2, JTDRIVERS_DATA)
	#define ETHERNET_ON_OPS			1
	#define ETHERNET_OFF_OPS		2
	#define ETH_DRIVER_DATA_STRUCT 		jtdrivers_data
	#define CMD_ETHERNET_ON			ETHERNET_ON
	#define CMD_ETHERNET_OFF		ETHERNET_OFF
	#define ETH_DRV_NAME			"/dev/JuanTime_Driver"
#endif

#ifdef LDR_IR_SERVICE_ON
//ldr ir service

	#define IR_TIMER_SET			_IOW('p', 20, DTDRIVERS_DATA)
	#define  IR_TIMER_SET_OPS		17
	#define LDR_IR_DRV_NAME			"/dev/DoorTalk_Drivers"
	#define LDR_IR_ADC_INPUT_PIN      	_IOW('S',0x0c,unsigned long)
	#define LDR_IR_ADC_PORT            	6
	#define LDR_IR_DEVICE_NAME         	"/dev/s5pc110_sfr"

	DTDRIVERS_DATA dtdrivers_data;
	SFR_DATA sfr_data;
	#define IR_DRV_DATA_STRUCT		dtdrivers_data
	#define IR_SFR_DATA_STRUCT		sfr_data
	#define CMD_IR_TIMER_SET		IR_TIMER_SET
	#define IR_LEVEL_1			35
	#define IR_LEVEL_2			57
	#define IR_LEVEL_3			70
	#define IR_ON_LEVEL			3686
	#define IR_OFF_LEVEL			3522
	#define IR_MAX_REFLECTION		450
#endif

//socket_service
#ifdef SOCKET_SERVICE_ON
	#define HEADER 				"PLATFORM\x00"
#endif
#endif
