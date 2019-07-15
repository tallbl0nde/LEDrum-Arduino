#include "sensitivity.h"

extern byte screen_num;
extern bool screen_update;

Screen_Sensitivity::Screen_Sensitivity(struct settings * s) : Screen(s){
    this->pos = 0;
    this->items[0] = "Snare:";
    this->items[1] = "High:";
    this->items[2] = "Low:";
    this->items[3] = "Floor:";
    this->items[4] = "Bass:";
    this->items[5] = " ";
}

void Screen_Sensitivity::draw(U8G2_ST7920_128X64_F_HW_SPI u8g2){
    u8g2.setFont(u8g2_font_crox2hb_tr);
    u8g2.drawStr(1, 11, "Sensitivity:");
    u8g2.setFont(u8g2_font_rosencrantz_nbp_tr);
    // No scroll
    if (this->pos < 3){
        for (byte i = 0; i < 4; i++){
            u8g2.drawStr(11, 13 + ((i+1)*13), this->items[i]);
            u8g2.drawBox(50, 3 + ((i+1)*13), 50, 10);
            u8g2.setDrawColor(0);
            u8g2.drawBox(51, 4 + ((i+1)*13), 48, 8);
            u8g2.setDrawColor(1);
            u8g2.drawBox(51, 4 + ((i+1)*13), 48*(this->setting->sensitivity[i]/400.0), 8);
            u8g2.setCursor(106, 13 + ((i+1)*13));
            u8g2.print(this->setting->sensitivity[i]);
        }
        if (this->sel_pos == 255){
            u8g2.drawStr(1, 13 + ((this->pos+1)*13), ">");
        } else {
            u8g2.drawStr(1, 13 + ((this->pos+1)*13), "*");
        }
    } else {
    // Scroll effect
        for (byte i = this->pos-2; i < this->pos+2; i++){
            u8g2.drawStr(11, 13 + ((i-(this->pos-3))*13), this->items[i]);
            if (i < 5){
                u8g2.drawBox(50, 3 + ((i-(this->pos-3))*13), 50, 10);
                u8g2.setDrawColor(0);
                u8g2.drawBox(51, 4 + ((i-(this->pos-3))*13), 48, 8);
                u8g2.setDrawColor(1);
                u8g2.drawBox(51, 4 + ((i-(this->pos-3))*13), 48*(this->setting->sensitivity[i]/400.0), 8);
                u8g2.setCursor(106, 13 + ((i-(this->pos-3))*13));
                u8g2.print(this->setting->sensitivity[i]);
            }
        }
        if (this->sel_pos == 255){
            u8g2.drawStr(1, 52, ">");
        } else {
            u8g2.drawStr(1, 52, "*");
        }
    }
}

void Screen_Sensitivity::button_pressed(byte b){
    switch (b){
        case 0:
            // Up
            if (this->sel_pos == 255){
                if (this->pos > 0){
                    this->pos--;
                    screen_update = true;
                }
            } else {
                if (this->setting->sensitivity[this->sel_pos] < 400){
                    this->setting->sensitivity[this->sel_pos]++;
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
                if (this->setting->sensitivity[this->sel_pos] > 0){
                    this->setting->sensitivity[this->sel_pos]--;
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