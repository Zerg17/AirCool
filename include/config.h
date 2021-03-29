#pragma once

#define VP   0x00000007

//#define AIR600W
//#define AIR1500W
//#define AIR3000W

#ifdef AIR600W
    #define TYPE 0x00000600
    
    #define minFanSpeedRPM1_DEF 2000
    #define fanSpeedRPM1_DEF 2800
    #define minFanSpeedRPM2_DEF 2000
    #define fanSpeedRPM2_DEF 2800
    #define fun1OffAv_DEF 1
    #define fun2OffAv_DEF 0
    #define fun1OffFC_DEF 1
    #define fun2OffFC_DEF 0
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
#endif
