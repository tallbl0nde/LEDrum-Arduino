#include "screen.h"

#ifndef MISC_H
#define MISC_H

class Screen_Misc : public Screen{
    private:
        // Cursor position
        byte pos;
        // Selected position
        byte sel_pos;
        // Menu items
        const char * items[6];

    public:
        // Constructor to initalise private vars
        Screen_Misc(struct settings *);
        // Draw settings screen
        virtual void draw(U8G2_ST7920_128X64_F_HW_SPI);
        // Handle relevant actions for screen on button press
        virtual void button_pressed(byte);
};

#endif