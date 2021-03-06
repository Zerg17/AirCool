#include "ssd1306.h"
#include "stm32f0xx.h"

uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x07, 0x00, 0x00, 0x00, 
		0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 
		0x0F, 0x03, 0x00, 0x00, 0xC0, 0xF0, 0xFC, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0x7F, 0x3F, 0x1F, 0x0F, 0x03, 0x01, 0x80, 0xC0, 0xF0, 0xF8, 
		0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x81, 0xC0, 0xE0, 
		0xF0, 0xF8, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0x7F, 0x3F, 0x1F, 0x1F, 0x0F, 0x07, 0x03, 0x81, 0xC0, 0xE0, 
		0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0x9F, 0x0F, 0x07, 0x0F, 0x1F, 
		0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x1F, 0x0F, 0x0F, 0x07, 0x03, 0x83, 0xC1, 0xC0, 0xE0, 0xF0, 
		0xF8, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xC0, 0x80, 
		0x01, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 
		0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 
		0xF8, 0xF0, 0xE0, 0xC0, 0x81, 0x03, 0x07, 0x0F, 0x0F, 0x1F, 
		0x3F, 0x3F, 0x7F, 0x7F, 0x7F, 0xFF, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xF8, 0xF8, 
		0xF8, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00
};

static void ssd1306_SendToDisplay(SSD1306_DATA_TYPE type, uint8_t *data, uint16_t length);

static SSD1306_t SSD1306;

void ssd1306_Init(){
	CS_SET;
	RESET_SET;
  // Waiting for SEG/COM ON after reset
  //for(volatile uint32_t i=0; i<100000; i++);
  // Initialize LCD
  uint8_t init_data[] = {0xAE, 0xA6, 0x20, 0x00, 0x21, 0x00, 0x7F, 0x22, 0x00, 0x07, 0xA0, 0xC0, 0xA8, 0x3F, 0x81, 0x20, 0x8D, 0x14, 0xAF};
  // 0xAE - display off
  // 0xA6 - normal (not inversed)
  // 0x20, 0x00 - page horizontal adressing mode
  // 0x21, 0x00, 0x7F - column address from 0 to 127
  // 0x22, 0x00, 0x07 - page address from 0 to 7
  // 0xA1 - segment re-map (vertical mirroring)
  // 0xC8 - COM scan direction (horizontal mirroring)
  // 0xA8, 0x3F - multiplex ratio
  // 0x81, 0x7F - contrast ratio 127 
  // 0x8D, 0x14 - enable charge pump
  // 0xAF - display on (only just after enabling charge pump)
  ssd1306_SendToDisplay(Commands, init_data, sizeof(init_data));
  ssd1306_UpdateScreen();
	SSD1306.CurrentX = 0;
  SSD1306.CurrentY = 0;
}

void ssd1306_Fill(SSD1306_COLOR color) {
  uint8_t toBuffer = color == White?0xFF:0x00;
  for(uint32_t i = 0; i < sizeof(SSD1306_Buffer); i++)
    SSD1306_Buffer[i] = toBuffer;
	
}

void ssd1306_UpdateScreen(void) {
  uint8_t update_region_data[6] = {0x21, 0x00, 0x7F, 0x22, 0x00, 0x07};
  //0x21, 0x00, 0x7F - column address from 0 to 127
  //0x22, 0x00, 0x07 - page address from 0 to 7
  ssd1306_SendToDisplay(Commands, update_region_data, 6);
  for(uint32_t page = 0; page < 8; page++) 
    ssd1306_SendToDisplay(Datas, &SSD1306_Buffer[SSD1306_WIDTH * page], SSD1306_WIDTH);
}

void ssd1306_DrawPixel(uint32_t x, uint32_t y, SSD1306_COLOR color){
  if((x < SSD1306_WIDTH) && (y < SSD1306_HEIGHT)){
    if (color == White) SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    else SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

#define abs(x) ((x)>0?(x):-(x))
void ssd1306_DrawLine(int16_t  x0, int16_t  y0, int16_t  x1, int16_t  y1, SSD1306_COLOR color) {
	uint32_t tmp;
  uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
		tmp=x0;
		x0=y0;
		y0=tmp;
		tmp=x1;
		x1=y1;
		y1=tmp;
  }
  if (x0 > x1) {
		tmp=x0;
		x0=x1;
		x1=tmp;
		tmp=y0;
		y0=y1;
		y1=tmp;
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      ssd1306_DrawPixel(y0, x0, color);
    } else {
      ssd1306_DrawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void ssd1306_DrawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, SSD1306_COLOR color) {
  ssd1306_DrawLine(x0, y0, x1, y0, color);
	ssd1306_DrawLine(x0, y1, x1, y1, color);
	ssd1306_DrawLine(x0, y0, x0, y1, color);
	ssd1306_DrawLine(x1, y0, x1, y1, color);
}

void ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color){
  uint32_t pixel, x;

	if(ch=='\r'){
	  SSD1306.CurrentX=INDENT;
	  return;
  	}
  	if(ch=='\n'){
	  	SSD1306.CurrentX=INDENT;
	  	SSD1306.CurrentY+=Font.FontHeight;
	  	return;
  	}

  	if((SSD1306.CurrentX + Font.FontWidth) > SSD1306_WIDTH){
	  	SSD1306.CurrentX=INDENT;
	  	SSD1306.CurrentY+=Font.FontHeight;
  	}

  	if (((SSD1306.CurrentY + Font.FontHeight) < SSD1306_HEIGHT)){
    	for (uint32_t y = 0; y < Font.FontHeight; y++){
      		if(ch < 127) pixel = Font.fontEn[(ch - 32) * Font.FontHeight + y];
      		else pixel = Font.fontRu[(ch - 192) * Font.FontHeight + y]; 
      		x = Font.FontWidth;
      		while(x--){
        		if (pixel & 0x0001) ssd1306_DrawPixel(SSD1306.CurrentX + x, (SSD1306.CurrentY + y), color);
        		else ssd1306_DrawPixel(SSD1306.CurrentX + x, (SSD1306.CurrentY + y), !color);
        		pixel >>= 1;
      		}
    	}
  	}
  	SSD1306.CurrentX += Font.FontWidth;
}

void ssd1306_Char(unsigned char ch){
	if(ch!=0)
		ssd1306_WriteChar(ch, Font_7x9, White);
}

void ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color){
  while (*str) 
    ssd1306_WriteChar(*str++, Font, color);
}

void ssd1306_SetCursor(uint32_t x, uint32_t y) {
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

void ssd1306_SetContrast(uint8_t contrast){
  uint8_t contrast_data[2] = {0x81};
  contrast_data[1] = contrast;
  ssd1306_SendToDisplay(Commands, contrast_data, 2);
}

void ssd1306_DisplayOn(){
  uint8_t contrast_data[1] = {0xAF};
  ssd1306_SendToDisplay(Commands, contrast_data, 1);
}

void ssd1306_DisplayOff(){
  uint8_t contrast_data[1] = {0xAE};
  ssd1306_SendToDisplay(Commands, contrast_data, 1);
}

static void ssd1306_SendToDisplay(SSD1306_DATA_TYPE type, uint8_t *data, uint16_t length){
	if(type) DC_SET;
	else DC_RES;

  	CS_RES;
   	for(uint16_t i=0; i<length; i++){
		*(uint8_t *)&(SPI1->DR) = data[i];
		while((SPI1->SR & SPI_SR_FTLVL) == SPI_SR_FTLVL);
	}
	while((SPI1->SR & SPI_SR_BSY));
    CS_SET;
}
