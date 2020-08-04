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

void sysInit();
void uartWrite(uint8_t d);
void flashInit();
void flashSectorClear(uint32_t adr);
void flashWrite(uint32_t adr, uint16_t data);

#endif