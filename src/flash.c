#include "flash.h"
#include "system.h"

void flashInit(){
    while ((FLASH->SR & FLASH_SR_BSY) != 0);
    if ((FLASH->CR & FLASH_CR_LOCK) != 0){
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }
}

void flashSectorClear(uint32_t adr){
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = adr;
    FLASH->CR |= FLASH_CR_STRT;

    while ((FLASH->SR & FLASH_SR_BSY) != 0);

    if ((FLASH->SR & FLASH_SR_EOP) != 0)
        FLASH->SR = FLASH_SR_EOP;

    FLASH->CR &= ~FLASH_CR_PER;
}

void flashWrite(uint32_t adr, uint16_t data){
    FLASH->CR |= FLASH_CR_PG;
    *(__IO uint16_t*)(adr) = data;

    while ((FLASH->SR & FLASH_SR_BSY) != 0);

    if ((FLASH->SR & FLASH_SR_EOP) != 0)
        FLASH->SR = FLASH_SR_EOP;
    FLASH->CR &= ~FLASH_CR_PG;
}
