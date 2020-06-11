#ifndef SYSTEM
#define SYSTEM

#include "stm32f0xx.h"
#include "xprintf.h"

#define BOAD 1200

void sysInit();
void rccInit();
void gpioInit();
void uartInit();

#endif