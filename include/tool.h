#pragma once
#ifndef TOOL
#define TOOL

#include <stdint.h>

int32_t tempCalc(int16_t a);
void sendPack(uint8_t type, uint8_t* data, uint8_t len);
void printTemp(int32_t t);

#endif