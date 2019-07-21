// Libraries
#include <Arduino.h>
#include <U8g2lib.h>
#include "setting.h"

// Classes (screens)
#include "backlight.h"
#include "idle.h"
#include "lighting.h"
#include "misc.h"
#include "rgbselect.h"
#include "sensitivity.h"
#include "settings.h"

/* #region Definitions */
#define pin_snare 6
#define pin_high 7
#define pin_low 10
#define pin_floor 11
#define pin_bass 12

#define pin_backlight 8

#define pin_red 4
#define pin_green 3
#define pin_blue 2

#define pzo_snare A0
#define pzo_high A1
#define pzo_low A2
#define pzo_floor A4
#define pzo_bass A3

#define button_up 33
#define button_down 35
#define button_back 37
#define button_ok 39
/* #endregion */

// Setup display
U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, 53, U8X8_PIN_NONE);

// Declaring variables
const byte pin_out[5] = {pin_snare, pin_high, pin_low, pin_floor, pin_bass};
const byte pin_in[5] = {pzo_snare, pzo_high, pzo_low, pzo_floor, pzo_bass};
int drum_level[5] = {0, 0, 0, 0, 0};
int piezo_level[5] = {0, 0, 0, 0, 0};

const byte button_pin[4] = {button_up, button_down, button_back, button_ok};
int button_state[4] = {HIGH, HIGH, HIGH, HIGH};
int button_last[4] = {HIGH, HIGH, HIGH, HIGH};

// Values to use for PWM
byte rgb[3] = {0, 0, 0};

// Timers
unsigned long fade_ms = 0;
unsigned long idle_ms = 0;
unsigned long shift_ms = 0;

// If in idle or not
byte is_idle = false;
// Stores last mode (used to handle a change)
byte mode_last = 255;

// Screen index to draw
byte screen_num = 0;
// Indicates whether to redraw screen or not
bool screen_update = true;

struct settings setting;
// Stores screen objects
Screen * screens[7];
Screen_Backlight scr_backlight(&setting);
Screen_Idle scr_idle(&setting);
Screen_Lighting scr_lighting(&setting);
Screen_Misc scr_misc(&setting);
Screen_RGBSelect scr_rgbselect(&setting);
Screen_Settings scr_settings(&setting);
Screen_Sensitivity scr_sensitivity(&setting);

void setup(){
    // Setup Pins
    for (byte i = 0; i < 5; i++){
        pinMode(pin_out[i], OUTPUT);
        pinMode(pin_in[i], INPUT);
    }
    pinMode(pin_red, OUTPUT);
    pinMode(pin_green, OUTPUT);
    pinMode(pin_blue, OUTPUT);
    pinMode(button_up, INPUT_PULLUP);
    pinMode(button_down, INPUT_PULLUP);
    pinMode(button_back, INPUT_PULLUP);
    pinMode(button_ok, INPUT_PULLUP);

    fade_ms = millis();
    idle_ms = millis();
    shift_ms = millis();

    // Read settings
    setting.lighting_mode = 0;
    setting.lighting_solid_rgb[0] = 0;
    setting.lighting_solid_rgb[1] = 130;
    setting.lighting_solid_rgb[2] = 250;
    setting.lighting_fade_speed = 15;
    setting.lighting_shift_speed = 15;
    setting.idle_timeout = 15000;
    setting.idle_animate_speed = 15;
    setting.sensitivity[0] = 120;
    setting.sensitivity[1] = 110;
    setting.sensitivity[2] = 100;
    setting.sensitivity[3] = 300;
    setting.sensitivity[4] = 40;
    setting.backlight_brightness = 100;
    setting.miscellaneous[0] = 40.0;
    setting.miscellaneous[1] = 700.0;
    setting.miscellaneous[2] = 200.0;
    setting.miscellaneous[3] = 80.0;
    setting.miscellaneous[4] = 110.0;

    // Display
    u8g2.begin();
    setBacklight(setting.backlight_brightness);

    // Assign screens
    screens[0] = &scr_settings;
    screens[1] = &scr_backlight;
    screens[2] = &scr_sensitivity;
    screens[3] = &scr_idle;
    screens[4] = &scr_lighting;
    screens[5] = &scr_rgbselect;
    screens[6] = &scr_misc;
}

void loop(){
    if (!is_idle){
        // Prepare next mode if changed
        if (setting.lighting_mode != mode_last){
            switch (setting.lighting_mode){
                case 0:
                    setup_drum_solid();
                    break;
                case 1:
                    setup_drum_rgb();
                    break;
                case 2:
                    setup_drum_select();
                    break;
            }
            mode_last = setting.lighting_mode;
        }

        // Execute appropriate mode
        switch (setting.lighting_mode){
            case 0:
                loop_drum_solid();
                break;
            case 1:
                loop_drum_rgb();
                break;
            case 2:
                loop_drum_select();
                break;
        }

        // Set idle if nothing happens after set interval
        if ((millis() - idle_ms) > setting.idle_timeout){
            is_idle = true;
            setup_idle_shift();
        }
    } else {
        loop_idle_shift();
    }

    // Update screen when requested
    if (screen_update){
        u8g2.clearBuffer();
        screens[screen_num]->draw(u8g2);
        u8g2.sendBuffer();
        screen_update = false;
    }

    // Handle button presses and send to screen object
    for (byte i = 0; i < 4; i++){
        button_state[i] = digitalRead(button_pin[i]);
        if (button_state[i] == LOW && button_state[i] != button_last[i]){
            screens[screen_num]->button_pressed(i);
        }
        button_last[i] = button_state[i];
    }
}

// Helper Functions
// Set pwm output to match r, g, b
void writeRGB(byte r, byte g, byte b){
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
    analogWrite(pin_red, rgb[0]);
    analogWrite(pin_green, rgb[1]);
    analogWrite(pin_blue, rgb[2]);
}

// Turn all drum MOSFETs off
void allOff(){
    for (byte i = 0; i < 5; i++){
        analogWrite(pin_out[i], 0);
    }
}

// Adjust PWM for screen brightness
void setBacklight(byte b){
    setting.backlight_brightness = b;
    analogWrite(pin_backlight, byte(255 * b/100.0));
}