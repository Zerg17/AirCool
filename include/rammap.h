#pragma once
#ifndef RAMMAP
#define RAMMAP

#include <stdint.h>

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

    uint16_t minFanSpeedRMP1;       // Минимальные обороты 1 вентилятора
    uint16_t fanSpeedRMP1;          // Номинальные обороты 1 вентилятора
    uint16_t numPulsesFun1;         // Импульсов на оборот 1 вентилятора
    uint16_t minFanSpeedRMP2;       // Минимальные обороты 2 вентилятора
    uint16_t fanSpeedRMP2;          // Номинальные обороты 2 вентилятора
    uint16_t numPulsesFun2;         // Импульсов на оборот 2 вентилятора

    uint16_t coolNumber;            // Номер вентилятора

    uint16_t typePin;               // Тип входорв и выходов

    uint16_t flags[4];

    //uint16_t periodStat;
} coreSetting_t;

typedef struct {
    uint32_t time;      // Вермя системы
    uint32_t status;    // Статус системы
    int16_t temp1;      // Температура на внешнем датчике
    int16_t temp2;      // Температура на внутреннем датчике
    uint16_t rmpFan1;   // Обороты 1 вентилятора
    uint16_t rmpFan2;   // Обороты 2 вентилятора
    uint16_t voltage;   // Напряжение системы
    uint16_t current;   // Ток системы
} coreStatus_t;

extern coreInfo_t coreInfo;
extern coreSetting_t coreSetting;
extern coreStatus_t coreStatus;

#endif