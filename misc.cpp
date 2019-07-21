#include "misc.h"

extern byte screen_num;
extern bool screen_update;

Screen_Misc::Screen_Misc(struct settings * s) : Screen(s){
    this->pos = 0;
    this->items[0] = "brightness_min:";
    this->items[1] = "brightness_max:";
    this->items[2] = "brightness_fix:";
    this->items[3] = "hit_fix:";
    this->items[4] = "idle_fix:";
    this->items[5] = " ";
}

void Screen_Misc::draw(U8G2_ST7920_128X64_F_HW_SPI u8g2){
    u8g2.setFont(u8g2_font_crox2hb_tr);
    u8g2.drawStr(1, 11, "Miscellaneous:");
    u8g2.setFont(u8g2_font_rosencrantz_nbp_tr);
    // No scroll
    if (this->pos < 3){
        for (byte i = 0; i < 4; i++){
            int w = u8g2.drawStr(11, 13 + ((i+1)*13), this->items[i]);
            u8g2.setCursor(w + 11, 13 + ((i+1)*13));
            u8g2.print(this->setting->miscellaneous[i]);
        }
        if (this->sel_pos == 255){
            u8g2.drawStr(1, 13 + ((this->pos+1)*13), ">");
        } else {
            u8g2.drawStr(1, 13 + ((this->pos+1)*13), "*");
        }
    } else {
    // Scroll effect
        for (byte i = this->pos-2; i < this->pos+2; i++){
            int w = u8g2.drawStr(11, 13 + ((i-(this->pos-3))*13), this->items[i]);
            if (i < 5){
                u8g2.setCursor(w + 11, 13 + ((i-(this->pos-3))*13));
                u8g2.print(this->setting->miscellaneous[i]);
            }
        }
        if (this->sel_pos == 255){
            u8g2.drawStr(1, 52, ">");
        } else {
            u8g2.drawStr(1, 52, "*");
        }
    }
}

void Screen_Misc::button_pressed(byte b){
    switch (b){
        case 0:
            // Up
            if (this->sel_pos == 255){
                if (this->pos > 0){
                    this->pos--;
                    screen_update = true;
                }
            } else {
                if (this->setting->miscellaneous[this->sel_pos] < 1000){
                    this->setting->miscellaneous[this->sel_pos]++;
                    screen_update = true;
                }
            }
            break;
        case 1:
            // Down
            if (this->sel_pos == 255){
                if (this->pos < 4){
                    this->pos++;
                    screen_update = true;
                }
            } else {
                if (this->setting->miscellaneous[this->sel_pos] > 0){
                    this->setting->miscellaneous[this->sel_pos]--;
                    screen_update = true;
                }
            }
            break;
        case 2:
            // Back
            (this->sel_pos == 255)? screen_num = 0 : this->sel_pos = 255;
            screen_update = true;
            break;
        case 3:
            // OK
            (this->sel_pos == 255)? this->sel_pos = this->pos : this->sel_pos = 255;
            screen_update = true;
            break;
    }
}