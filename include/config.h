#pragma once

#define VP   0x00000009

//#define AIR600W
//#define AIR1500W
//#define AIR3000W
#define AIR_UNIVERSAL

#ifdef AIR_UNIVERSAL
    #define TYPE 0x00000000
    
    #define minFanSpeedRPM1_DEF 2000
    #define fanSpeedRPM1_DEF 3200    
    #define minFanSpeedRPM2_DEF 2000
    #define fanSpeedRPM2_DEF 3200
    #define fun1OffAv_DEF 0
    #define fun2OffAv_DEF 0
    #define fun1OffFC_DEF 0
    #define fun2OffFC_DEF 0
    #define compressorCurrentMin_DEF 10
    #define compressorCurrentMax_DEF 900
    #define heaterCurrentMin_DEF 100
    #define heaterCurrentMax_DEF 900
#endif

#ifdef AIR600W
    #define TYPE 0x00000600
    
    #define minFanSpeedRPM1_DEF 2000
    #define fanSpeedRPM1_DEF 2800
    #define minFanSpeedRPM2_DEF 2000
    #define fanSpeedRPM2_DEF 2800
    #define fun1OffAv_DEF 0
    #define fun2OffAv_DEF 1
    #define fun1OffFC_DEF 0
    #define fun2OffFC_DEF 1
    #define compressorCurrentMin_DEF 10
    #define compressorCurrentMax_DEF 1000
    #define heaterCurrentMin_DEF 100
    #define heaterCurrentMax_DEF 1000
#endif

#ifdef AIR1500W
    #define TYPE 0x00001500

    #define minFanSpeedRPM1_DEF 2000
    #define fanSpeedRPM1_DEF 2800
    #define minFanSpeedRPM2_DEF 2000
    #define fanSpeedRPM2_DEF 2800
    #define fun1OffAv_DEF 0
    #define fun2OffAv_DEF 0
    #define fun1OffFC_DEF 0
    #define fun2OffFC_DEF 0
    #define compressorCurrentMin_DEF 10
    #define compressorCurrentMax_DEF 1000
    #define heaterCurrentMin_DEF 100
    #define heaterCurrentMax_DEF 1000
#endif

#ifdef AIR3000W
    #define TYPE 0x00003000
    
    #define minFanSpeedRPM1_DEF 2000
    #define fanSpeedRPM1_DEF 2800    
    #define minFanSpeedRPM2_DEF 2000
    #define fanSpeedRPM2_DEF 2800
    #define fun1OffAv_DEF 0
    #define fun2OffAv_DEF 0
    #define fun1OffFC_DEF 0
    #define fun2OffFC_DEF 0
    #define compressorCurrentMin_DEF 10
    #define compressorCurrentMax_DEF 1000
    #define heaterCurrentMin_DEF 100
    #define heaterCurrentMax_DEF 1000
#endif
