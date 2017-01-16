#include "led.h"
#include "is31fl3236.h"
#include "iprintf/iprintf.h"
#include <project.h>

//FIXME rm
//#35 is top LED
//0-31 are bar
//32,33,34 are RGB

bool led_Start(void) {
    //TODO init state tracking
    
    if(!is3_Start()) {
        iprintf("led: Failed to init is3\n");
        return false;
    }
    
    return true;
}

void led_GiveTime(void) {
}