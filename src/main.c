#include "system.h"
#include "sysControl.h"
#include "rammap.h"
#include "menu.h"
#include "uart.h"

int main(void){ 
    sysInit();
    loadFlashSetting();

    while(1){
        if(sec>1)updateMenu();
        logicUart();
        IWDG->KR = 0xAAAA;
    }
}
