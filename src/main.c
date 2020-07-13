#include "system.h"
#include "ssd1306.h"
#include "rammap.h"
#include "menu.h"
#include "tool.h"

extern volatile uint8_t msgResponse;
extern uint8_t msgType;
extern uint8_t aa;

int main(void){
    sysInit();
    
    xdev_out(ssd1306_Char);

    while(sec<5);

    while(1){
        // ssd1306_Fill(0);
        // for(volatile uint32_t i=0; i<100000; i++);
        // if(sec<0)drawWait(10-sec);
        // else{
        //     for(uint8_t i=0; i<128; i++)ssd1306_DrawPixel(i, 63-buff[(i+aa)%128], 1);
        //     ssd1306_SetCursor(4, 0);
        //     xprintf("%u\n%u\nALM1=%u\nALM2=%u\nV=%u\n",
        //     rpm1, rpm2, (GPIOA->IDR & GPIO_IDR_5) == 0, (GPIOA->IDR & GPIO_IDR_8) == 0, 
        //     adcF[2]*3377/200+750);
        //     //drawErr(sec/5%16);
        //     //drawMain(2500, 2300, 5300, 1500);
        //     //drawDebug(rpm, 0, 0, 0, 0, sec);
        // }
        // ssd1306_UpdateScreen();

        // if(msgResponse){
        //     if(msgType==0) sendPack(0, (uint8_t*)&coreInfo, sizeof(coreInfo_t));
        //     if(msgType==1) sendPack(1, (uint8_t*)&coreStatus, sizeof(coreStatus_t));
        //     if(msgType==2) sendPack(2, (uint8_t*)&coreSetting, sizeof(coreSetting_t));
            
        //     msgResponse = 0;
        // }
    }
}
