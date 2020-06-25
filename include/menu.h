#pragma once
#ifndef MSG
#define MSG

#include "stdint.h"

#define TESTING_MSG "Тестирование"
#define WAIT_MSG "Ожидание"
#define COOLING_MSG "Охлаждение"
#define HEAT_MSG "Нагрев"

void drawWait(uint16_t t);
void drawErr(uint8_t err);
void drawMain(int16_t t, uint16_t rmp, uint16_t v, uint16_t a);

#endif