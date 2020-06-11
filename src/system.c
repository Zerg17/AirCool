#include "system.h"

void sysInit(){
    rccInit();
    gpioInit();
    uartInit();
}

void rccInit(){
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}

void gpioInit(){
    GPIOA->MODER |= GPIO_MODER_MODER2_1;
    GPIOA->AFR[0] |= (1<<GPIO_AFRL_AFRL2_Pos);
}

void uartChar(unsigned char c){
    while(!(USART1->ISR & USART_ISR_TC));
        USART1->TDR=c;
}

void uartInit(){
    xdev_out(uartChar);
    USART1->BRR = (F_CPU+BOAD/2)/BOAD;
    USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}