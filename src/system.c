#include "system.h"
#include "ssd1306.h"
#include "uart.h"
#include "proc.h"
#include "tool.h"
#include "rammap.h"

uint32_t volatile tick=0;
uint32_t volatile sec_d=0;
uint32_t volatile sec=0;
volatile uint16_t rpm1=0, rpm2=0;
int32_t term1, term2;
uint16_t voltage, current;

void SysTick_Handler(void) {
    tick++;
    if(++sec_d==100){
        sec_d=0;
        sec++;
    }

    if(rpm1)rpm1--;
    if(rpm2)rpm2--;

    static int32_t I1, I2;
    I1=constrain(I1+(int16_t)rpm1S-(int16_t)rpm1, 2400*250, 7999*250);
    I2=constrain(I2+(int16_t)rpm2S-(int16_t)rpm2, 2400*250, 7999*250);
    TIM3->CCR1 = rpm1S?constrain(I1/250, 2400, 7999):0;
    TIM3->CCR2 = rpm2S?constrain(I2/250, 2400, 7999):0;

    term1 = tempCalc(adcF[0])*100/65535;
    term2 = tempCalc(adcF[1])*100/65535;
    voltage = adcF[2]*3377/2000+80;
    current = adcF[3]*5283/20000;

    coreStatus.time = sec;
    coreStatus.temp1 = term1;
    coreStatus.temp2 = term2;
    coreStatus.rpmFan1 = rpm1;
    coreStatus.rpmFan2 = rpm2;
    coreStatus.voltage = voltage;
    coreStatus.current = current;

    // if(tick%100==0){
    //     xfprintf(uartWrite, "%u %d %d %u %u %02X\n", sec, term1, term2, voltage, current, coreStatus.status);
    // }

    // if(tick%100==0){
    //     msgResponseType=1;
    //     msgResponse=1;
    // }

    logicProc();
}

void rccInit(){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOFEN | RCC_AHBENR_DMAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_ADCEN | RCC_APB2ENR_SPI1EN | RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM14EN;
}

void gpioInit(){
    // PA15 - SERIAL_RX     - USART1_RX
    // PA12 - THO_OUT       - EXTI4_15
    // PA11 - SSD1306_DC    - GPIO_OUT
    // PA9  - SERIAL_TX     - USART1_TX
    // PA8  - ALM2          - GPIO_IN
    // PA7  - OU_EXT        - TIM3_CH2
    // PA6  - OU_INT        - TIM3_CH1
    // PA5  - ALM1          - GPIO_IN
    // PA4  - SSD1306_CS    - GPIO_OUT
    // PA3  - CURRENT       - ADC3
    // PA2  - VOLTAGE       - ADC2
    // PA1  - TEMP2         - ADC1
    // PA0  - TEMP1         - ADC0
    // PB7  - ALARM         - GPIO_OUT
    // PB5  - SSD1306_SDA   - SPI
    // PB4  - THO_IN        - EXTI4_15
    // PB3  - SSD1306_SCK   - SPI
    // PB1  - COOL          - GPIO_OUT
    // PB0  - HEAT          - GPIO_OUT
    // PF0  - SSD1306_RESET - GPIO_OUT

    // 0 - GPIO_OUT
    // 1 - Alternate function
    // Msk - Analog mode

    GPIOA->MODER |= GPIO_MODER_MODER15_1 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER9_1 | GPIO_MODER_MODER7_1 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER3_Msk | GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER1_Msk | GPIO_MODER_MODER0_Msk;
    GPIOB->MODER |= GPIO_MODER_MODER7_0 | GPIO_MODER_MODER5_1 | GPIO_MODER_MODER3_1 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER0_0;
    GPIOF->MODER |= GPIO_MODER_MODER0_0;
    GPIOA->AFR[1]|= 0x10000010;
    GPIOA->AFR[0]|= 0x11000000;
}

/////////////////////////////////////////////////////////////////////

void extiInit(){
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PB;
    EXTI->FTSR = (1<<4) | (1<<12);
    EXTI->IMR =  (1<<4) | (1<<12);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void EXTI4_15_IRQHandler(){
    uint16_t t = TIM14->CNT;
    static uint16_t lt1, lt2;

    if((EXTI->PR & EXTI_PR_PR4) == EXTI_PR_PR4){
        EXTI->PR |= EXTI_PR_PR4;
        uint16_t dt=t-lt1;
        if(dt>2400U){
            for(volatile uint16_t i=0; i<400; i++);
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

/////////////////////////////////////////////////////////////////////

void spiInit(void){
    SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_MSTR;
    SPI1->CR1 |= SPI_CR1_SPE;
}

/////////////////////////////////////////////////////////////////////

uint16_t adcR[4];
uint16_t adcF[4];
int32_t filterK[] = {1024, 1024, 512, 2048};

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

    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void DMA1_Channel1_IRQHandler(void){
    static uint32_t acc[4] = {2048*512, 2048*512, 2800*512};
    if(DMA1->ISR & DMA_ISR_TCIF1){
        DMA1->IFCR=DMA_IFCR_CTCIF1;
        for(uint8_t i=0; i<4; i++){
            acc[i]+=adcR[i]-adcF[i];
            adcF[i]=acc[i]/filterK[i];
        }
    }
}

/////////////////////////////////////////////////////////////////////

void tim3Init(){
    TIM3->CCMR1 = (0x06 << TIM_CCMR1_OC1M_Pos) | (0x06 << TIM_CCMR1_OC2M_Pos);
    TIM3->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM3->CR1 = TIM_CR1_ARPE;
    TIM3->CR2 = TIM_CR2_MMS_1;
    TIM3->PSC = 0;
    TIM3->ARR = 7999;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void tim14Init(){
    TIM14->PSC = 24;
    TIM14->CR1 |= TIM_CR1_CEN;
}

void sysInit(){
    rccInit();
    gpioInit();
    uartInit();
    spiInit();
    adcInit();
    extiInit();
    tim3Init();
    tim14Init();
    ssd1306_Init();
    SysTick_Config(F_CPU/100);
}