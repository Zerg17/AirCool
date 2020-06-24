#include "rammap.h"
#include "config.h"

coreInfo_t coreInfo = {SN, VP, DATA};
coreSetting_t coreSetting = {
    tCool_DEF,
    tHeat_DEF,
    deltaTCool_DEF,
    deltaTHeat_DEF,
    calidT_DEF,
    calidT_DEF,
    0, 0, 0, 0, 0, 0,
    calidOffsetCurrent_DEF,
    0,
    calidoffsetVoltage_DEF,
    0,
    alrmTmin_DEF,
    alrmTmax_DEF,
    heaterCurrentMin_DEF,
    heaterCurrentMax_DEF,
    compressorCurrentMin_DEF,
    compressorCurrentMax_DEF,
    alrmVmin_DEF,
    alrmVmax_DEF,
    minFanSpeedRPM_DEF,
    fanSpeedRPM_DEF,
    numPulsesFun_DEF,
    minFanSpeedRPM_DEF,
    fanSpeedRPM_DEF,
    numPulsesFun_DEF,
    coolNumber_DEF,
    typePin_DEF
};
coreStatus_t coreStatus;

uint8_t checkSettingParam(coreSetting_t* st){
    if(st->tCool>tCool_MIN && st->tCool<tCool_MAX) return 1;
    if(st->tHeat>tHeat_MIN && st->tHeat<tHeat_MAX) return 2;
    if(st->deltaTCool>deltaTCool_MIN && st->deltaTCool<deltaTCool_MAX) return 3;
    if(st->deltaTHeat>deltaTHeat_MIN && st->deltaTHeat<deltaTHeat_MAX) return 4;
    if(st->calidT1>calidT_MIN && st->calidT1<calidT_MAX) return 4;
    if(st->calidT2>calidT_MIN && st->calidT2<calidT_MAX) return 5;
    if(st->calidOffsetCurrent>calidOffsetCurrent_MIN && st->calidOffsetCurrent<calidOffsetCurrent_MAX) return 13;
    if(st->calidoffsetVoltage>calidoffsetVoltage_MIN && st->calidoffsetVoltage<calidoffsetVoltage_MAX) return 15;
    if(st->alrmTmin>alrmTmin_MIN && st->alrmTmin<alrmTmin_MAX) return 17;
    if(st->alrmTmax>alrmTmax_MIN && st->alrmTmax<alrmTmax_MAX) return 18;
    if(st->heaterCurrentMin>heaterCurrentMin_MIN && st->heaterCurrentMin<heaterCurrentMin_MAX) return 19;
    if(st->heaterCurrentMax>heaterCurrentMax_MIN && st->heaterCurrentMax<heaterCurrentMax_MAX) return 20;
    if(st->compressorCurrentMin>compressorCurrentMin_MIN && st->compressorCurrentMin<compressorCurrentMin_MAX) return 21;
    if(st->compressorCurrentMax>compressorCurrentMax_MIN && st->compressorCurrentMax<compressorCurrentMax_MAX) return 22;
    if(st->alrmVmin>alrmVmin_MIN && st->alrmVmin<alrmVmin_MAX) return 23;
    if(st->alrmVmax>alrmVmax_MIN && st->alrmVmax<alrmVmax_MAX) return 24;
    if(st->minFanSpeedRPM1>minFanSpeedRPM_MIN && st->minFanSpeedRPM1<minFanSpeedRPM_MAX) return 25;
    if(st->fanSpeedRPM1>fanSpeedRPM_MIN && st->fanSpeedRPM1<fanSpeedRPM_MAX) return 26;
    if(st->numPulsesFun1<1) return 27;
    if(st->minFanSpeedRPM2>minFanSpeedRPM_MIN && st->minFanSpeedRPM2<minFanSpeedRPM_MAX) return 28;
    if(st->fanSpeedRPM2>fanSpeedRPM_MIN && st->fanSpeedRPM2<fanSpeedRPM_MAX) return 29;
    if(st->numPulsesFun2<1) return 30;
    if((st->tHeat+st->deltaTHeat)+1>(st->tCool-st->deltaTCool)) return 0x80;
    return 0;
}