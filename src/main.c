#include "system.h"
#include "ssd1306.h"
#include "rammap.h"

extern volatile uint8_t msgResponse;
extern uint8_t msgType;

unsigned char bufDisp[6][18];
uint8_t posX=0;
uint8_t posY=0;

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

uint16_t crc;

uint8_t crcf(uint8_t d){
    crc+=d*211;
    crc^=crc>>8;
}

int main(void){

    sysInit();

    ssd1306_Init(0x3C);
    xdev_out(bufDispWrite);

    xprintf("%d\n", sizeof(coreStatus_t));

    while(!USART1->BRR);
    xprintf("BRR: %u\n", F_CPU/USART1->BRR);

    while(1){
        ssd1306_Fill(0);
        for(uint8_t j=0; j<6; j++){
            ssd1306_SetCursor(0, j*10);
            for(uint8_t i=0; i<18; i++){
                ssd1306_Char(bufDisp[(j+posY+1)%6][i]);
            }
        }
        ssd1306_UpdateScreen();

        if(msgResponse){
            if(msgType==0){
                crc=0;
                uartWrite(0x55);crcf(0x55);
                uartWrite(0x00);crcf(0x00);
                uartWrite(0x00);crcf(0x00);
                uartWrite(sizeof(coreStatus_t));crcf(sizeof(coreStatus_t));
                for(uint8_t i=0; i<sizeof(coreStatus_t); i++){
                    uartWrite(((uint8_t*)&coreStatus)[i]);
                    crcf(((uint8_t*)&coreStatus)[i]);
                }
                uartWrite(crc);
            }
            msgResponse = 0;
        }
    }
}
