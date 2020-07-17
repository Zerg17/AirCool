#include "system.h"
#include "ssd1306.h"
#include "rammap.h"
#include "menu.h"
#include "uart.h"
#include "proc.h"
#include "flash.h"

extern uint8_t msgType;
extern uint8_t aa;

int main(void){
    sysInit();
    
    xdev_out(ssd1306_Char);

    if(*(uint16_t*)(0x08007C00) == 0x55AA  && *(uint16_t*)(0x08007FFC) == 0x55AA 
    && *(uint16_t*)(0x08007C02) == *(uint16_t*)(0x08007FFE))
        for(uint8_t i=0; i<(sizeof(coreSetting_t)+1)/2; i++)
            ((uint16_t*)&coreSetting)[i] = ((uint16_t*)(0x08007C04))[i] ;

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
            uint8_t buf[2];
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
                case 0x82:
                    buf[1] = checkSettingParam((coreSetting_t*)dataMsg);
                    //xfprintf(uartWrite, "%d %d\n", ((coreSetting_t*)dataMsg)->tCool, ((coreSetting_t*)dataMsg)->alrmTmin);
                    
                    if(buf[1]){
                        buf[0]=1;
                        sendPack(0x82, buf, 2);
                    }else{
                        for(uint8_t i=0; i<(sizeof(coreSetting_t)+1)/2; i++)
                            ((uint16_t*)&coreSetting)[i] = ((uint16_t*)(dataMsg))[i];
                        flashInit();
                        flashSectorClear(0x08007C00);
                        uint16_t tmp = *(uint16_t*)(0x08007C02)+1;
                        flashWrite(0x08007C00, 0x55AA);
                        flashWrite(0x08007C02, tmp);
                        for(uint8_t i=0; i<(sizeof(coreSetting_t)+1)/2; i++)
                            flashWrite(0x08007C04+i*2, ((uint16_t*)&coreSetting)[i]);
                        flashWrite(0x08007FFC, 0x55AA);
                        flashWrite(0x08007FFE, tmp);
                        buf[0]=0;
                        buf[1]=0;
                        sendPack(0x82, buf, 2);  
                    }
                    break;
            }
            
            msgResponse = 0;
        }
    }
}
