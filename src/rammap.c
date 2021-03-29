#include "rammap.h"
#include "system.h"
#include "flash.h"

coreInfo_t coreInfo = {VP, TYPE};

coreSetting_t coreSetting = {
    .tCool = tCool_DEF,
    .tHeat = tHeat_DEF,
    .deltaTCool = deltaTCool_DEF,
    .deltaTHeat = deltaTHeat_DEF,
    .alrmT1min = alrmT1min_DEF,
    .alrmT1max = alrmT1max_DEF,
    .alrmT2min = alrmT2min_DEF,
    .alrmT2max = alrmT2max_DEF,
    .compressorCurrentMin = compressorCurrentMin_DEF,
    .compressorCurrentMax = compressorCurrentMax_DEF,
    .heaterCurrentMin = heaterCurrentMin_DEF,
    .heaterCurrentMax = heaterCurrentMax_DEF,
    .alrmVmin = alrmVmin_DEF,
    .alrmVmax = alrmVmax_DEF,
    .minFanSpeedRPM1 = minFanSpeedRPM1_DEF,
    .fanSpeedRPM1 = fanSpeedRPM1_DEF,
    .minFanSpeedRPM2 = minFanSpeedRPM2_DEF,
    .fanSpeedRPM2 = fanSpeedRPM2_DEF,
    .invRel = invRel_DEF,
    .invAlm2 = invAlm2_DEF,
    .invAlm1 = invAlm1_DEF,
    .coolNumber = coolNumber_DEF,
    .coolOffAv = coolOffAv_DEF,
    .heatOffAv = heatOffAv_DEF,
    .fun1OffAv = fun1OffAv_DEF,
    .fun2OffAv = fun2OffAv_DEF,
    .fun1OffFC = fun1OffFC_DEF,
    .fun2OffFC = fun2OffFC_DEF,
    .flags3 = 0,
    .flags4 = 0,
    .calidT1 = calidT1_DEF,
    .calidT2 = calidT2_DEF,
    .calidOffsetCurrent = calidOffsetCurrent_DEF,
    .calidSlopeCurrent = calidSlopeCurrent_DEF,
    .calidOffsetVoltage = calidOffsetVoltage_DEF,
    .calidSlopeVoltage = calidSlopVoltage_DEF
};

coreStatus_t coreStatus;
coreDebug_t coreDebug;

#define inRange(val, min, max) (((val)>=(min)) && ((val)<=(max)))

uint8_t checkSettingParam(coreSetting_t* st){
    if(!inRange(st->tCool, tCool_MIN, tCool_MAX)) return 1;
    if(!inRange(st->tHeat, tHeat_MIN, tHeat_MAX)) return 2;
    if(!inRange(st->deltaTCool, deltaTCool_MIN, deltaTCool_MAX)) return 3;
    if(!inRange(st->deltaTHeat, deltaTHeat_MIN, deltaTHeat_MAX)) return 4;
    if(!inRange(st->alrmT1min, alrmT1min_MIN, alrmT1min_MAX)) return 5;
    if(!inRange(st->alrmT1max, alrmT1max_MIN, alrmT1max_MAX)) return 6;
    if(!inRange(st->alrmT2min, alrmT2min_MIN, alrmT2min_MAX)) return 7;
    if(!inRange(st->alrmT2max, alrmT2max_MIN, alrmT2max_MAX)) return 8;
    if(!inRange(st->compressorCurrentMin, compressorCurrentMin_MIN, compressorCurrentMin_MAX)) return 9;
    if(!inRange(st->compressorCurrentMax, compressorCurrentMax_MIN, compressorCurrentMax_MAX)) return 10;
    if(!inRange(st->heaterCurrentMin, heaterCurrentMin_MIN, heaterCurrentMin_MAX)) return 11;
    if(!inRange(st->heaterCurrentMax, heaterCurrentMax_MIN, heaterCurrentMax_MAX)) return 12;
    if(!inRange(st->alrmVmin, alrmVmin_MIN, alrmVmin_MAX)) return 13;
    if(!inRange(st->alrmVmax, alrmVmax_MIN, alrmVmax_MAX)) return 14;
    if(!inRange(st->minFanSpeedRPM1, minFanSpeedRPM1_MIN, minFanSpeedRPM1_MAX)) return 15;
    if(!inRange(st->fanSpeedRPM1, fanSpeedRPM1_MIN, fanSpeedRPM1_MAX)) return 16;
    if(!inRange(st->minFanSpeedRPM2, minFanSpeedRPM2_MIN, minFanSpeedRPM2_MAX)) return 17;
    if(!inRange(st->fanSpeedRPM2, fanSpeedRPM2_MIN, fanSpeedRPM2_MAX)) return 18;
    if(st->typePin & typePin_MASK) return 19;
    if(!(((st->tHeat+st->deltaTHeat)+100)<(st->tCool-st->deltaTCool))) return 0x80;
    if(st->fanSpeedRPM1 < st->minFanSpeedRPM1) return 0x80+1;
    if(st->fanSpeedRPM2 < st->minFanSpeedRPM2) return 0x80+2;
    return 0;
}

void loadFlashSetting(){
    if(*(uint16_t*)(0x08007C00) == 0x55AA
    && *(uint16_t*)(0x08007FFC) == 0x55AA
    && *(uint16_t*)(0x08007C02) == *(uint16_t*)(0x08007FFE)
    && *(uint32_t*)(0x08007FF4) == TYPE
    && *(uint32_t*)(0x08007FF8) == VP){
        CRC->CR=CRC_CR_RESET;
        for(uint8_t i=0; i<(sizeof(coreSetting_t)+1)/2; i++) *(uint16_t *)(&(CRC->DR))=((uint16_t*)(0x08007C04))[i];
        if(CRC->DR == *(uint32_t*)(0x08007FF0))
        for(uint8_t i=0; i<(sizeof(coreSetting_t)+1)/2; i++) ((uint16_t*)&coreSetting)[i] = ((uint16_t*)(0x08007C04))[i];
    }
}

void saveFlashSetting(coreSetting_t* st){
    for(uint8_t i=0; i<(sizeof(coreSetting_t)+1)/2; i++)
        ((uint16_t*)&coreSetting)[i] = ((uint16_t*)(st))[i];
    flashInit();
    uint16_t tmp = *(uint16_t*)(0x08007C02)+1;
    flashSectorClear(0x08007C00);
    flashWrite(0x08007C00, 0x55AA);
    flashWrite(0x08007C02, tmp);
    CRC->CR=CRC_CR_RESET;
    for(uint8_t i=0; i<(sizeof(coreSetting_t)+1)/2; i++){
        flashWrite(0x08007C04+i*2, ((uint16_t*)&coreSetting)[i]);
        *(uint16_t *)(&(CRC->DR))=((uint16_t*)&coreSetting)[i];
    }
    flashWrite(0x08007FF0, CRC->DR);
    flashWrite(0x08007FF2, CRC->DR>>16);
    flashWrite(0x08007FF4, TYPE);
    flashWrite(0x08007FF6, TYPE>>16);
    flashWrite(0x08007FF8, VP);
    flashWrite(0x08007FFA, VP>>16);
    flashWrite(0x08007FFC, 0x55AA);
    flashWrite(0x08007FFE, tmp);
}