#pragma once
#ifndef RAMMAP
#define RAMMAP

#include <stdint.h>
#include "config.h"

#define tCool_MIN 1000
#define tCool_DEF 3500
#define tCool_MAX 6000

#define tHeat_MIN -1000
#define tHeat_DEF 500
#define tHeat_MAX 3500


#define deltaTCool_MIN 100
#define deltaTCool_DEF 500
#define deltaTCool_MAX 2000

#define deltaTHeat_MIN 100
#define deltaTHeat_DEF 500
#define deltaTHeat_MAX 2000


#define alrmT1min_MIN -5000
#define alrmT1min_DEF 0
#define alrmT1min_MAX 3500

#define alrmT1max_MIN 1500
#define alrmT1max_DEF 4500
#define alrmT1max_MAX 7000

#define alrmT2min_MIN -5000
#define alrmT2min_DEF 0
#define alrmT2min_MAX 3500

#define alrmT2max_MIN 1500
#define alrmT2max_DEF 4500
#define alrmT2max_MAX 7000


#define compressorCurrentMin_MIN 0
#define compressorCurrentMin_DEF 10
#define compressorCurrentMin_MAX 1000

#define compressorCurrentMax_MIN 100
#define compressorCurrentMax_DEF 1000
#define compressorCurrentMax_MAX 1000


#define heaterCurrentMin_MIN 0
#define heaterCurrentMin_DEF 10
#define heaterCurrentMin_MAX 1000

#define heaterCurrentMax_DEF 1000
#define heaterCurrentMax_MIN 100
#define heaterCurrentMax_MAX 1000


#define alrmVmin_MIN 4000
#define alrmVmin_DEF 4000
#define alrmVmin_MAX 4800

#define alrmVmax_MIN 5000
#define alrmVmax_DEF 5700
#define alrmVmax_MAX 6300


#define minFanSpeedRPM1_MIN 1500
#define minFanSpeedRPM1_MAX 5000

#define fanSpeedRPM1_MIN 1500
#define fanSpeedRPM1_MAX 5000

#define minFanSpeedRPM2_MIN 1500
#define minFanSpeedRPM2_MAX 5000

#define fanSpeedRPM2_MIN 1500
#define fanSpeedRPM2_MAX 5000

#define invRel_DEF 0
#define invAlm2_DEF 1
#define invAlm1_DEF 1
#define typePin_MASK 0xFFF8

#define coolNumber_DEF 0

#define coolOffAv_DEF 0
#define heatOffAv_DEF 0

#define calidT1_DEF 0
#define calidT2_DEF 0

#define calidOffsetCurrent_DEF 0
#define calidSlopeCurrent_DEF 0
#define calidOffsetVoltage_DEF 0
#define calidSlopVoltage_DEF 0

enum coreMode{
    waitStartMode,
    waitMode,
    coolMode,
    heatMode,
    testFun1Mode,
    testFun2Mode,
    testHeatMode,
    testCoolMode,
    befStartMode,
    watiTestMode,
    manCtrMode,
    offMode=0xE,
    errMode=0xF
};

typedef struct{
    uint32_t vp;
    uint32_t type;
} coreInfo_t;

