#include "sysControl.h"
#include "system.h"
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

uint32_t timWaitNext=0;

void logicProc(){
    static uint8_t curStat=0xFF;
    static uint8_t testComplite=0;

    if(curStat != coreStatus.mode){
        switch(coreStatus.mode){
            case waitStartMode: timWaitNext=5*60*100+5*100; break;
            //case waitStartMode: timWaitNext=5*100; break;
            case watiTestMode: timWaitNext=5*100; break;
            case testFun1Mode: timWaitNext=2*60*100; break;
            case testFun2Mode: timWaitNext=2*60*100; break;
            case testHeatMode: timWaitNext=2*60*100; break;
            case testCoolMode: timWaitNext=3*60*100; break;
            case befStartMode: timWaitNext=30*100; break;
            case afterErrMode: 
                if(testComplite){
                    coreStatus.mode = befStartMode;
                    timWaitNext=30*100;
                }else{
                    coreStatus.mode = waitStartMode;
                    timWaitNext=5*60*100;
                    //timWaitNext=100;
                }
                break;
        }
        curStat = coreStatus.mode;
    }

    if(timWaitNext) timWaitNext--;
    else{
        switch(coreStatus.mode){
            case waitStartMode: coreStatus.mode=watiTestMode; break;
            case watiTestMode: coreStatus.mode=testFun1Mode; break;
            case testFun1Mode: coreStatus.mode=testFun2Mode; break;
            case testFun2Mode: coreStatus.mode=testHeatMode; break;
            case testHeatMode: coreStatus.mode=testCoolMode; break;
            case testCoolMode: coreStatus.mode=befStartMode; testComplite=1; break;
            case befStartMode: coreStatus.mode=waitMode; break;
        }
    }

    if(coreStatus.mode == waitMode){
        if(term1>coreSetting.tCool)coreStatus.mode=coolMode;
        if(term1<coreSetting.tHeat)coreStatus.mode=heatMode;
    }

    if(coreStatus.mode == coolMode){
        if(term1<(coreSetting.tCool-coreSetting.deltaTCool))coreStatus.mode=waitMode;
    }

    if(coreStatus.mode == heatMode){
        if(term1>(coreSetting.tCool+coreSetting.deltaTCool))coreStatus.mode=waitMode;
    }
}

