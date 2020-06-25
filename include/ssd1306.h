#pragma once
#ifndef __SSD1306_H
#define __SSD1306_H

#include <stdint.h>
#include "fonts.h"
#include "system.h"

#define SSD1306_ADDRESS 0x78

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define INDENT 4

extern uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

typedef enum {
  Black,
  White
} SSD1306_COLOR;

typedef enum {
  Commands = 0x00, 
  Datas = 0x40  
} SSD1306_DATA_TYPE;

typedef struct {
  uint32_t CurrentX;
  uint32_t CurrentY;
  uint8_t Address;
} SSD1306_t;

void ssd1306_Init(uint8_t address);
void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_UpdateScreen(void);
void ssd1306_DrawPixel(uint32_t x, uint32_t y, SSD1306_COLOR color);
void ssd1306_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, SSD1306_COLOR color);
void ssd1306_DrawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, SSD1306_COLOR color);
void ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
void ssd1306_Char(unsigned char ch);
void ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);
void ssd1306_SetCursor(uint32_t x, uint32_t y);
void ssd1306_SetContrast(uint8_t contrast);
void ssd1306_DisplayOn(void);
void ssd1306_DisplayOff(void);



#endif
