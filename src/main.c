#include "system.h"
#include "sysControl.h"
#include "rammap.h"
#include "menu.h"
#include "uart.h"

int main(void){
    
    sysInit();
    loadFlashSetting();
    while(sec<5);

    while(1){
        updateMenu();
        logicUart();
    }
}
