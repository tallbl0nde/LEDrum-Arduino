#include "screen.h"

#ifndef IDLE_H
#define IDLE_H

class Screen_Idle : public Screen{
    private:
        // Cursor position
        byte pos;
        // Selected position
        byte sel_pos;

        // Handle buttons when first entry Selected
        void button_first(byte);
        // Handle buttons when second entry selected
        void button_second(byte);

    public:
        // Constructor to initalise private vars
        Screen_Idle(struct settings *);
        // Draw settings screen
        virtual void draw(U8G2_ST7920_128X64_F_HW_SPI);
        // Handle relevant actions for screen on button press
        virtual void button_pressed(byte);
};

#endif