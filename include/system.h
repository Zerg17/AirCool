#ifndef SYSTEM
#define SYSTEM

#include "stm32f0xx.h"
#include "xprintf.h"

#define BOAD 500000

extern int16_t adcB[256];

void sysInit();
void rccInit();
void gpioInit();
void uartInit();
void i2cInit();
void adcInit();

#endif