#include "screen.h"

#ifndef LIGHTING_H
#define LIGHTING_H

class Screen_Lighting : public Screen{
    private:
        // Cursor position
        byte pos;
        // Selected position
        byte sel_pos;

        // Handle buttons when second entry Selected
        void button_second(byte);
        // Handle buttons when third entry selected
        void button_third(byte);

    public:
        // Constructor to initalise private vars
        Screen_Lighting(struct settings *);
        // Draw settings screen
        virtual void draw(U8G2_ST7920_128X64_F_HW_SPI);
        // Handle relevant actions for screen on button press
        virtual void button_pressed(byte);
};

#endif