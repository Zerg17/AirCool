#include "proc.h"
#include "rammap.h"
#include "system.h"
#include "tool.h"
#include "uart.h"

volatile uint8_t msgResponse=0, msgResponseType=0;
uint16_t rpm1S=0, rpm2S=0;

void logicProc(){
    coreStatus.time=sec;
    static uint16_t countCur=0;

    if(voltage<coreSetting.alrmVmin) coreStatus.lowVoltage=1;
    if(voltage-100>coreSetting.alrmVmin) coreStatus.lowVoltage=0;
    if(voltage>coreSetting.alrmVmax) coreStatus.highVoltage=1;
    if(voltage+100<coreSetting.alrmVmax) coreStatus.highVoltage=0;
    coreStatus.errTmp1=term1<-5000;
    coreStatus.errTmp1=term2<-5000;
    if(coreStatus.heatOn && current>coreSetting.heaterCurrentMax){
        countCur+=2;
        if(countCur>500)coreStatus.highCurrentHeat=1;
    }
    if(coreStatus.heatOn && current<coreSetting.heaterCurrentMin){
        countCur+=2;
        if(countCur>2000)coreStatus.lowCurrentHeat=1;
    }
    if(coreStatus.coolOn && current>coreSetting.compressorCurrentMax){
        countCur+=2;
        if(countCur>500)coreStatus.highCurrentCool=1;
    }
    if(coreStatus.coolOn && current<coreSetting.compressorCurrentMin){
        countCur+=2;
        if(countCur>2000)coreStatus.lowCurrentCool=1;
    }
    if(countCur)countCur--;

    coreStatus.alm1 = GPIOA->IDR & GPIO_IDR_5;
    coreStatus.alm2 = GPIOA->IDR & GPIO_IDR_8;


    if(!((coreStatus.status & CRITICAL_ERR) | coreStatus.alm1 | coreStatus.alm2)){
        if(sec==305 && sec_d==0){
            coreStatus.status = 0;
            coreStatus.mode = testMode;
            rpm1S=coreSetting.minFanSpeedRPM1;
            COOL_OFF;
            HEAT_OFF;
            coreStatus.coolOn = 0;
            coreStatus.heatOn = 0;
        }

        if(sec==365 && sec_d==0){
            if(abs((int16_t)rpm1S-(int16_t)rpm1)>200){
                coreStatus.errFun1 = 1;
            }
            rpm1S=coreSetting.fanSpeedRPM1;
        }

        if(sec==425 && sec_d==0){
            if(abs((int16_t)rpm1S-(int16_t)rpm1)>200){
               coreStatus.errFun1 = 1;
            }
            rpm1S=coreSetting.minFanSpeedRPM1;
            HEAT_ON;
            coreStatus.heatOn = 1;
        }

        if(sec==545 && sec_d==0){
            HEAT_OFF;
            coreStatus.heatOn = 0;
            COOL_ON;
            coreStatus.coolOn = 1;
        }

        if(sec==725 && sec_d==0){
            COOL_OFF;
            coreStatus.coolOn = 0;
            coreStatus.mode = waitMode;
        }

        if(sec>785){

            rpm1S=coreStatus.mode == coolMode?coreSetting.fanSpeedRPM1:coreSetting.minFanSpeedRPM1;

            if(coreStatus.mode==waitMode && term1>coreSetting.tCool){
                COOL_ON;
                coreStatus.mode = coolMode;
                coreStatus.coolOn = 1;
            }

            if(coreStatus.mode==coolMode && term1<coreSetting.tCool-coreSetting.deltaTCool){
                COOL_OFF;
                coreStatus.mode = waitMode;
                coreStatus.coolOn = 0;
            }

            if(coreStatus.mode==waitMode && term1<coreSetting.tHeat){
                HEAT_ON;
                coreStatus.mode = heatMode;
                coreStatus.heatOn = 1;
            }

            if(coreStatus.mode==heatMode && term1>coreSetting.tHeat+coreSetting.deltaTHeat){
                HEAT_OFF;
                coreStatus.mode = waitMode;
                coreStatus.heatOn = 0;
            }
        }

        ALARM_OFF;
    }else{
        //xfprintf(uartWrite, "err %02X\n", coreStatus.status);
        coreStatus.mode = waitMode;
        rpm1S=0;
        rpm2S=0;
        HEAT_OFF;
        COOL_OFF;
        ALARM_ON;
    }


    if(msgFlug){
        msgFlug=0;
        msgResponseType=msgType;
        switch(msgType){
            case 0:
                msgResponse=1;
                break;
            case 1:
                msgResponse=1;
                break;
            case 2:
                msgResponseType=msgLen?0x82:2;
                msgResponse=1;
                break;
            case 0x72:
                msgResponse=1;
                sec=300;
                break;
            case 0x70:
                msgResponse=1;
                coreStatus.off=1;
                break;
            case 0x71:
                msgResponse=1;
                coreStatus.off=0;
                break;
            case 0x7F:
                NVIC_SystemReset();
                break;
        }
    }
}
