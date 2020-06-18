#include "system.h"
#include "ssd1306.h"

#define B 3950
#define S_R 10000
#define T_R 10000
#define N_T 25

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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
        if(++posX==18)posX=0;
    }
}

int main(void){

    sysInit();

    ssd1306_Init(0x3C);
    xdev_out(bufDispWrite);

    while(1){
        ssd1306_Fill(0);
        for(uint8_t j=0; j<6; j++){
            ssd1306_SetCursor(0, j*10);
            for(uint8_t i=0; i<18; i++){
                ssd1306_Char(bufDisp[(posY+j+1)%6][i]);
            }
        }
        ssd1306_UpdateScreen();

        //xprintf("%u\n", TIM3->CNT);
    }
}