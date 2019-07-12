#include "settings.h"

extern bool screen_update;

Screen_Settings::Screen_Settings(){
    this->pos = 0;
    this->items[0] = "Mode";
    this->items[1] = "Sensitivity";
    this->items[2] = "Timeout";
}

void Screen_Settings::draw(U8G2_ST7920_128X64_F_HW_SPI u8g2){
    u8g2.setFont(u8g2_font_helvR10_tr);
    u8g2.drawStr(1, 11, "Settings:");
    for (byte i = 0; i < 3; i++){
        // Draw indicator
        if (this->pos == i){
            u8g2.drawStr(1, 12 + ((i+1)*12), ">");
        }
        u8g2.drawStr(11 ,12 + ((i+1)*12), this->items[i]);
    }
    // u8g2.setFont(u8g2_font_helvR08_tr);
    // u8g2.drawStr(10+w+5,35, "266");
}

void Screen_Settings::button_pressed(byte b){
    switch (b){
        case 0:
            // Up
            if (pos > 0){
                pos--;
                screen_update = true;
            }
            break;
        case 1:
            // Down
            if (pos < 2){
                pos++;
                screen_update = true;
            }
            break;
        case 2:
            // Back

            break;
        case 3:
            // OK

            break;
    }
}