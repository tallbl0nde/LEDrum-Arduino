// Libraries
#include <Arduino.h>
#include <U8g2lib.h>

// Classes (screens)
#include "settings.h"

/* #region Definitions */
// Pins
#define pin_snare 6
#define pin_high 7
#define pin_low 10
#define pin_floor 11
#define pin_bass 12

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

// Settings
#define drum_bright_min 40.0
#define drum_bright_max 700.0
#define drum_bright_fix 200.0
#define drum_hit_fix 80.0
#define drum_idle_fix 110.0
#define update_interval 15
#define idle_timeout 4000

// Default Colour
#define red 0
#define green 130
#define blue 250
/* #endregion */

// Setup display
U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, 53, U8X8_PIN_NONE);

// Declaring variables
const byte pin_out[5] = {pin_snare, pin_high, pin_low, pin_floor, pin_bass};
const byte pin_in[5] = {pzo_snare, pzo_high, pzo_low, pzo_floor, pzo_bass};
const int drum_hit_min[5] = {120, 110, 100, 300, 40};

int drum_level[5] = {0, 0, 0, 0, 0};
int piezo_level[5] = {0, 0, 0, 0, 0};

const byte button_pin[4] = {button_up, button_down, button_back, button_ok};
int button_state[4] = {HIGH, HIGH, HIGH, HIGH};
int button_last[4] = {HIGH, HIGH, HIGH, HIGH};

byte rgb[3] = {0, 0, 0};

unsigned long update_ms = 0;
unsigned long idle_ms = 0;

// Depicts which mode to execute
byte mode = 2;
// Stores last mode (used to handle a change)
byte mode_last = 5;

// Screen index to draw
byte screen_num = 0;
// Indicates whether to redraw screen or not
bool screen_update = true;

// Stores screen objects
Screen * screens[1];
Screen_Settings scr_settings;

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

    update_ms = millis();
    idle_ms = millis();

    // Display
    u8g2.begin();

    // Assign screens
    screens[0] = &scr_settings;
}

void loop(){
    // Prepare next mode if changed
    if (mode != mode_last){
        switch (mode){
            case 0:
                setup_idle();
                break;
            case 1:
                setup_drum_solid();
                break;
            case 2:
                setup_drum_rgb();
                break;
        }
        mode_last = mode;
    }

    // Execute appropriate mode
    switch (mode){
        case 0:
            loop_idle();
            break;
        case 1:
            loop_drum_solid();
            break;
        case 2:
            loop_drum_rgb();
            break;
    }

    // Set idle if nothing happens after set interval
    if (mode != 0 && (millis() - idle_ms) > idle_timeout){
        mode = 0;
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