#pragma once

#include <stdint.h>

extern uint8_t msgFlug;
extern uint8_t msgType;
extern uint8_t msgLen;
extern uint8_t dataMsg[255];

void uartInit();
void uartWrite(uint8_t d);
void sendPack(uint8_t type, uint8_t* data, uint8_t len);