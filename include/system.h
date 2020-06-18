#pragma once
#ifndef SYSTEM
#define SYSTEM

#include "stm32f0xx.h"
#include "xprintf.h"

#define BOAD 500000

extern uint32_t tick;
extern uint32_t sec;

extern uint16_t adcR[2];
extern uint16_t adcF[2];

void sysInit();
void rccInit();
void gpioInit();
void uartInit();
void i2cInit();
void adcInit();

#endif