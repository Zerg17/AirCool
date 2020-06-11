#ifndef SYSTEM
#define SYSTEM

#include "stm32f0xx.h"
#include "xprintf.h"

#define BOAD 500000

void sysInit();
void rccInit();
void gpioInit();
void uartInit();
void i2cInit();

#endif