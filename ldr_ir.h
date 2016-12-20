/*
#include "sample_ioctl.h"
#include "sfr_main.h"
#include "ntek_service_ioctl.h"
//*/

void set_ldr_values(int a,int b);
int ADC(int ADCPort);
void ir_control(int control);
void* thread_ldr_ir(void*args);
void ldr_ir_service();
