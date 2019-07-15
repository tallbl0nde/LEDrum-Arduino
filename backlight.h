#include "screen.h"

#ifndef BACKLIGHT_H
#define BACKLIGHT_H

class Screen_Backlight : public Screen{
    private:
        bool selected;

    public:
        // Constructor to initalise private vars
        Screen_Backlight(struct settings *);
        // Draw settings screen
        virtual void draw(U8G2_ST7920_128X64_F_HW_SPI);
        // Handle relevant actions for screen on button press
        virtual void button_pressed(byte);
};

#endif