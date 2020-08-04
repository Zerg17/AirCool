#pragma once

#include <stdint.h>

extern volatile uint32_t tick;
extern volatile uint32_t sec_d;
extern volatile uint32_t sec;


extern volatile uint16_t rpm1, rpm2;
extern uint16_t rpm1S, rpm2S;

extern int32_t term1, term2;
extern uint16_t voltage, current;


extern uint16_t adcR[4];
extern uint16_t adcF[4];