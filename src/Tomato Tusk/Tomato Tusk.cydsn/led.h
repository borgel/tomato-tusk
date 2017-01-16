#ifndef LED_H__
#define LED_H__
    
#include <stdbool.h>
#include <stdint.h>

bool led_Start(void);
void led_GiveTime(void);

bool led_SetLED(uint8_t intensity);
bool led_SetRGB(uint8_t r, uint8_t g, uint8_t b);

#endif//LED_H__