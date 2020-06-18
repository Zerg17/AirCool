#include <stdint.h>
#include "system.h"

uint32_t tick=0;
uint32_t sec_d=0;
uint32_t sec=0;

uint8_t aa=0;

void SysTick_Handler(void) {
    tick++;
    if(++sec_d==100){
        sec_d=0;
        sec++;
    }
}

void DMA1_Channel1_IRQHandler(void){
    static uint32_t acc[2] = {2048*1024, 2048*1024};
    if(DMA1->ISR & DMA_ISR_TCIF1){
        DMA1->IFCR=DMA_IFCR_CTCIF1;
        acc[0]+=adcR[0]-adcF[0];
        adcF[0]=acc[0]/1024;
    }
}