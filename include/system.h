#pragma once
#ifndef SYSTEM
#define SYSTEM

#include "stm32f0xx.h"
#include "xprintf.h"

#define BOAD 19200

extern volatile uint32_t tick;
extern volatile uint32_t sec;

extern uint16_t adcR[4];
extern uint16_t adcF[4];

void sysInit();
void rccInit();
void gpioInit();
void uartWrite(uint8_t d);
void uartInit();
void i2cInit();
void adcInit();
void flashInit();
void flashSectorClear(uint32_t adr);
void flashWrite(uint32_t adr, uint16_t data);

#endif