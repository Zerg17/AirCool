#include "system.h"
#include "sysControl.h"
#include "ssd1306.h"
#include "rammap.h"
#include "menu.h"
#include "uart.h"
#include "flash.h"

extern uint8_t msgType;
extern uint8_t aa;

int main(void){
    sysInit();
    loadFlashSetting();
    while(sec<5);

    while(1){
        updateMenu();
    }
}
