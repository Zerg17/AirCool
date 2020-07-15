#include "proc.h"
#include "rammap.h"
#include "system.h"
#include "tool.h"

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


    if(!(coreStatus.status & CRITICAL_ERR)){
        if(sec==305 && sec_d==0){
            coreStatus.mode = testMode;
            rpm1S=coreSetting.minFanSpeedRPM1;
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

        if(sec>725){

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

    // if(tick%50 == 0){
    //     //xfprintf(uartWrite, "%d %d %u %u %u %X\n", term1, term2, voltage, current, sec, err);
    //     xfprintf(uartWrite, "%u\n", current);
    // }

    // if(msgFlug){
    //     if(msgType<3 && msgLen==0)msgResponse=1;
    //     msgFlug=0;
    // }
}
