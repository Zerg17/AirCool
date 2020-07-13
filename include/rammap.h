#pragma once
#ifndef RAMMAP
#define RAMMAP

#include <stdint.h>

#define tCool_DEF 3200
#define tCool_MIN 2500
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

#define alrmTmin_DEF 4500
#define alrmTmin_MIN 3000
#define alrmTmin_MAX 7000

#define alrmTmax_DEF 0
#define alrmTmax_MIN -4000
#define alrmTmax_MAX 2000

#define heaterCurrentMin_DEF 20
#define heaterCurrentMin_MIN 20
#define heaterCurrentMin_MAX 300

#define heaterCurrentMax_DEF 300
#define heaterCurrentMax_MIN 100
#define heaterCurrentMax_MAX 300

#define compressorCurrentMin_DEF 20
#define compressorCurrentMin_MIN 20
#define compressorCurrentMin_MAX 100

#define compressorCurrentMax_DEF 300
#define compressorCurrentMax_MIN 100
#define compressorCurrentMax_MAX 300

#define alrmVmin_DEF 4000
#define alrmVmin_MIN 4000
#define alrmVmin_MAX 4800

#define alrmVmax_DEF 5700
#define alrmVmax_MIN 5000
#define alrmVmax_MAX 6300

#define minFanSpeedRPM_DEF 1000
#define minFanSpeedRPM_MIN 500
#define minFanSpeedRPM_MAX 2000

#define fanSpeedRPM_DEF 2300
#define fanSpeedRPM_MIN 500
#define fanSpeedRPM_MAX 3000

#define numPulsesFun_DEF 2

#define coolNumber_DEF 0

#define typePin_DEF 0

#define CRITICAL_ERR 0x000037F0

enum coreMode{
    waitMode,
    coolMode,
    heatMode,
    testMode
};

typedef struct{
    uint32_t SN;
    uint32_t VP;
    uint32_t D;
} coreInfo_t;

typedef struct coreSetting{
    int16_t tCool;                  // Температура охлаждения
    int16_t tHeat;                  // Температура нагрева

    int16_t deltaTCool;             // Дельта температуры охлаждения
    int16_t deltaTHeat;             // Дельта температуры нагрева

    int16_t calidT1;                // Калибровка смещения температуры 1
    int16_t calidT2;                // Калибровка смещения температуры 2
    uint16_t reserved0;
    uint16_t reserved1;
    uint16_t reserved2;
    uint16_t reserved3;
    uint16_t reserved4;
    uint16_t reserved5;

    int16_t calidOffsetCurrent;     // Калибровка тока
    uint16_t reserved6;
    int16_t calidoffsetVoltage;     // Калибровка напряжения
    uint16_t reserved7;

    int16_t alrmTmin;               // Критически низкая температура
    int16_t alrmTmax;               // Критически высокая температура
    uint16_t heaterCurrentMin;      // Минимальный ток нагревателя
    uint16_t heaterCurrentMax;      // Максимальный ток нагревателя
    uint16_t compressorCurrentMin;  // Минимальный ток компрессора
    uint16_t compressorCurrentMax;  // Максимальный ток компрессора
    uint16_t alrmVmin;              // Минимальное напряжение
    uint16_t alrmVmax;              // Максимальное напряжение

    uint16_t minFanSpeedRPM1;       // Минимальные обороты 1 вентилятора
    uint16_t fanSpeedRPM1;          // Номинальные обороты 1 вентилятора
    uint16_t numPulsesFun1;         // Импульсов на оборот 1 вентилятора
    uint16_t minFanSpeedRPM2;       // Минимальные обороты 2 вентилятора
    uint16_t fanSpeedRPM2;          // Номинальные обороты 2 вентилятора
    uint16_t numPulsesFun2;         // Импульсов на оборот 2 вентилятора

    uint16_t coolNumber;            // Номер вентилятора

    uint16_t typePin;               // Тип входорв и выходов

    uint16_t flags[4];

    //uint16_t periodStat;
} coreSetting_t;

typedef struct {
    uint32_t time;              // Вермя системы
    union{
        struct{
            uint32_t mode:2;            // Режим работы
            uint32_t lowTemp:1;         // Слишком низкая температура
            uint32_t highTemp:1;        // Слишком высокая температура
            uint32_t lowCurrentHeat:1;  // Слишком низкий ток нагревателя
            uint32_t highCurrentHeat:1; // Слишком высокий ток нагревателя
            uint32_t lowCurrentCool:1;  // Слишком низкий ток компрессора
            uint32_t highCurrentCool:1; // Слишком высокий ток компрессора
            uint32_t lowVoltage:1;      // Слишком низкое напряжение
            uint32_t highVoltage:1;     // Слишком высокой напряжение
            uint32_t errFun1:1;         // Ошибка вентилятора 1
            uint32_t errFun2:1;         // Ошибка вентилятора 2
            uint32_t errTmp1:1;         // Ошибка датчика температуры 1
            uint32_t errTmp2:1;         // Ошибка датчика температуры 1
            uint32_t heatOn:1;          // Нагреватель включен
            uint32_t coolOn:1;          // Компрессор включен
            uint32_t reserved:16;
            };
        uint32_t status;
    };
    int16_t temp1;              // Температура на внешнем датчике
    int16_t temp2;              // Температура на внутреннем датчике
    uint16_t rmpFan1;           // Обороты 1 вентилятора
    uint16_t rmpFan2;           // Обороты 2 вентилятора
    uint16_t voltage;           // Напряжение системы
    uint16_t current;           // Ток системы
} coreStatus_t;

extern coreInfo_t coreInfo;
extern coreSetting_t coreSetting;
extern coreStatus_t coreStatus;

#endif