#include <Arduino.h>
#include <U8g2lib.h>

#ifndef SCREEN_H
#define SCREEN_H

class Screen{
    public:
        // Called to draw/update this screen
        virtual void draw(U8G2_ST7920_128X64_F_HW_SPI) = 0;
        // Handles button presses
        virtual void button_pressed(byte) = 0;
};

#endif