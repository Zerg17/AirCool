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


uint16_t crc;

void crcf(uint8_t d){
    crc+=d*211;
    crc^=crc>>8;
}


void sendPack(uint8_t type, uint8_t* data, uint8_t len){
    crc=0x0F;
    uartWrite(0x55);
    uartWrite(0x00);
    uartWrite(len);crcf(len);
    uartWrite(type);crcf(type);
    for(uint8_t i=0; i<len; i++){
        uartWrite(data[i]);
        crcf(data[i]);
    }
    uartWrite(crc);
}