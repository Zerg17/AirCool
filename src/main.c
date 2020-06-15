#include "system.h"
#include "ssd1306.h"

int main(void){

    sysInit();

    ssd1306_Init(0x3C);

    //xprintf("Sys init\n");

    while(1){
        ssd1306_Fill(0);

        for(uint8_t i=0; i<128; i++){
            ssd1306_DrawPixel(i, 63-adcB[i*2]/64, 1);
            ssd1306_DrawPixel(i, 63-adcB[i*2+1]/64, 1);
        }

        ssd1306_UpdateScreen();

        //xprintf("%u\n", TIM3->CNT);
    }
}