typedef struct coreSetting{
    int16_t tCool;                 // 1 Температура охлаждения
    int16_t tHeat;                 // 2 Температура нагрева

    int16_t deltaTCool;            // 3 Дельта температуры охлаждения
    int16_t deltaTHeat;            // 4 Дельта температуры нагрева

    int16_t alrmT1min;             // 5 Критически низкая температура
    int16_t alrmT1max;             // 6 Критически высокая температура
    int16_t alrmT2min;             // 7 Критически низкая температура
    int16_t alrmT2max;             // 8 Критически высокая температура

    uint16_t compressorCurrentMin; // 9 Минимальный ток компрессора
    uint16_t compressorCurrentMax; // 10 Максимальный ток компрессора

    uint16_t heaterCurrentMin;     // 11 Минимальный ток нагревателя
    uint16_t heaterCurrentMax;     // 12 Максимальный ток нагревателя

    uint16_t alrmVmin;             // 13 Минимальное напряжение
    uint16_t alrmVmax;             // 14 Максимальное напряжение

    uint16_t minFanSpeedRPM1;      // 15 Минимальные обороты 1 вентилятора
    uint16_t fanSpeedRPM1;         // 16 Номинальные обороты 1 вентилятора
    
    uint16_t minFanSpeedRPM2;      // 17 Минимальные обороты 2 вентилятора
    uint16_t fanSpeedRPM2;         // 18 Номинальные обороты 2 вентилятора
            
    union{  // 19 Тип входорв и выходов
        struct{
            uint16_t invRel:1;
            uint16_t invAlm2:1;
            uint16_t invAlm1:1;
        };
        uint16_t typePin;
    };


    uint16_t coolNumber;           // Номер кондиционера
    
    union{
        struct{
            uint16_t coolOffAv:1;    // Управление компрессором
            uint16_t heatOffAv:1;    // Управление нагревателем
            uint16_t fun1OffAv:1;    // Управление компрессором
            uint16_t fun2OffAv:1;    // Управление нагревателем
            uint16_t fun1OffFC:1;
            uint16_t fun2OffFC:1;
        };
        uint16_t flags1;
    };
    union{
        struct{
            uint16_t waitSkip:1;
            uint16_t testSkip:1;
        };
        uint16_t flags2;
    };
    uint16_t flags3;
    uint16_t flags4;

    int16_t calidT1;               // Калибровка смещения температуры 1
    int16_t calidT2;               // Калибровка смещения температуры 2

    int16_t calidOffsetCurrent;     // Калибровка тока
    int16_t calidSlopeCurrent;     // Калибровка тока
    int16_t calidOffsetVoltage;     // Калибровка напряжения
    int16_t calidSlopeVoltage;      // Калибровка напряжения
} coreSetting_t;

typedef struct {
    uint32_t time;                          // Вермя системы
    union{
        struct{
            uint32_t mode:4;                // Режим работы
            uint32_t opMode:2;              // Режим управления
            uint32_t coolOn:1;              // Компрессор включен
            uint32_t heatOn:1;              // Нагреватель включен
            uint32_t manCtrCool:1;          // Ручное управление компрессором
            uint32_t manCtrHeat:1;          // Ручное управление нагревателем

            uint32_t errT1min:1;            // Слишком низкая температура 1
            uint32_t errT1max:1;            // Слишком высокая температура 1
            uint32_t errT2min:1;            // Слишком низкая температура 2
            uint32_t errT2max:1;            // Слишком высокая температура 2
            uint32_t errLowCurrentCool:1;   // Слишком низкий ток компрессора
            uint32_t errHighCurrentCool:1;  // Слишком высокий ток компрессора
            uint32_t errLowCurrentHeat:1;   // Слишком низкий ток нагревателя
            uint32_t errHighCurrentHeat:1;  // Слишком высокий ток нагревателя
            uint32_t errLowVoltage:1;       // Слишком низкое напряжение
            uint32_t errHighVoltage:1;      // Слишком высокой напряжение
            uint32_t errFun1:1;             // Ошибка вентилятора 1
            uint32_t errFun2:1;             // Ошибка вентилятора 2
            uint32_t errTmp1:1;             // Ошибка датчика температуры 1
            uint32_t errTmp2:1;             // Ошибка датчика температуры 2
            
            uint32_t alm1:1;                // Внешняя ошибка 1
            uint32_t alm2:1;                // Внешняя ошибка 2

            uint32_t debug:4;
            uint32_t reserved:2;
        };
        uint32_t status;
    };
    uint16_t voltage;                   // Напряжение системы
    uint16_t current;                   // Ток системы
    int16_t temp1;                      // Температура на датчике 1
    int16_t temp2;                      // Температура на датчике 2
    uint16_t rpmFan1;                   // Обороты 1 вентилятора
    uint16_t rpmFan2;                   // Обороты 2 вентилятора
} coreStatus_t;

typedef struct {
    uint32_t time;
    uint16_t adc[4];
    int32_t I1;
    int32_t I2;
    uint16_t rpm1S;
    uint16_t rpm2S;
    uint32_t cnt;
} coreDebug_t;

extern coreInfo_t coreInfo;
extern coreSetting_t coreSetting;
extern coreStatus_t coreStatus;
extern coreDebug_t coreDebug;

uint8_t checkSettingParam(coreSetting_t* st);
void loadFlashSetting();
void saveFlashSetting(coreSetting_t* st);

#endif