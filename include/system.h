#pragma once
#ifndef SYSTEM
#define SYSTEM

#include "stm32f0xx.h"
#include "xprintf.h"

#define BOAD 19200

#define ALARM_ON  GPIOB->BSRR = GPIO_BSRR_BS_7
#define ALARM_OFF GPIOB->BSRR = GPIO_BSRR_BR_7

#define COOL_ON  GPIOB->BSRR = GPIO_BSRR_BS_1
#define COOL_OFF GPIOB->BSRR = GPIO_BSRR_BR_1

#define HEAT_ON  GPIOB->BSRR = GPIO_BSRR_BS_0
#define HEAT_OFF GPIOB->BSRR = GPIO_BSRR_BR_0

extern uint8_t buff[128];

extern volatile uint16_t rpm1, rpm2;
extern int32_t term1, term2;
extern uint16_t voltage, current;

extern volatile uint32_t tick;
extern volatile uint32_t sec_d;
extern volatile uint32_t sec;

extern uint16_t adcR[4];
extern uint16_t adcF[4];

void sysInit();
void uartWrite(uint8_t d);
void flashInit();
void flashSectorClear(uint32_t adr);
void flashWrite(uint32_t adr, uint16_t data);

#endif