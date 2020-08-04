#include "proc.h"
#include "rammap.h"
#include "system.h"
#include "uart.h"
#include "sysControl.h"

uint32_t timWaitNext=0;

void logicProc(){
    static uint8_t curStat=0xFF;

    if(curStat != coreStatus.mode){
        switch(coreStatus.mode){
            case waitStartMode: timWaitNext=5*60*100+5*100; break;
            case testFun1Mode: timWaitNext=2*60*100; break;
            case testFun2Mode: timWaitNext=2*60*100; break;
            case testHeatMode: timWaitNext=2*60*100; break;
            case testCoolMode: timWaitNext=3*60*100; break;
            case perStartMode: timWaitNext=30*100; break;
        }
        curStat = coreStatus.mode;
    }

    if(timWaitNext) timWaitNext--;
    else{
        switch(coreStatus.mode){
            case waitStartMode: coreStatus.mode=testFun1Mode; break;
            case testFun1Mode: coreStatus.mode=testHeatMode; break;
            case testHeatMode: coreStatus.mode=testCoolMode; break;
            case testCoolMode: coreStatus.mode=perStartMode; break;
            case perStartMode: coreStatus.mode=waitMode; break;
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