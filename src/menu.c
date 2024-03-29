#include "menu.h"
#include "ssd1306.h"
#include "system.h"
#include "rammap.h"
#include "tool.h"
#include "sysControl.h"

const uint8_t num0[] = {
    0xE0, 0xF8, 0xFC, 0x3C, 0x1E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 
    0x1E, 0x3C, 0xFC, 0xF8, 0xE0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x07, 0x1F, 0x3F, 0x3C, 0x78, 
    0x70, 0x70, 0x70, 0x70, 0x70, 0x78, 0x3C, 0x3F, 0x1F, 0x07
};

const uint8_t num1[] = {
    0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0x7C, 0xFE, 0xFE, 0xFE, 0x01, 
    0x03, 0x03, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x7F, 0x7F, 0x7F
};

const uint8_t num2[] = {
    0x70, 0x78, 0x7C, 0x1C, 0x1E, 0x0E, 0x0E, 0x0E, 0x0E, 0x1E, 
    0x1C, 0x7C, 0xF8, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF8, 0x7F, 0x1F, 0x0F, 
    0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xF8, 0x7E, 0x1F, 0x0F, 
    0x03, 0x01, 0x00, 0x00, 0x00, 0x70, 0x78, 0x7E, 0x7F, 0x7F, 
    0x73, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70
};

const uint8_t num3[] = {
    0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 
    0x1C, 0x3C, 0xF8, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0xE0, 0xE0, 
    0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF8, 0xBF, 0x1F, 0x0F, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x01, 0x03, 0xFF, 0xFF, 0xFC, 0x70, 0x70, 0x70, 0x70, 0x70, 
    0x70, 0x70, 0x70, 0x70, 0x70, 0x38, 0x3C, 0x1F, 0x0F, 0x03
};

const uint8_t num4[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFE, 0xFE, 0x1E, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x80, 0xF8, 0xFF, 0x7F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0xFF, 0xFF, 
    0xC3, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xFF, 
    0xC0, 0xC0, 0xC0, 0xC0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 0x01, 0x7F, 0x7F, 0x7F, 0x01, 0x01, 
    0x01, 0x01
};

const uint8_t num5[] = {
    0xFE, 0xFE, 0xFE, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 
    0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 
    0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x01, 0x03, 0xFF, 0xFF, 0xFC, 0x70, 0x70, 0x70, 0x70, 0x70, 
    0x70, 0x70, 0x70, 0x70, 0x70, 0x38, 0x3C, 0x1F, 0x0F, 0x03
};

const uint8_t num6[] = {
    0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0x78, 0x3C, 0x1C, 0x1E, 
    0x0E, 0x0E, 0x00, 0x00, 0x00, 0xE0, 0xFC, 0xFF, 0xFF, 0xE3, 
    0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0x80, 0x00, 
    0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0xFF, 0xFF, 0xFE, 0x07, 0x1F, 0x3F, 0x38, 0x70, 
    0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x38, 0x3F, 0x1F, 0x07
};

