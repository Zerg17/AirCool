#include "system.h"

uint8_t buff[128];

void rccInit(){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOFEN | RCC_AHBENR_DMAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_ADCEN | RCC_APB2ENR_SPI1EN | RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM14EN;
}

void gpioInit(){
    // PA4  - SSD1306_CS    - GPIO_OUT
    // PA11 - SSD1306_DC    - GPIO_OUT
    // PB3  - SSD1306_SCK   - SPI
    // PB5  - SSD1306_SDA   - SPI
    // PF0  - SSD1306_RESET - GPIO_OUT
    // PA0  - TEMP1         - ADC0
    // PA1  - TEMP2         - ADC1
    // PA2  - VOLTAGE       - ADC2
    // PA3  - CURRENT       - ADC3
    // PA6  - OU_INT        - TIM3_CH1
    // PA7  - OU_EXT        - TIM3_CH2

    GPIOA->MODER |= GPIO_MODER_MODER4_0 | GPIO_MODER_MODER11_0;
    GPIOB->MODER |= GPIO_MODER_MODER3_1 | GPIO_MODER_MODER5_1;
    GPIOF->MODER |= GPIO_MODER_MODER0_0;

    GPIOA->MODER |= GPIO_MODER_MODER0_Msk | GPIO_MODER_MODER1_Msk | GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk;

    GPIOA->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
    GPIOA->AFR[0]|= 0x11000000;

    GPIOB->MODER |= GPIO_MODER_MODER6_1;
}

void extiInit(){
    TIM14->PSC = 24;
    TIM14->CR1 |= TIM_CR1_CEN;

    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PB;

    EXTI->FTSR = (1<<4) | (1<<12);
    EXTI->IMR =  (1<<4) | (1<<12);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void uartWrite(uint8_t d){
    while(!(USART1->ISR & USART_ISR_TXE));
    USART1->TDR=d;
}

void uartInit(){
    USART1->BRR = (F_CPU+BOAD/2)/BOAD;
    USART1->CR1 = USART_CR1_TE;//USART_CR1_RE
    USART1->CR2 = USART_CR2_ABREN | USART_CR2_ABRMODE_Msk;
    USART1->CR1 |= USART_CR1_UE;
    while(!(USART1->ISR & (USART_ISR_TEACK | USART_ISR_REACK)));
    USART1->CR1 |= USART_CR1_RXNEIE;
    USART1->CR3 = USART_CR3_EIE;
    //NVIC_EnableIRQ(USART1_IRQn);
}

void spiInit(void){
    SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_MSTR;
    SPI1->CR1 |= SPI_CR1_SPE;
}

void i2cInit(){
    I2C1->TIMINGR = 0x00000404;
    I2C1->CR1 = I2C_CR1_PE;
}

void flashInit(){
    while ((FLASH->SR & FLASH_SR_BSY) != 0);
    if ((FLASH->CR & FLASH_CR_LOCK) != 0){
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }
}

void flashSectorClear(uint32_t adr){
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = adr;
    FLASH->CR |= FLASH_CR_STRT;

    while ((FLASH->SR & FLASH_SR_BSY) != 0);

    if ((FLASH->SR & FLASH_SR_EOP) != 0)
        FLASH->SR = FLASH_SR_EOP;

    FLASH->CR &= ~FLASH_CR_PER;
}

void flashWrite(uint32_t adr, uint16_t data){
    FLASH->CR |= FLASH_CR_PG;
    *(__IO uint16_t*)(adr) = data;

    while ((FLASH->SR & FLASH_SR_BSY) != 0);

    if ((FLASH->SR & FLASH_SR_EOP) != 0)
        FLASH->SR = FLASH_SR_EOP;
    FLASH->CR &= ~FLASH_CR_PG;
}

uint16_t adcR[4];
uint16_t adcF[4];

void adcInit(){
    if ((ADC1->CR & ADC_CR_ADEN) != 0) ADC1->CR |= ADC_CR_ADDIS;
    while ((ADC1->CR & ADC_CR_ADEN) != 0);
    ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN;
    ADC1->CR = ADC_CR_ADCAL;
    while(ADC1->CR & ADC_CR_ADCAL);
    if ((ADC1->ISR & ADC_ISR_ADRDY) != 0);
    
    ADC1->CR = ADC_CR_ADEN;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));

    ADC1->SMPR = 0b111;

    ADC1->CFGR1 = ADC_CFGR1_DMACFG | ADC_CFGR1_DMAEN | ADC_CFGR1_EXTEN_0 | ADC_CFGR1_EXTSEL_1 | ADC_CFGR1_EXTSEL_0;
    ADC1->CHSELR = 0b1111;

    DMA1_Channel1->CNDTR = 4;
    DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));
    DMA1_Channel1->CMAR = (uint32_t)adcR;
    DMA1_Channel1->CCR = DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_MINC | DMA_CCR_CIRC | DMA_CCR_TCIE;
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    
    ADC1->CR |= ADC_CR_ADSTART;

    TIM3->CCMR1 = (0x06 << TIM_CCMR1_OC1M_Pos) | (0x06 << TIM_CCMR1_OC2M_Pos);
    TIM3->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM3->CR1 = TIM_CR1_ARPE;
    TIM3->CR2 = TIM_CR2_MMS_1;
    TIM3->PSC = 0;
    TIM3->ARR = 7999;
    TIM3->CR1 |= TIM_CR1_CEN;

    TIM3->CCR1 = 3000;
    TIM3->CCR2 = 3000;

    //NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void sysInit(){
    rccInit();
    gpioInit();
    uartInit();
    spiInit();
    adcInit();
    extiInit();
    SysTick_Config(F_CPU/100);
}
