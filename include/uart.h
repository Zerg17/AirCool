#pragma once

#include <stdint.h>

void uartInit();
void uartWrite(uint8_t d);
void sendPack(uint8_t type, uint8_t* data, uint8_t len);