const uint8_t num7[] = {
    0x7E, 0x7E, 0x7E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 
    0x0E, 0xFE, 0xFE, 0xFE, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x80, 0xF8, 0xFF, 0x7F, 0x0F, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF8, 0xFF, 0x7F, 
    0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x7C, 0x7F, 0x3F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t num8[] = {
    0x00, 0x00, 0xC0, 0xF0, 0xF8, 0x3C, 0x1C, 0x0E, 0x0E, 0x0E, 
    0x0E, 0x0E, 0x1C, 0x3C, 0xF8, 0xF0, 0xC0, 0x00, 0x00, 0x00, 
    0x07, 0x9F, 0xFF, 0xF8, 0xF0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 
    0xF0, 0xF8, 0xFF, 0x9F, 0x07, 0x00, 0xF8, 0xFE, 0xFF, 0x07, 
    0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 
    0x07, 0xFF, 0xFF, 0xFC, 0x01, 0x07, 0x0F, 0x1E, 0x3C, 0x38, 
    0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x38, 0x3C, 0x1E, 0x0F, 
    0x07, 0x01
};

const uint8_t num9[] = {
    0xE0, 0xF8, 0xFC, 0x1C, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 
    0x0E, 0x1C, 0xFC, 0xF8, 0xE0, 0x7F, 0xFF, 0xFF, 0x80, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 
    0x00, 0x01, 0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
    0xC7, 0xFF, 0xFF, 0x3F, 0x07, 0x00, 0x00, 0x00, 0x70, 0x70, 
    0x78, 0x38, 0x3C, 0x1E, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00
};

const uint8_t dot[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 
    0x70, 0x70
};

const uint8_t dotdot[] = {
    0x00, 0x00, 0x00, 0x0E, 0x0E, 0x0E, 0xE0, 0xE0, 0xE0, 0x00, 
    0x00, 0x00
};

const uint8_t minus[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t numS[] = {15, 9, 15, 15, 18, 15, 15, 15, 18, 15, 3, 3, 10};                                   // Ширина символов
const uint8_t* numA[] = {num0, num1, num2, num3, num4, num5, num6, num7, num8, num9, dot, dotdot, minus};   // Указатели на массивы символов

uint8_t numPrint(uint8_t n, uint8_t x){ // Вывод символов в виде изображения 4 строки в высоту
    uint8_t w = numS[n];
    for(uint8_t i=0; i<w*4; i++)
        SSD1306_Buffer[i%w+(i/w*128)+128*3+x]=numA[n][i];
    return w;
}

void printTemp(int32_t t){  // Вывод температуры
    uint8_t xp;
    uint8_t m=0;
    if(t<0){
        t=-t;
        m=numS[12];
    }
    uint8_t size=m;
    if(t>=10000)size+=numS[t/10000%10]+2;
    if(t>=1000)size+=numS[t/1000%10]+2;
    size+=numS[t/100%10]+2;
    size+=numS[10]+2;
    size+=numS[t/10%10]+2;

    xp=37-size/2;
    if(m)xp+=numPrint(12, xp)+2;
    if(t>=10000)xp+=numPrint(t/1000%10, xp)+2;
    if(t>=1000)xp+=numPrint(t/1000%10, xp)+2;
    xp+=numPrint(t/100%10, xp)+2;
    if(size<68){  
        xp+=numPrint(10, xp)+2;
        xp+=numPrint(t/10%10, xp)+2;
    }
}

void drawWait(uint16_t t){  // Вывод таймера ожидания
    uint8_t xp=63-(numS[t/600%10]+numS[t/60%10]+9);
    ssd1306_SetCursor(64-sizeof("Ожидание")/2*7,4);
    ssd1306_WriteString("Ожидание", Font_7x9, White);
    ssd1306_DrawRect(0, 0, 127, 63, 1);
    ssd1306_DrawLine(0, 16, 127, 16, 1);
    xp+=numPrint(t/600%10, xp)+2;
    xp+=numPrint(t/60%10, xp)+6;
    xp+=numPrint(11, xp)+6;
    xp+=numPrint(t%60/10%10, xp)+2;
    numPrint(t%10, xp);
}

void drawErr(uint8_t err, char* str){   // Вывод сообщения об ошибки
    char bufC[12];
    ssd1306_DrawRect(0, 0, 127, 63, 1);
    ssd1306_DrawLine(0, 16, 127, 16, 1);
    xsprintf(bufC, "Ошибка:E%u", err);
    ssd1306_SetCursor(32,4);
    ssd1306_WriteString(bufC, Font_7x9, White);
    ssd1306_SetCursor(INDENT, 20);
    ssd1306_WriteString(str, Font_7x9, White);
}

void drawMain(char* str){   // Вывод главного экрана
    ssd1306_DrawRect(0, 0, 127, 63, 1);
    ssd1306_DrawLine(0, 16, 127, 16, 1);
    ssd1306_DrawLine(70, 16, 70, 63, 1);
    ssd1306_DrawLine(70, 42, 127, 42, 1);
    ssd1306_DrawLine(97, 42, 97, 63, 1);
    
    ssd1306_SetCursor(64-strl(str)/2*7,4);
    ssd1306_WriteString(str, Font_7x9, White);

    ssd1306_SetCursor(75, 26);
    uint16_t rpmMax = max(coreStatus.rpmFan1, coreStatus.rpmFan2);
    xprintf("%04dRPM", rpmMax);

    ssd1306_SetCursor(75, 49);
    xprintf("%uV", (coreStatus.voltage+50)/100%100);

    ssd1306_SetCursor(102, 49);
    xprintf("%u", coreStatus.current/100);
    ssd1306_SetCursor(107, 49);
    xprintf(".");
    ssd1306_SetCursor(112, 49);
    xprintf("%uA", coreStatus.current%100/10);

    printTemp(coreStatus.temp1);
}

void updateMenu(){  // Обновления меню экрана
    ssd1306_Fill(0);
    switch(coreStatus.mode){
        case waitStartMode: drawWait(timWaitNext/100); break;
        case watiTestMode: drawMain("Ожидание запуска"); break;
        case waitMode: 
            if(!coreStatus.alm2) drawMain("Ожидание");
            else drawMain("Нет первичной сети");
            break;
        case coolMode: drawMain("Охлаждение"); break;
        case heatMode: drawMain("Нагрев"); break;
        case testFun1Mode: drawMain("Тест 1 вентилятора"); break;
        case testFun2Mode: drawMain("Тест 2 вентилятора"); break;
        case testHeatMode: drawMain("Тест нагревателя"); break;
        case testCoolMode: drawMain("Тест компрессора"); break;
        case befStartMode: drawMain("Ожидание запуска"); break;
        case manCtrMode: drawMain("Ручной режим!!!"); break;
        case offMode: drawMain("Выключен"); break;
        case errMode: 
            if(coreStatus.errFun2 == 1){drawErr(1, "Аварийный сигнал внутреннего вентилятора");break;}
            if(coreStatus.errFun1 == 1){drawErr(3, "Аварийный сигнал внешнего вентилятора");break;}
            if(coreStatus.errHighCurrentCool == 1){drawErr(6, "Аварийный сигнал превышения тока компрессора");break;}
            if(coreStatus.errLowCurrentCool == 1){drawErr(5, "Аварийный сигнал слабого тока компрессора");break;}
            if(coreStatus.alm1 == 1 || coreStatus.alm2 == 1){drawErr(13, "Аварийный сигнал панели управления");break;}
            if(coreStatus.errHighCurrentHeat == 1){drawErr(15, "Аварийный сигнал превышения тока нагревателя");break;}
            if(coreStatus.errLowCurrentHeat == 1){drawErr(14, "Аварийный сигнал низкого тока нагревателя");break;}
            if(coreStatus.errHighVoltage == 1){drawErr(17, "Аварийный сигнал превышения напряжения");break;}
            if(coreStatus.errLowVoltage == 1){drawErr(16, "Аварийный сигнал низкого напряжения");break;}
            if(coreStatus.errTmp1 == 1){drawErr(18, "Аварийный сигнал датчика температуры 1");break;}
            if(coreStatus.errTmp2 == 1){drawErr(19, "Аварийный сигнал датчика температуры 2");break;}
            if(coreStatus.errT1max == 1){drawErr(10, "Аварийный сигнал высокой температуры");break;}
            if(coreStatus.errT1min == 1){drawErr(11, "Аварийный сигнал низкой температуры");break;}
            if(coreStatus.errT2max == 1 || coreStatus.errT2min == 1){drawErr(7, "Аварийный сигнал датчика внутренней температуры");break;}
            
            drawErr(0, "");
        break;
    }
    ssd1306_UpdateScreen();
}