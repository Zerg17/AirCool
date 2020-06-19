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
    minFanSpeedRMP_DEF,
    fanSpeedRMP_DEF,
    numPulsesFun_DEF,
    minFanSpeedRMP_DEF,
    fanSpeedRMP_DEF,
    numPulsesFun_DEF,
    coolNumber_DEF,
    typePin_DEF
};
coreStatus_t coreStatus;