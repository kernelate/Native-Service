#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>

//#include "led_main.h"
#include "project_header.h"
#include "ldr_ir.h"

#ifdef LDR_IR_SERVICE_ON

int ir_level_1=IR_LEVEL_1;
int ir_level_2=IR_LEVEL_2;
int ir_level_3=IR_LEVEL_3;
int ir_pwm=0;

int ldr_ir_on=IR_ON_LEVEL;//3686;//3360;
int ldr_ir_off=IR_OFF_LEVEL;//3522;//2950;
int max_reflection=IR_MAX_REFLECTION;//450;

int fd = 0;
int irfd=0;

pthread_t ldr_ir_pid;



void set_ldr_values(int a,int b){
	ldr_ir_on=4200-(a*4.2);
	ldr_ir_off=4200-(b*4.2);
}

int ADC(int ADCPort)
{
    int a, ret;
    char adc_buf[1];
    int fd = open("/dev/adc",O_RDWR);
    if(fd < 0){
        printf("unable to open ADCdrver\n");
        return -1;
    }    
    if (ioctl(fd,LDR_IR_ADC_INPUT_PIN,ADCPort)){
        close(fd);
        return(-1);
    }

    read(fd,&a,sizeof(int));
    close(fd);
    return(a);
}

void set_ir_pwm(int ir_level){
			irfd = open(LDR_IR_DRV_NAME, O_RDWR);
			dtdrivers_data.value=ir_level;
			if (ioctl(irfd, CMD_IR_TIMER_SET, &IR_DRV_DATA_STRUCT) < 0) {
				printf("ioctl() Failed\n");
				}
			else{	
				printf("ioctl() success\n");
			}
			close(irfd);
}
void ir_led_off(){
//*
			irfd = open(LDR_IR_DRV_NAME, O_RDWR);
			dtdrivers_data.value=0;
			if (ioctl(irfd, CMD_IR_TIMER_SET, &IR_DRV_DATA_STRUCT) < 0) {
				printf("ioctl() Failed\n");
				}
			else{	
				printf("ioctl() success\n");
			}
			close(irfd);
//*/
}

void init_ir(){
	fd = open(LDR_IR_DEVICE_NAME, O_RDWR);
	IR_SFR_DATA_STRUCT.special_function_register = VA_GPIO_BASE + 0x00A0;
	IR_SFR_DATA_STRUCT.value = 0;
	if (ioctl(fd, SFR_READ, &IR_SFR_DATA_STRUCT) < 0) {
                printf("ioctl() Failed\n");
	}
	IR_SFR_DATA_STRUCT.value=IR_SFR_DATA_STRUCT.value&0x0FFF;
	IR_SFR_DATA_STRUCT.value=IR_SFR_DATA_STRUCT.value|0x1000;
	if (ioctl(fd, SFR_WRITE, &IR_SFR_DATA_STRUCT) < 0) {
		printf("ioctl() Failed\n");
	}

	IR_SFR_DATA_STRUCT.special_function_register = VA_GPIO_BASE + 0x0c40;
	IR_SFR_DATA_STRUCT.value = 0;
	if (ioctl(fd, SFR_READ, &IR_SFR_DATA_STRUCT) < 0) {
                printf("ioctl() Failed\n");
	}
	IR_SFR_DATA_STRUCT.value=IR_SFR_DATA_STRUCT.value&0xF0FFFFFF;
	IR_SFR_DATA_STRUCT.value=IR_SFR_DATA_STRUCT.value|0x01000000;
	if (ioctl(fd, SFR_WRITE, &IR_SFR_DATA_STRUCT) < 0) {
		printf("ioctl() Failed\n");
	}
  
	IR_SFR_DATA_STRUCT.special_function_register = VA_GPIO_BASE + 0x0c60;
	IR_SFR_DATA_STRUCT.value = 0;
	if (ioctl(fd, SFR_READ, &IR_SFR_DATA_STRUCT) < 0) {
                printf("ioctl() Failed\n");
	}
	IR_SFR_DATA_STRUCT.value=IR_SFR_DATA_STRUCT.value&0xFFFF0FFF;
	IR_SFR_DATA_STRUCT.value=IR_SFR_DATA_STRUCT.value|0x00001000;
	if (ioctl(fd, SFR_WRITE, &IR_SFR_DATA_STRUCT) < 0) {
		printf("ioctl() Failed\n");
	}
  	close(fd);
}


void ir_control(int control)
{
//*
  switch(control)
    {
        //case IRLED_OFF_OPS:
        case 1:
            printf("off ir\n");
//			ir_led_off();
			ir_pwm=0;
            break;
        //case LOW_OPS:
        case 2:
            printf("low ir\n");
			ir_pwm=ir_level_1;
//		    ir_led_on_2();
//			if (ioctl(fd, IRLED_LOW, &IR_SFR_DATA_STRUCT) < 0) {
//				printf("ioctl() Failed\n");
//        }
            break;
        //case MID_OPS:
        case 3:
            printf("mid ir\n");
			ir_pwm=ir_level_2;
//		    ir_led_on_2();
//			if (ioctl(fd, IRLED_MID, &IR_SFR_DATA_STRUCT) < 0) {
//				printf("ioctl() Failed\n");
//			}
            break;
        //case HIGH_OPS:
        case 4:
			printf("high ir\n");
			ir_pwm=ir_level_3;
//			ir_led_on_3();
//            IR_SFR_DATA_STRUCT.value = 0x57;
            break;
		case 5:
			printf("always on");
			ir_pwm=100;
			break;
        //case IRLED_OFF_OPS:
    }

	set_ir_pwm(ir_pwm);
//*/

}
void* thread_ldr_ir(void*args)
{
    int i;
    int adc_val=0;
    int adc_total=0;
    init_ir();
	set_ir_pwm(0);
    int adc_ir=0;
#if 1
    printf("This is a random test\n");
    while(1)
    {
        adc_val = ADC(LDR_IR_ADC_PORT);
        printf("Adc = %d\n", adc_val);

/* alpha code with no reflection reference
	if(adc_val>3600){
            ir_control(4);
	}else if(adc_val>3300){
            ir_control(3);
	}else if(adc_val>2900){
            ir_control(2);
	}else if(adc_val<2700){
            ir_control(1);
	}
//*/

//* with reference
        if(adc_val-adc_ir > ldr_ir_on){
            ir_control(4);//turn on
	    if (adc_ir==0){
		adc_total=ADC(LDR_IR_ADC_PORT);
		adc_ir=(adc_val-adc_total);
	    }
	}
	
        if((adc_val+adc_ir)<ldr_ir_off){
            ir_control(1);//turn off
		adc_ir=0;
        }
	if(adc_ir>0){
		printf("adc_ir=%d\n",adc_ir);
		if (adc_ir>max_reflection){
			adc_ir=max_reflection;
		}
	}
//*/
        usleep(5000000);

    }
#else
    adc_val = ADC(LDR_IR_ADC_PORT);
    printf("%d\n", adc_val);
#endif


}

void ldr_ir_service(){
	pthread_create(&ldr_ir_pid, NULL,&thread_ldr_ir, NULL);
}

#endif
