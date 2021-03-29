#include "system.h"
#include "uart.h"
#include "sysControl.h"
#include "ssd1306.h"

/////////////////////////////////////////////////////////////////////

void rccInit(){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOFEN | RCC_AHBENR_DMAEN | RCC_AHBENR_CRCEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_ADCEN | RCC_APB2ENR_SPI1EN | RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM14EN;
}

/////////////////////////////////////////////////////////////////////

void gpioInit(){
    // PA15 - SERIAL_RX     - USART1_RX
    // PA12 - THO_OUT       - EXTI4_15
    // PA11 - SSD1306_DC    - GPIO_OUT
    //// PA9  - SERIAL_TX     - USART1_TX
    // PA8  - ALM2          - GPIO_IN
    // PA7  - OU_EXT        - TIM3_CH2
    // PA6  - OU_INT        - TIM3_CH1
    // PA5  - ALM1          - GPIO_IN
    // PA4  - SSD1306_CS    - GPIO_OUT
    // PA3  - CURRENT       - ADC3
    // PA2  - VOLTAGE       - ADC2
    // PA1  - TEMP2         - ADC1
    // PA0  - TEMP1         - ADC0
    // PB7  - ALARM_RELAY   - GPIO_OUT
    // PB6  - SERIAL_TX     - USART1_TX
    // PB5  - SSD1306_SDA   - SPI
    // PB4  - THO_IN        - EXTI4_15
    // PB3  - SSD1306_SCK   - SPI
    // PB1  - COOL_RELAY    - GPIO_OUT
    // PB0  - HEAT_RELAY    - GPIO_OUT
    // PF0  - SSD1306_RESET - GPIO_OUT

    // 0 - GPIO_OUT
    // 1 - Alternate function
    // Msk - Analog mode

    GPIOA->MODER |= GPIO_MODER_MODER15_1 | GPIO_MODER_MODER11_0 | /*GPIO_MODER_MODER9_1 |*/ GPIO_MODER_MODER7_1 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER3_Msk | GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER1_Msk | GPIO_MODER_MODER0_Msk;
    GPIOB->MODER |= GPIO_MODER_MODER7_0 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER5_1 | GPIO_MODER_MODER3_1 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER0_0;
    GPIOF->MODER |= GPIO_MODER_MODER0_0;
    GPIOA->AFR[1]|= 0x10000000;
    GPIOA->AFR[0]|= 0x11000000;
}

/////////////////////////////////////////////////////////////////////

// Настройка внешних перрываний для измерения скорости вентиляторов
void extiInit(){
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PB;
    EXTI->FTSR = (1<<4) | (1<<12);  // Включаем триггер внешнего прерывания по фронту
    EXTI->IMR =  (1<<4) | (1<<12);
    NVIC_SetPriority(EXTI4_15_IRQn, 1);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/////////////////////////////////////////////////////////////////////

// Настройка SPI для дисплея ssd1306
void spiInit(void){
    SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_MSTR;
    SPI1->CR1 |= SPI_CR1_SPE;
}

/////////////////////////////////////////////////////////////////////

// Настройка ADC для работы с 3-0 каналами по внешнему триггеру 3 таймера
void adcInit(){
    // Сброс ADC если был запущен
    if ((ADC1->CR & ADC_CR_ADEN) != 0) ADC1->CR |= ADC_CR_ADDIS;
    while ((ADC1->CR & ADC_CR_ADEN) != 0);
    ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN;

    // Калибровка ADC
    ADC1->CR = ADC_CR_ADCAL;
    while(ADC1->CR & ADC_CR_ADCAL);
    if ((ADC1->ISR & ADC_ISR_ADRDY) != 0);
    
    // Запуск ADC
    ADC1->CR = ADC_CR_ADEN;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));

    ADC1->SMPR = 0b111;  // Время зарядки емкости ADC 239.5 тактов

    ADC1->CFGR1 
        = ADC_CFGR1_DISCEN  // Прерывистый режим
        | ADC_CFGR1_EXTEN_0 // Работа по фронту триггера
        | ADC_CFGR1_EXTSEL_1 | ADC_CFGR1_EXTSEL_0 // Выбираем 3 таймер в качестве триггера
        | ADC_CFGR1_DMACFG  // Циклический режим работы с ДМА
        | ADC_CFGR1_DMAEN;   // Разрешение работы с ДМА  
        
    ADC1->CHSELR = 0b1111;  // Выбираем каналы для преобразования 3-0

    DMA1_Channel1->CNDTR = 4;
    DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));
    DMA1_Channel1->CMAR = (uint32_t)adcR;
    DMA1_Channel1->CCR = DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_MINC | DMA_CCR_CIRC | DMA_CCR_TCIE;
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    
    ADC1->CR |= ADC_CR_ADSTART;  // Запускаем ADC для отслеживания триггера

    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/////////////////////////////////////////////////////////////////////

// Настройка TIM3 для управление вентиляторами и генерации триггера для ADC
void tim3Init(){
    TIM3->CCMR1 = (0x06 << TIM_CCMR1_OC1M_Pos) | (0x06 << TIM_CCMR1_OC2M_Pos);
    TIM3->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM3->CR1 = TIM_CR1_ARPE;
    TIM3->CR2 = TIM_CR2_MMS_1;  // Генерация сигнала триггера при обновлении
    TIM3->PSC = 0;
    TIM3->ARR = 7999;
    TIM3->CR1 |= TIM_CR1_CEN;
}

// Настройка TIM14 для расчета скорости вращения вентиляторов
void tim14Init(){
    TIM14->PSC = 24;
    TIM14->CR1 |= TIM_CR1_CEN;
}

/////////////////////////////////////////////////////////////////////

void sysInit(){
    xdev_out(ssd1306_Char);
    rccInit();
    gpioInit();
    uartInit();
    spiInit();
    adcInit();
    extiInit();
    tim3Init();
    tim14Init();
    ssd1306_Init();
    NVIC_SetPriority(SysTick_IRQn, 3);
    SysTick_Config(F_CPU/100);
}