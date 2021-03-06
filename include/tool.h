#pragma once
#ifndef TOOL
#define TOOL

#include <stdint.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define map(val, inMin, inMax, outMin, outMax) (((val)-(inMin))*((outMax)-(outMin))/((inMax)-(inMin))+(outMin))

int32_t tempCalc(int16_t a);
void printTemp(int32_t t);
uint16_t strl(char *str);

#endif