void SysTick_Handler(void) {

    tick++;
    if(++sec_d==100){
        sec_d=0;
        sec++;
    }
    if(tick == 0xFFFFFFFF) NVIC_SystemReset();    

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
    // Обработка аварий

    coreStatus.errHighVoltage = voltage>coreSetting.alrmVmax;
    coreStatus.errLowVoltage = voltage<coreSetting.alrmVmin;
    coreStatus.errT1max = term1>coreSetting.alrmT1max;
    coreStatus.errT1min = term1<coreSetting.alrmT1min;
    coreStatus.errT2max = term2>coreSetting.alrmT2max;
    coreStatus.errT2min = term2<coreSetting.alrmT2min;
    coreStatus.errTmp1 = adcF[0]>4076;
    coreStatus.errTmp2 = adcF[1]>4076;
    coreStatus.alm1 = !ALM1_GET;
    coreStatus.alm2 = !ALM2_GET;

    /////////////////////////////////////////////////////////////////
    // Реакция на аварии

    if(    coreStatus.errHighVoltage     || coreStatus.errLowVoltage 
        || coreStatus.errFun1            || coreStatus.errFun2 
        || coreStatus.errHighCurrentCool || coreStatus.errLowCurrentCool
        || coreStatus.errHighCurrentHeat || coreStatus.errLowCurrentHeat
        || coreStatus.errTmp1            || coreStatus.errTmp2
        || coreStatus.alm1               || coreStatus.alm2){
        coreStatus.mode=errMode;
        ALARM_ON;
    }else{
        if(coreStatus.mode==errMode)coreStatus.mode=afterErrMode;
        ALARM_OFF;
    }

    /////////////////////////////////////////////////////////////////
    // Обработка 1 вентилятора

    //static uint32_t heatTim=0;
    //if(coreStatus.mode == heatMode) heatTim=tick;
    static uint32_t errFun1Count=0;
    if(coreStatus.errFun1==0 && (coreStatus.mode == coolMode ||  coreStatus.mode == waitMode || coreStatus.mode == testFun1Mode || coreStatus.mode == testCoolMode)){
        
        if(coreStatus.mode == coolMode || coreStatus.mode == testCoolMode) rpm1S=coreSetting.fanSpeedRPM1;
        else rpm1S=coreSetting.minFanSpeedRPM1;
        //if((10*60*100+heatTim)>tick)rpm1S=0;
        //if(sec%140<20)rpm1S=0;
        //if(coreStatus.mode == waitMode && term2<500) rpm1S=0;
        
        if(abs((int16_t)rpm1S-(int16_t)rpm1)>200)errFun1Count++;
        else if(errFun1Count) errFun1Count--;

        if(errFun1Count>30*100){
            errFun1Count=0;   
            coreStatus.errFun1=1;
        }
    }else{
        rpm1S=0;
    }

    /////////////////////////////////////////////////////////////////
    // Обработка 2 вентилятора

    static uint32_t errFun2Count=0;
    if(coreStatus.errFun2==0 && (coreStatus.mode == coolMode || coreStatus.mode == heatMode ||  coreStatus.mode == waitMode || coreStatus.mode == testFun2Mode || coreStatus.mode == testCoolMode || coreStatus.mode == testHeatMode)){

        if(coreStatus.mode == coolMode || coreStatus.mode == testCoolMode) rpm2S=coreSetting.fanSpeedRPM2;
        else rpm2S=coreSetting.minFanSpeedRPM2;

        if(abs((int16_t)rpm2S-(int16_t)rpm2)>200)errFun2Count++;
        else if(errFun2Count) errFun2Count--;

        if(errFun2Count>30*100){
            errFun2Count=0;   
            coreStatus.errFun2=1;
        }
    }else{
        rpm2S=0;
    }


    /////////////////////////////////////////////////////////////////
    // Обработка компрессора
    
    static uint32_t coolTim=0;
    if(coolTim)coolTim--;
    static uint32_t errCoolCount=0;
    if(coolTim==0 && coreStatus.errLowCurrentCool==0 && coreStatus.errHighCurrentCool==0 && (coreStatus.mode == coolMode || coreStatus.mode == testCoolMode)){
        COOL_ON;
        coreStatus.coolOn=1;

        if(current<coreSetting.compressorCurrentMin || current>coreSetting.compressorCurrentMax)errCoolCount++;
        else if(errCoolCount) errCoolCount--;

        if(coreStatus.mode == testCoolMode && errCoolCount>5*100){
            errCoolCount=0;
            COOL_OFF;
            coreStatus.coolOn=0;
            coreStatus.errHighCurrentCool=current>coreSetting.compressorCurrentMax;
            coreStatus.errLowCurrentCool=current<coreSetting.compressorCurrentMin;
        }
    }else{
        COOL_OFF;
        if(coolTim==0 && coreStatus.coolOn) coolTim=60*100;
        coreStatus.coolOn=0;
        
    }

    /////////////////////////////////////////////////////////////////
    // Обработка нагревателя

    static uint32_t errHeatCount=0;
    if(coreStatus.errLowCurrentHeat==0 && coreStatus.errHighCurrentHeat==0 && (coreStatus.mode == heatMode || coreStatus.mode == testHeatMode)){
        HEAT_ON;
        coreStatus.heatOn=1;

        if(current<coreSetting.heaterCurrentMin || current>coreSetting.heaterCurrentMax)errHeatCount++;
        else if(errHeatCount) errHeatCount--;

        if(coreStatus.mode == testHeatMode && errHeatCount>20*100){
            errHeatCount=0;
            HEAT_OFF;
            coreStatus.heatOn=0;
            coreStatus.errHighCurrentHeat=current>coreSetting.heaterCurrentMax;
            coreStatus.errLowCurrentHeat=current<coreSetting.heaterCurrentMin;
        }
    }else{
        HEAT_OFF;
        coreStatus.heatOn=0;
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
    static uint32_t acc[4] = {2048*1024, 2048*1024, 2800*512};
    if(DMA1->ISR & DMA_ISR_TCIF1){
        DMA1->IFCR=DMA_IFCR_CTCIF1;
        for(uint8_t i=0; i<4; i++){
            acc[i]+=adcR[i]-adcF[i];
            adcF[i]=acc[i]/filterK[i];
        }
    }
}