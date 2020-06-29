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
void drawDebug(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6);

#endif