#pragma once
#ifndef MSG
#define MSG

#include "stdint.h"

#define TESTING_MSG "������������"
#define WAIT_MSG "��������"
#define COOLING_MSG "����������"
#define HEAT_MSG "������"

void drawWait(uint16_t t);
void drawErr(uint8_t err);
void drawMain(int16_t t, uint16_t rmp, uint16_t v, uint16_t a);

#endif