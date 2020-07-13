#include "uart.h"
#include "system.h"
#include "tool.h"

uint8_t msgFlug = 0;
uint8_t msgType = 0;
uint8_t msgLen = 0;
uint8_t dataMsg[255];

void uartWrite(uint8_t d){
    while(!(USART1->ISR & USART_ISR_TXE));
    USART1->TDR=d;
}

void uartInit(){
    USART1->BRR = (F_CPU+BOAD/2)/BOAD;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE;
    USART1->CR2 = USART_CR2_ABREN | USART_CR2_ABRMODE_Msk;
    USART1->CR1 |= USART_CR1_UE;
    while(!(USART1->ISR & (USART_ISR_TEACK | USART_ISR_REACK)));
    USART1->CR1 |= USART_CR1_RXNEIE;
    USART1->CR3 = USART_CR3_EIE;
    NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_IRQHandler(void){
    static uint16_t crc;
    static uint8_t status=0;
    static uint8_t count;
    static uint32_t tim;

    if(USART1->ISR & USART_ISR_RXNE){
        uint8_t d = USART1->RDR;
        if(tick-tim>100)status=0;
        tim=tick;
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