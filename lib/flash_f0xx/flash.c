#include "flash.h"
#include "stm32f0xx.h"

// Инициализация flash для записи
void flashInit(){
    while ((FLASH->SR & FLASH_SR_BSY) != 0);
    if ((FLASH->CR & FLASH_CR_LOCK) != 0){
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }
}

// Очистка сектора(1КБ)
void flashSectorClear(uint32_t adr){
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = adr;
    FLASH->CR |= FLASH_CR_STRT;

    while ((FLASH->SR & FLASH_SR_BSY) != 0);

    if ((FLASH->SR & FLASH_SR_EOP) != 0)
        FLASH->SR = FLASH_SR_EOP;

    FLASH->CR &= ~FLASH_CR_PER;
}

// Запись 2 байт данных(связанно с организацией flash) по адресу
void flashWrite(uint32_t adr, uint16_t data){
    FLASH->CR |= FLASH_CR_PG;
    *(__IO uint16_t*)(adr) = data;

    while ((FLASH->SR & FLASH_SR_BSY) != 0);

    if ((FLASH->SR & FLASH_SR_EOP) != 0)
        FLASH->SR = FLASH_SR_EOP;
    FLASH->CR &= ~FLASH_CR_PG;
}
