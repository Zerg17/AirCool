#include "sysControl.h"
#include "system.h"
#include "proc.h"
#include "rammap.h"
#include "tool.h"

uint32_t volatile tick=0;
uint32_t volatile sec_d=0;
uint32_t volatile sec=0;

volatile uint16_t rpm1=0, rpm2=0;
uint16_t rpm1S=0, rpm2S=0;

int32_t term1, term2;
uint16_t voltage, current;

uint16_t adcR[4];
uint16_t adcF[4];

void SysTick_Handler(void) {
    tick++;
    if(++sec_d==100){
        sec_d=0;
        sec++;
    }
    //if(tick == 0xFFFFFFFF) NVIC_SystemReset();

    /////////////////////////////////////////////////////////////////

    static uint32_t coolTim=0;
    if(coolTim)coolTim--;

    /////////////////////////////////////////////////////////////////

    if(rpm1)rpm1--;
    if(rpm2)rpm2--;

    static int32_t I1, I2;
    I1=constrain(I1+(int16_t)rpm1S-(int16_t)rpm1, 2400*250, 7999*250);
    I2=constrain(I2+(int16_t)rpm2S-(int16_t)rpm2, 2400*250, 7999*250);
    TIM3->CCR1 = rpm1S?constrain(I1/250, 2400, 7999):0;
    TIM3->CCR2 = rpm2S?constrain(I2/250, 2400, 7999):0;

    /////////////////////////////////////////////////////////////////

    term1 = tempCalc(adcF[0])*100/65535;
    term2 = tempCalc(adcF[1])*100/65535;
    voltage = adcF[2]*3377/2000+80;
    current = adcF[3]*5283/20000;

    /////////////////////////////////////////////////////////////////

    coreStatus.time = sec;
    coreStatus.temp1 = term1;
    coreStatus.temp2 = term2;
    coreStatus.rpmFan1 = rpm1;
    coreStatus.rpmFan2 = rpm2;
    coreStatus.voltage = voltage;
    coreStatus.current = current;

    /////////////////////////////////////////////////////////////////

    logicProc();

    /////////////////////////////////////////////////////////////////

    if(coreStatus.mode == waitStartMode || coreStatus.mode == waitMode || coreStatus.mode == perStartMode || coreStatus.mode == offMode || coreStatus.mode == errMode){
        rpm1S=0;
        rpm2S=0;
        HEAT_OFF;
        COOL_OFF;
        coreStatus.coolOn=0;
        coreStatus.heatOn=0;
        coolTim=60*100;
    }

    /////////////////////////////////////////////////////////////////

    static uint32_t errFun1Count=0;
    if(coreStatus.mode == coolMode || coreStatus.mode == testFun1Mode || coreStatus.mode == testCoolMode){
        rpm1=coreSetting.fanSpeedRPM1;

        if(abs((int16_t)rpm1S-(int16_t)rpm1)>200)errFun1Count++;
        else if(errFun1Count) errFun1Count--;

        if(errFun1Count>30*100){
            errFun1Count=0;
            coreStatus.mode=errMode;
            coreStatus.errFun1=1;
        }
    }

    /////////////////////////////////////////////////////////////////
    
    static uint32_t errCoolCount=0;
    if(coolTim==0 && coreStatus.errHighCurrentCool==0 && coreStatus.errHighCurrentCool==0 && (coreStatus.mode == coolMode || coreStatus.mode == testCoolMode)){
        COOL_ON;
        coreStatus.coolOn=1;

        if(current<coreSetting.compressorCurrentMin || current>coreSetting.compressorCurrentMax)errCoolCount++;
        else if(errCoolCount) errCoolCount--;

        if(errCoolCount>5*100){
            errCoolCount=0;
            COOL_OFF;
            coreStatus.coolOn=0;
            coreStatus.errHighCurrentCool=current>coreSetting.compressorCurrentMax;
            coreStatus.errLowCurrentCool=current<coreSetting.compressorCurrentMin;
        }
    }

    /////////////////////////////////////////////////////////////////

    static uint32_t errHeatCount=0;
    if(coreStatus.errHighCurrentHeat==0 && coreStatus.errLowCurrentHeat==0 && (coreStatus.mode == heatMode || coreStatus.mode == testHeatMode)){
        HEAT_ON;
        coreStatus.heatOn=1;

        if(current<coreSetting.heaterCurrentMin || current>coreSetting.heaterCurrentMax)errHeatCount++;
        else if(errHeatCount) errHeatCount--;

        if(errHeatCount>20*100){
            errHeatCount=0;
            HEAT_OFF;
            coreStatus.heatOn=0;
            coreStatus.errHighCurrentHeat=current>coreSetting.heaterCurrentMax;
            coreStatus.errLowCurrentHeat=current<coreSetting.heaterCurrentMin;
        }
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
    static const int32_t filterK[] = {1024, 1024, 512, 2048};
    static uint32_t acc[4] = {2048*512, 2048*512, 2800*512};
    if(DMA1->ISR & DMA_ISR_TCIF1){
        DMA1->IFCR=DMA_IFCR_CTCIF1;
        for(uint8_t i=0; i<4; i++){
            acc[i]+=adcR[i]-adcF[i];
            adcF[i]=acc[i]/filterK[i];
        }
    }
}