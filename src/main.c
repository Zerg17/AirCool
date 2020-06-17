#include "system.h"
#include "ssd1306.h"

#define B 3950
#define S_R 10000
#define T_R 10000
#define N_T 25


long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

extern uint8_t aa;
extern uint16_t aRB[128];
extern uint16_t aFB[128];

int main(void){

    sysInit();

    ssd1306_Init(0x3C);

    //xprintf("Sys init\n");

    while(1){
        ssd1306_Fill(0);

        uint16_t min=65535;
        uint16_t max=0;

        for(uint8_t i=0; i<128; i++){
            if(min>aRB[i])min=aRB[i];
            if(max<aRB[i])max=aRB[i];
        }

        for(uint8_t i=0; i<128; i++){
            ssd1306_DrawPixel(i, map(aRB[(i+aa)%128], min, max, 63, 0), 1);
            ssd1306_DrawPixel(i, map(aFB[(i+aa)%128], min, max, 63, 0), 1);
        }

        ssd1306_SetCursor(0, 0);
        xfprintf(ssd1306_Char, "%d", sec);

        ssd1306_SetCursor(0, 10);
        int32_t t = tempCalc(adcR[0]);
        xfprintf(ssd1306_Char, "%d.%02d", t/65536, abs(((t*100)/65536)%100));

        ssd1306_SetCursor(0, 20);
        t = tempCalc(adcF[0]);
        xfprintf(ssd1306_Char, "%d.%02d", t/65536, abs(((t*100)/65536)%100));
  
        ssd1306_UpdateScreen();

        //xprintf("%u\n", TIM3->CNT);
    }
}