#include "tool.h"
#include "system.h"
#include "ssd1306.h"

int32_t logfix (uint32_t x){
    int32_t b = 1U << (16 - 1);
    int32_t y = 0;

    if (x == 0) {
        return INT32_MIN; // represents negative infinity
    }

    while (x < 1U << 16) {
        x <<= 1;
        y -= 1U << 16;
    }

    while (x >= 2U << 16) {
        x >>= 1;
        y += 1U << 16;
    }

    uint64_t z = x;

    for (uint8_t i = 0; i < 16; i++) {
        z = z * z >> 16;
        if (z >= 2U << (uint64_t)16) {
            z >>= 1;
            y += b;
        }
        b >>= 1;
    }

    return (y * (uint64_t)0x58b90bfc)>>31;
}

int32_t tempCalc(int16_t a){
    return (0xF6E00000000)/(logfix(((int64_t)a<<32)/((0x1000-a)<<16))+0xD3F94)-0x1112666;
}

uint16_t strl(char *str){
	uint16_t l=0;
    while (*str++ != 0)l++;
	return l;
}
