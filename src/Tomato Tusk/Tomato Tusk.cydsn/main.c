#include <project.h>

#include <stdbool.h>
#include <stdint.h>

#include "iprintf/iprintf.h"

#include "is31fl3236.h"
int main()
{
    CyGlobalIntEnable;
    
    // init uart
    UART_Start();
    iprintf("\nTomato Tusk " __DATE__ " : " __TIME__ " Starting...\n");
    
    // setup tilt sensors
    tiltChanged = false;
    //FIXME en
    //TiltSideISR_StartEx(TiltChangeISR);
    //TiltUpISR_StartEx(TiltChangeISR);
    
    if(!is3_Start()) {
        iprintf("Failed to start LED controller\n");
    }
    
    //FIXME rm
    is3_SetLED(32, 100);
    is3_SetLED(33, 100);
    is3_SetLED(34, 100);
    is3_updateDisplay();
    

    //FIXME rm
    iprintf("Into main loop\n");
    for(;;)
    {
    }
}
