#include <stdint.h>
#include "system.h"
#include "rammap.h"

uint16_t volatile rpm1, rpm2;

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
    //TIM3->CCR2 = sec%40>20?2500:5000; 
    
    coreStatus.time=sec;


    if(tick%10==0){
        xfprintf(uartWrite, "%u\n", rpm2);
    }

    if(tick%40==0){
        buff[aa++]=rpm2/32;
        if(aa==128)aa=0;
    }

    // if(tick%200 == 0){
    //     msgType=1;
    //     msgResponse=1;
    // }

    if(msgFlug){
        if(msgType<3 && msgLen==0)msgResponse=1;
        msgFlug=0;
    }
}

void EXTI4_15_IRQHandler(){
    uint16_t t = TIM14->CNT;
    static uint16_t lt1, lt2;

    if((EXTI->PR & EXTI_PR_PR4) == EXTI_PR_PR4){
        EXTI->PR |= EXTI_PR_PR4;
        uint16_t dt=t-lt1;
        if(dt>2400U){
            for(volatile uint16_t i=0; i<400; i++);
            for(volatile uint8_t i=0; i<25; i++);
            if(!(GPIOB->IDR & GPIO_IDR_4)){
                rpm1=9600000/dt;
                lt1=t; 
            }
        }
    }

    if((EXTI->PR & EXTI_PR_PR12) == EXTI_PR_PR12){
        EXTI->PR |= EXTI_PR_PR12;
        uint16_t dt=t-lt2;
        if(dt>2400U){
            for(volatile uint16_t i=0; i<400; i++);
            if(!(GPIOA->IDR & GPIO_IDR_12)){
                rpm2=9600000/dt;
                lt2=t; 
            }
        }
    }
}

void DMA1_Channel1_IRQHandler(void){
    static uint32_t acc[4] = {2048*512, 2048*512, 2800*512};
    if(DMA1->ISR & DMA_ISR_TCIF1){
        DMA1->IFCR=DMA_IFCR_CTCIF1;
        for(uint8_t i=0; i<4; i++){
            acc[i]+=adcR[i]-adcF[i];
            adcF[i]=acc[i]/512;
        }
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
    }
    if(USART1->ISR & USART_ISR_NE){     //  START bit Noise detection flag
        USART1->ICR = USART_ICR_NCF;
    }
}