#include <project.h>

#include <stdbool.h>
#include <stdint.h>

#include "iprintf/iprintf.h"

#include "is31fl3236.h"

static volatile bool tiltChanged;
CY_ISR(TiltChangeISR) {
    tiltChanged = true;
    
    TiltSideISR_ClearPending();
    TiltUpISR_ClearPending();
}

int main()
{
    CyGlobalIntEnable;
    
    // init uart
    UART_Start();
    iprintf("\nTomato Tusk " __DATE__ " : " __TIME__ " Starting...\n");
    
    // setup tilt sensors
    tiltChanged = false;
    //FIXME en
    TiltSideISR_StartEx(TiltChangeISR);
    TiltUpISR_StartEx(TiltChangeISR);
    
    if(!is3_Start()) {
        iprintf("Failed to start LED controller\n");
    }
    
    //FIXME rm
    
    is3_SetLED(10, 20);
    is3_SetLED(11, 40);
    is3_SetLED(12, 60);
    is3_SetLED(13, 80);
    is3_SetLED(14, 100);
    
    is3_SetLED(23, 100);
    is3_SetLED(32, 100);
    //is3_SetLED(33, 100);
    //is3_SetLED(34, 100);
    is3_updateDisplay();

    //FIXME rm
    iprintf("Into main loop\n");
    int i = 0;
    for(;;)
    {
        //TODO debounce tilt changes (just wait XXXms to settle)
        if(tiltChanged) {
            iprintf("Tilt changed!");
            tiltChanged = false;
        }
        
        CyDelay(2 * 1000);
        //TODO set some LEDs
        if(!is3_SetLED(i, 100)) {
            iprintf("Failed to set LED %d\n", i);
        }
        if(!is3_updateDisplay()) {
            iprintf("Failed to update display\n");
        }
        iprintf("Set LED %d\n", i);
        i++;
        if(i > 35) {
            i = 0;
        }
    }
}
