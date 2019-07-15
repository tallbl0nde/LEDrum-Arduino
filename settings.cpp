#include "settings.h"

extern byte screen_num;
extern bool screen_update;

Screen_Settings::Screen_Settings(struct settings * s) : Screen(s){
    this->pos = 0;
    this->items[0] = "Lighting";
    this->items[1] = "Idle";
    this->items[2] = "Drum Sensitivity";
    this->items[3] = "Backlight";
    this->items[4] = "Miscellaneous";
    this->items[5] = "[SAVE]";
}

void Screen_Settings::draw(U8G2_ST7920_128X64_F_HW_SPI u8g2){
    u8g2.setFont(u8g2_font_crox2hb_tr);
    u8g2.drawStr(1, 11, "Settings:");
    u8g2.setFont(u8g2_font_rosencrantz_nbp_tr);
    for (byte i = 0; i < 6; i++){
        // Draw indicator
        if (this->pos == i){
            u8g2.drawStr(1, 13 + ((i+1)*13), ">");
        }
        u8g2.drawStr(11 ,13 + ((i+1)*13), this->items[i]);
    }
}

void Screen_Settings::button_pressed(byte b){
    switch (b){
        case 0:
            // Up
            if (this->pos > 0){
                this->pos--;
                screen_update = true;
            }
            break;
        case 1:
            // Down
            if (this->pos < 5){
                this->pos++;
                screen_update = true;
            }
            break;
        case 2:
            // Back

            break;
        case 3:
            // OK
            switch (pos){
                case 0:
                    // Lighting Mode
                    screen_num = 1;
                    screen_update = true;
                    break;
                case 1:
                    // Idle Options
                    screen_num = 3;
                    screen_update = true;
                    break;
                case 2:
                    // Drum Sensitivity

                    break;
                case 3:
                    // Backlight

                    break;
                case 4:
                    // Miscellaneous

                    break;
                case 5:
                    // Save

                    break;
            }
            break;
    }
}