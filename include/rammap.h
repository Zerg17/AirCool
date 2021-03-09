#pragma once
#ifndef RAMMAP
#define RAMMAP

#include <stdint.h>
#include "config.h"

#define tCool_DEF 3500
#define tCool_MIN 2000
#define tCool_MAX 5500

#define tHeat_DEF 500
#define tHeat_MIN -1000
#define tHeat_MAX 1500

#define deltaTCool_DEF 500
#define deltaTCool_MIN 100
#define deltaTCool_MAX 2000

#define deltaTHeat_DEF 500
#define deltaTHeat_MIN 100
#define deltaTHeat_MAX 1000

#define calidT_DEF 0
#define calidT_MIN -1000
#define calidT_MAX 1000

#define calidOffsetCurrent_DEF 0
#define calidOffsetCurrent_MIN -1000
#define calidOffsetCurrent_MAX 1000

#define calidoffsetVoltage_DEF 0
#define calidoffsetVoltage_MIN -1000
#define calidoffsetVoltage_MAX 1000

#define alrmTmax_DEF 4500
#define alrmTmax_MIN 3000
#define alrmTmax_MAX 7000

#define alrmTmin_DEF 0
#define alrmTmin_MIN -4000
#define alrmTmin_MAX 2000

#define heaterCurrentMin_DEF 10
#define heaterCurrentMin_MIN 0
#define heaterCurrentMin_MAX 1000

#define heaterCurrentMax_DEF 300
#define heaterCurrentMax_MIN 100
#define heaterCurrentMax_MAX 1000

#define compressorCurrentMin_DEF 10
#define compressorCurrentMin_MIN 0
#define compressorCurrentMin_MAX 1000

#define compressorCurrentMax_DEF 300
#define compressorCurrentMax_MIN 100
#define compressorCurrentMax_MAX 1000

#define alrmVmin_DEF 4000
#define alrmVmin_MIN 4000
#define alrmVmin_MAX 4800

#define alrmVmax_DEF 5700
#define alrmVmax_MIN 5000
#define alrmVmax_MAX 6300

#define minFanSpeedRPM1_DEF 2000
#define minFanSpeedRPM1_MIN 1500
#define minFanSpeedRPM1_MAX 3000

#define fanSpeedRPM1_DEF 2800
#define fanSpeedRPM1_MIN 1500
#define fanSpeedRPM1_MAX 3000

#define minFanSpeedRPM2_MIN 1500
#define minFanSpeedRPM2_MAX 3000

#define fanSpeedRPM2_MIN 1500
#define fanSpeedRPM2_MAX 3000

#define numPulsesFun_DEF 2

#define coolNumber_DEF 0

#define typePin_DEF 0

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
    afterErrMode=0xD,
    offMode=0xE,
    errMode=0xF
};

typedef struct{
    uint32_t sn;
    uint32_t vp;
    uint32_t type;
} coreInfo_t;

typedef struct coreSetting{
    int16_t tCool;                 // 1 Температура охлаждения
    int16_t tHeat;                 // 2 Температура нагрева

    int16_t deltaTCool;            // 3 Дельта температуры охлаждения
    int16_t deltaTHeat;            // 4 Дельта температуры нагрева

    int16_t calidT1;               // 5 Калибровка смещения температуры 1
    int16_t calidT2;               // 6 Калибровка смещения температуры 2
    uint16_t reserved0;
    uint16_t reserved1;
    uint16_t reserved2;
    uint16_t reserved3;
    uint16_t reserved4;
    uint16_t reserved5;

    int16_t calidOffsetCurrent;    // 13 Калибровка тока
    uint16_t reserved6;
    int16_t calidoffsetVoltage;    // 15 Калибровка напряжения
    uint16_t reserved7;

    int16_t alrmT1min;             // 17 Критически низкая температура
    int16_t alrmT1max;             // 18 Критически высокая температура
    int16_t alrmT2min;             // 19 Критически низкая температура
    int16_t alrmT2max;             // 20 Критически высокая температура

    uint16_t heaterCurrentMin;     // 21 Минимальный ток нагревателя
    uint16_t heaterCurrentMax;     // 22 Максимальный ток нагревателя

    uint16_t compressorCurrentMin; // 23 Минимальный ток компрессора
    uint16_t compressorCurrentMax; // 24 Максимальный ток компрессора

    uint16_t alrmVmin;             // 25 Минимальное напряжение
    uint16_t alrmVmax;             // 26 Максимальное напряжение

    uint16_t minFanSpeedRPM1;      // 27 Минимальные обороты 1 вентилятора
    uint16_t fanSpeedRPM1;         // 28 Номинальные обороты 1 вентилятора
    uint16_t numPulsesFun1;        // 29 Импульсов на оборот 1 вентилятора
    
    uint16_t minFanSpeedRPM2;      // 30 Минимальные обороты 2 вентилятора
    uint16_t fanSpeedRPM2;         // 31 Номинальные обороты 2 вентилятора
    uint16_t numPulsesFun2;        // 32 Импульсов на оборот 2 вентилятора

    uint16_t coolNumber;           // 33 Номер кондиционера

    uint16_t typePin;              // 34 Тип входорв и выходов

    union{
        struct{
            uint16_t coolCtr:1;    // Управление компрессором
            uint16_t heatCtr:1;    // Управление нагревателем
        };
        uint16_t flags1;
    };
    uint16_t flags2;
    uint16_t flags3;
    uint16_t flags4;
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

extern coreInfo_t coreInfo;
extern coreSetting_t coreSetting;
extern coreStatus_t coreStatus;

uint8_t checkSettingParam(coreSetting_t* st);
void loadFlashSetting();
void saveFlashSetting(coreSetting_t* st);

#endif