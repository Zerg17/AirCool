#pragma once
#ifndef SYSTEM
#define SYSTEM

#include "stm32f0xx.h"
#include "xprintf.h"

#define BOAD 500000

extern uint8_t buff[128];

extern volatile uint16_t rpm1;
extern volatile uint16_t rpm2;

extern volatile uint32_t tick;
extern volatile uint32_t sec;

extern uint16_t adcR[4];
extern uint16_t adcF[4];

void sysInit();
void uartWrite(uint8_t d);
void flashInit();
void flashSectorClear(uint32_t adr);
void flashWrite(uint32_t adr, uint16_t data);

#endif