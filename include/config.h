#pragma once

#define SN   0x11223344
#define VP   0x00000006

//#define AIR600W
//#define AIR1500W
//#define AIR3000W

#ifdef AIR600W
    #define TYPE 0x00000600

    #define minFanSpeedRPM2_DEF 0
    #define fanSpeedRPM2_DEF 0
#endif

#ifdef AIR1500W
    #define TYPE 0x00001500

    #define minFanSpeedRPM2_DEF 2000
    #define fanSpeedRPM2_DEF 2800
#endif

#ifdef AIR3000W
    #define TYPE 0x00003000
    
    #define minFanSpeedRPM2_DEF 2000
    #define fanSpeedRPM2_DEF 2800
#endif
