#include "system.h"
#include "ssd1306.h"

int main(void){

    sysInit();

    ssd1306_Init(0x3C);

    //xprintf("Sys init\n");

    while(1){
        for(volatile uint32_t i=0; i<100000; i++);
        ssd1306_UpdateScreen();
    }
}