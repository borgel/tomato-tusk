#ifndef is31fl3236_H__
#define is31fl3236_H__
    
//Note the `is3` prefix is short for is31fl3236, which is painfully long
    
#include <stdbool.h>
#include <stdint.h>
    
//33 white, + 1 RGB (3 channels)
static const uint8_t NUM_LEDS       = 33;
    
bool is3_Start(void);
bool is3_updateDisplay(void);
bool is3_SetLED(uint8_t channel, uint8_t intensity);    //scale of 0-100%

//TODO API to take array of LED updates, and do them all in one transaction

#endif//is31fl3236_H__