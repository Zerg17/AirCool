#include "system.h"
#include "ssd1306.h"
#include "rammap.h"
#include "msg.h"
#include "tool.h"

extern volatile uint8_t msgResponse;
extern uint8_t msgType;

unsigned char bufDisp[6][18];
uint8_t posX=0;
uint8_t posY=0;

uint32_t t1, t2;

void bufDispWrite(unsigned char c){
    if(c=='\n'){
        posX=0;
        posY=(posY+1)%6;
        for(uint8_t i=0; i<18; i++)bufDisp[posY][i]=0;
    }else if(c=='\r'){
        posX=0;
    }
    else{
        bufDisp[posY][posX]=c;
        if(++posX==18){
            posX=0;
            posY=(posY+1)%6;
            for(uint8_t i=0; i<18; i++)bufDisp[posY][i]=0;
        }
    }
}

int main(void){
    sysInit();

    ssd1306_Init(0x3C);
    xdev_out(bufDispWrite);

    // coreStatus.current=1000;
    // coreStatus.rmpFan1=2200;
    // coreStatus.rmpFan2=0;
    // coreStatus.temp1=2600;
    // coreStatus.temp2=1800;
    // coreStatus.voltage=5600;

    while(1){
        // ssd1306_Fill(0);
        // for(uint8_t j=0; j<6; j++){
        //     ssd1306_SetCursor(0, j*10);
        //     for(uint8_t i=0; i<18; i++){
        //         ssd1306_Char(bufDisp[(j+posY+1)%6][i]);
        //     }
        // }
        // ssd1306_UpdateScreen();

        ssd1306_Fill(0);
        ssd1306_DrawRect(0, 0, 127, 63, 1);
        ssd1306_DrawLine(0, 15, 127, 15, 1);
        ssd1306_DrawLine(63, 15, 63, 63, 1);
        ssd1306_DrawLine(0, 42, 63, 42, 1);
        ssd1306_DrawLine(31, 42, 31, 63, 1);
        ssd1306_SetCursor(64-sizeof(WAIT_MSG)/2*7,4);
        ssd1306_WriteString(WAIT_MSG, Font_7x9, White);
        ssd1306_UpdateScreen();

        if(msgResponse){
            if(msgType==0) sendPack(0, (uint8_t*)&coreInfo, sizeof(coreInfo_t));
            if(msgType==1) sendPack(1, (uint8_t*)&coreStatus, sizeof(coreStatus_t));
            if(msgType==2) sendPack(2, (uint8_t*)&coreSetting, sizeof(coreSetting_t));
            
            msgResponse = 0;
        }
    }
}
