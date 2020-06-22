#include <stdint.h>
#include "system.h"
#include "rammap.h"

uint8_t msgFlug = 0;
uint8_t msgType = 0;
uint8_t msgLen = 0;
uint8_t msgResponse=0;
uint8_t dataMsg[255];

uint32_t volatile tick=0;
uint32_t sec_d=0;
uint32_t volatile sec=0;

uint8_t aa=0;

void SysTick_Handler(void) {
    tick++;
    if(++sec_d==100){
        sec_d=0;
        sec++;   
    }

    coreStatus.time=sec;

    // if(tick%200 == 0){
    //     msgType=1;
    //     msgResponse=1;
    // }

    if(msgFlug){
        if(msgType<3 && msgLen==0)msgResponse=1;
        msgFlug=0;
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

void bufDispWrite(unsigned char c);

void USART1_IRQHandler(void){
    static uint16_t crc;
    static uint8_t status=0;
    static uint8_t count;
    static uint32_t tim;

    if(USART1->ISR & USART_ISR_RXNE){
        uint8_t d = USART1->RDR;
        if(tick-tim>100)status=0;
        tim=tick;
        //if(d==0x55)xprintf("\n", d);
        //xprintf("%02X %02X %d\n", d, 0xFF&crc, status);
        switch(status){
            case 0: if(d==0x55)status=1;crc=0; break;
            case 1: if(d==0)status=2; else status=0; break;
            case 2: msgLen=d; count=0; status=3; break;
            case 3: msgType=d; status=4; break;
            case 4: 
                if(count!=msgLen){
                    dataMsg[count++]=d;
                    break;
                }
            case 5: 
                if(d==(0xFF&crc))msgFlug=1;
                status=0;
                break;
        }
        crc+=d*211;
        crc^=crc>>8;
    }
    if(USART1->ISR & USART_ISR_FE){     // Framing error
        USART1->ICR = USART_ICR_FECF;
    }
    if(USART1->ISR & USART_ISR_ORE){    // Overrun error    
        USART1->ICR = USART_ICR_ORECF;
        xprintf("O");
    }
    if(USART1->ISR & USART_ISR_NE){     //  START bit Noise detection flag
        USART1->ICR = USART_ICR_NCF;
        xprintf("N");
    }
}