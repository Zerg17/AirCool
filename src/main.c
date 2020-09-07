#include "system.h"
#include "sysControl.h"
#include "rammap.h"
#include "menu.h"
#include "ssd1306.h"
#include "xprintf.h"

int main(void){
    xdev_out(ssd1306_Char);
    sysInit();
    loadFlashSetting();
    while(sec<5);

    while(1){
        updateMenu();
    }
}
