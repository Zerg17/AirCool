#include "system.h"
#include "sysControl.h"
#include "ssd1306.h"
#include "rammap.h"
#include "menu.h"
#include "uart.h"
#include "flash.h"

extern uint8_t msgType;
extern uint8_t aa;

int main(void){
    sysInit();
    xdev_out(ssd1306_Char);
    loadFlashSetting();
    while(sec<5);

    while(1){
        ssd1306_Fill(0);
        switch(coreStatus.mode){
            case waitStartMode: drawWait(timWaitNext/100); break;
            case waitMode: drawMain("Ожидание"); break;
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
            break;
        }
        ssd1306_UpdateScreen();
    }
}
