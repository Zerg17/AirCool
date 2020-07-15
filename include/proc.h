#pragma once

#include <stdint.h>

extern uint16_t rpm1S, rpm2S;
extern volatile uint8_t msgResponse, msgResponseType;

void logicProc();
