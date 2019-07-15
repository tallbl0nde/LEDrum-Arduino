#include "screen.h"

#ifndef SETTINGS_H
#define SETTINGS_H

class Screen_Settings : public Screen{
    private:
        // Cursor position
        byte pos;
        // Menu items
        const char * items[7];

    public:
        // Constructor to initalise private vars
        Screen_Settings(struct settings *);
        // Draw settings screen
        virtual void draw(U8G2_ST7920_128X64_F_HW_SPI);
        // Handle relevant actions for screen on button press
        virtual void button_pressed(byte);
};

#endif