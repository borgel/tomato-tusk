#include "is31fl3236.h"
#include <project.h>
#include "iprintf/iprintf.h"

#include <stdint.h>
#include <stdbool.h>

//#define LERP(from, to) ((A*(255-x)+B*x)/255)

static const uint8_t I2C_ADDR               = 0x3C; //shifted down, bottom bit is end of addr
static const uint8_t LED_CHANNELS           = 36;
static const uint8_t BRIGHTNESS_STEPS       = 255;

static const uint8_t I2C_REG_SHUTDOWN       = 0x00;
static const uint8_t I2C_REG_PWM_BASE       = 0x01;
static const uint8_t I2C_REG_PWM_UPDATE     = 0x25;
static const uint8_t I2C_REG_LED_CTRL_BASE  = 0x26;  //LED0 at 0x26, LED35 at 0x49
static const uint8_t I2C_REG_GLOBAL_CONTROL = 0x4A;

static const uint8_t SHUTDOWN_EN            = 0x1;
static const uint8_t SHUTDOWN_DISABLE       = 0x0;

union RegShutdown {
    uint8_t     data;
    struct {
        uint8_t shutdown:1;
        uint8_t      pad:7;
    };
};

union RegLEDControl {
    uint8_t     data;
    struct {
        uint8_t     en  :1;
        uint8_t     sl  :2;
        uint8_t     pad :5;
    };
};

union RegGlobalControl {
    uint8_t data;
    struct {
        uint8_t enable  :1;
        uint8_t pad     :7;
    };
};

//datasheet pg 9 (Table 5)
enum is3_CurrentLevel {
    CURR_HIGH       = 0x0,
    CURR_MEDIUM     = 0x1,
    CURR_LOW        = 0x2,
    CURR_VERY_LOW   = 0x3,
};

static bool is3_setShutdown(bool shut);
static bool setGlobalEnable(bool en);
static bool is3_SetChannelParameters(uint8_t channel, enum is3_CurrentLevel currentLevel, bool enable);
static bool setAllCurrentLevels(enum is3_CurrentLevel level);

static bool is3_WriteByte(uint8_t reg, uint8_t data);

bool is3_Start(void) {
    I2C_Start();
    
    //wake up!
    if(!is3_setShutdown(false)) {
        iprintf("is3: Failed to wake up controller\n");
        return false;   
    }
    
    //set lowest current level for all channels
    if(!setAllCurrentLevels(CURR_VERY_LOW)) {
        iprintf("is3: Failed to set all current levels\n");
        return false;
    }
    
    //display it
    if(!is3_updateDisplay()) {
        iprintf("is3: Failed to update display\n");
        return false;
    }
    
    if(!setGlobalEnable(true)) {
        iprintf("is3: Failed to set global enable\n");
        return false;
    }
    
    return true;
}

/*
Set the intensity of the given channel.
*/
bool is3_SetLED(uint8_t channel, uint8_t intensity) {
    if(channel > LED_CHANNELS) {
        iprintf("is3: Nonsense channel %d. Valid range is 0-%d\n", channel, LED_CHANNELS);
        return false;
    }
    
    //TODO do this more correctly
    //interpolate from 0->100 to 0->255
    uint8_t r = ((intensity/100)*BRIGHTNESS_STEPS);
    //FIXME rm
    //r = 0xFF;
    return is3_WriteByte(I2C_REG_PWM_BASE + channel, r);
}

/*
Force all written control values to be committed (and displayed). MUST be called
after changing what is being shown for it to matter.

Returns:
    true  - success
    false - failed
*/
bool is3_updateDisplay(void) {
    return is3_WriteByte(I2C_REG_PWM_UPDATE, 0x00);
}

bool is3_setShutdown(bool shut) {
    return is3_WriteByte(I2C_REG_SHUTDOWN, !shut);
}

/*
Write to the global control register to enable/disable all LEDs
*/
static bool setGlobalEnable(bool en) {
    union RegGlobalControl r = {.enable = !en};
    
    return is3_WriteByte(I2C_REG_GLOBAL_CONTROL, r.data);
}

/*
Set global current control levels.
`Channel` is base 0 (the 1st channel is #0)
*/
static bool is3_SetChannelParameters(uint8_t channel, enum is3_CurrentLevel currentLevel, bool enable) {
    union RegLEDControl r = {.sl = currentLevel, .en = enable};
    return is3_WriteByte(I2C_REG_LED_CTRL_BASE + channel, r.data);
}

/*
Set current level parameters for the all channels one at a time
*/
//TODO do this in one transaction instead of many
static bool setAllCurrentLevels(enum is3_CurrentLevel level) {
    int i;
    for(i = 0; i < LED_CHANNELS; i++) {
        if(!is3_SetChannelParameters(i, level, true)) {
            return false;
        }
    }
    return true;
}

/*
Write a single byte and return if it was sucessful.
*/
static bool is3_WriteByte(uint8_t reg, uint8_t data) {
    uint32_t res = 0;
    
    res |= I2C_I2CMasterSendStart(I2C_ADDR, I2C_I2C_WRITE_XFER_MODE);
    res |= I2C_I2CMasterWriteByte(reg);
    res |= I2C_I2CMasterWriteByte(data);
    res |= I2C_I2CMasterSendStop();
    
    return (res == I2C_I2C_MSTR_NO_ERROR);
}
