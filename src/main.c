#include "system.h"
#include "ssd1306.h"
#include "rammap.h"
#include "menu.h"
#include "uart.h"
#include "proc.h"

extern uint8_t msgType;
extern uint8_t aa;

int main(void){
    sysInit();
    uartWrite('A');
    
    xdev_out(ssd1306_Char);

    while(sec<5);

    while(1){
        ssd1306_Fill(0);
        if(!(coreStatus.status & CRITICAL_ERR)){
            if(sec<=305)drawWait(305-sec);
            else drawMain();
        }else{
            if(coreStatus.off) drawErr(0);
            else if(coreStatus.errFun1) drawErr(1);
            else if(coreStatus.errFun2) drawErr(2);
            else if(coreStatus.lowCurrentCool) drawErr(5);
            else if(coreStatus.highCurrentCool) drawErr(6);
            else if(coreStatus.errTmp1) drawErr(7);
            else if(coreStatus.errTmp2) drawErr(9);
            else if(coreStatus.highTemp) drawErr(10);
            else if(coreStatus.lowTemp) drawErr(11);
            else if(coreStatus.lowCurrentHeat) drawErr(14);
            else if(coreStatus.highCurrentHeat) drawErr(15);
        }
        ssd1306_UpdateScreen();

        if(msgResponse){
            switch(msgResponseType){
                case 0:
                    sendPack(0, (uint8_t*)&coreInfo, sizeof(coreInfo_t));
                    break;
                case 1:
                    sendPack(1, (uint8_t*)&coreStatus, sizeof(coreStatus_t));
                    break;
                case 2:
                    sendPack(2, (uint8_t*)&coreSetting, sizeof(coreSetting_t));
                    break;
                case 0x72:
                    sendPack(0xF2, 0, 0);
                    break;
                case 0x70:
                    sendPack(0xF0, 0, 0);
                    break;
                case 0x71:
                    sendPack(0xF1, 0, 0);
                    break;
            }
            
            msgResponse = 0;
        }
    }
}
