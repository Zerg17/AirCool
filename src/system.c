#include "system.h"

void sysInit(){
    rccInit();
    gpioInit();
    uartInit();
    i2cInit();
    adcInit();
    SysTick_Config(F_CPU/100);
}

void rccInit(){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_DMAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_ADCEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN | RCC_APB1ENR_TIM3EN;
}

void gpioInit(){
    GPIOA->OTYPER |= GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_10;
    //GPIOA->PUPDR |= GPIO_PUPDR_PUPDR3_0 | GPIO_PUPDR_PUPDR2_0;
    GPIOA->MODER |= GPIO_MODER_MODER0_Msk | GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1 | GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;
    GPIOA->AFR[0] |= 0x00001100;
    GPIOA->AFR[1] |= 0x00000440;
}

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

void i2cInit(){
    I2C1->TIMINGR = 0x00000404;
    I2C1->CR1 = I2C_CR1_PE;
}

uint16_t adcR[2];
uint16_t adcF[2];

void adcInit(){
    ADC1->CR = ADC_CR_ADCAL;
    while(ADC1->CR & ADC_CR_ADCAL);
    ADC1->SMPR = 0b111;
    ADC1->CR = ADC_CR_ADEN;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));

    ADC1->CFGR1 = ADC_CFGR1_DMACFG | ADC_CFGR1_DMAEN | ADC_CFGR1_EXTEN_0 | ADC_CFGR1_EXTSEL_1 | ADC_CFGR1_EXTSEL_0;
    ADC1->CHSELR = 0b1;

    DMA1_Channel1->CNDTR = 1;
    DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));
    DMA1_Channel1->CMAR = (uint32_t)adcR;
    DMA1_Channel1->CCR = DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_MINC | DMA_CCR_CIRC | DMA_CCR_TCIE;
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    
    ADC1->CR |= ADC_CR_ADSTART;

    TIM3->CR2 |= TIM_CR2_MMS_1;
    TIM3->PSC = 0;
    TIM3->ARR = 7999;
    TIM3->CR1 |= TIM_CR1_CEN;

    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

// void adcInit(){
//     ADC1->CR = ADC_CR_ADCAL;
//     while(ADC1->CR & ADC_CR_ADCAL);
//     ADC1->SMPR = 0b111;
//     ADC1->CR = ADC_CR_ADEN;
//     while(!(ADC1->ISR & ADC_ISR_ADRDY));

//     ADC1->CFGR1 = ADC_CFGR1_CONT | ADC_CFGR1_DMACFG | ADC_CFGR1_DMAEN;
//     ADC1->CHSELR = 0b11;
//     DMA1_Channel1->CNDTR = 256;
//     DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));
//     DMA1_Channel1->CMAR = (uint32_t)adcR;
//     DMA1_Channel1->CCR = DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_MINC | DMA_CCR_CIRC;
//     DMA1_Channel1->CCR |= DMA_CCR_EN;
//     ADC1->CR |= ADC_CR_ADSTART;
// }