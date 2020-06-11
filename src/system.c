#include "system.h"

void sysInit(){
    rccInit();
    gpioInit();
    uartInit();
    i2cInit();
}

void rccInit(){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
}

void gpioInit(){
    GPIOA->OTYPER |= GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_10;
    GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;
    GPIOA->AFR[0] |= (1<<GPIO_AFRL_AFRL2_Pos);
    GPIOA->AFR[1] |= 0x00000440;
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

void i2cInit(){
    I2C1->TIMINGR = 0x00000404;
    I2C1->CR1 = I2C_CR1_PE;
}
