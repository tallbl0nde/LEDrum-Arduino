#include "lighting.h"

// Need to make this less dodgy
extern int screen_num;
extern bool screen_update;

Screen_Lighting::Screen_Lighting(struct settings * s) : Screen(s){
    this->pos = 0;
    this->sel_pos = 255;
}

void Screen_Lighting::draw(U8G2_ST7920_128X64_F_HW_SPI u8g2){
    u8g2.setFont(u8g2_font_crox2hb_tr);
    u8g2.drawStr(1, 11, "Lighting:");
    u8g2.setFont(u8g2_font_rosencrantz_nbp_tr);
    if (this->setting->lighting_mode == 0){
        u8g2.drawStr(11, 26, "Mode: Solid");
    } else {
        u8g2.drawStr(11, 26, "Mode: RGB Shift");
    }
    int w = u8g2.drawStr(11, 39, "Fade Speed (ms): ");
    u8g2.setCursor(11 + w, 39);
    u8g2.print(this->setting->lighting_fade_speed);
    if (this->setting->lighting_mode == 0){
        w = u8g2.drawStr(11, 52, "RGB");
    } else {
        w = u8g2.drawStr(11, 52, "Shift Speed (ms): ");
        u8g2.setCursor(11 + w, 52);
        u8g2.print(this->setting->lighting_shift_speed);
    }
    for (byte i = 0; i < 3; i++){
        if (i == this->sel_pos){
            u8g2.drawStr(1, 13 + ((i+1)*13), "*");
        } else if (i == this->pos){
            u8g2.drawStr(1, 13 + ((i+1)*13), ">");
        }
    }
}

void Screen_Lighting::button_pressed(byte b){
    switch (this->sel_pos){
        case 1:
            this->button_second(b);
            break;
        case 2:
            this->button_third(b);
            break;
        default:
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
                    if (this->pos < 2){
                        this->pos++;
                        screen_update = true;
                    }
                    break;
                case 2:
                    // Back
                    screen_num = 0;
                    screen_update = true;
                    break;
                case 3:
                    // OK
                    switch (this->pos){
                        case 0:
                            (this->setting->lighting_mode == 0) ? this->setting->lighting_mode = 1 : this->setting->lighting_mode = 0;
                            screen_update = true;
                            break;
                        case 1:
                            this->sel_pos = this->pos;
                            screen_update = true;
                            break;
                        case 2:
                            if (this->setting->lighting_mode == 1){
                                this->sel_pos = this->pos;
                            } else {
                                screen_num = 5;
                                this->setting->lighting_mode = 2;
                            }
                            screen_update = true;
                            break;
                    }
                    break;
            }
    }
}

void Screen_Lighting::button_second(byte b){
    switch (b){
        case 0:
            // Up
            if (this->setting->lighting_fade_speed < 100){
                this->setting->lighting_fade_speed++;
                screen_update = true;
            }
            break;
        case 1:
            // Down
            if (this->setting->lighting_fade_speed > 0){
                this->setting->lighting_fade_speed--;
                screen_update = true;
            }
            break;
        case 2:
            // Back
            this->sel_pos = 255;
            screen_update = true;
            break;
        case 3:
            // OK
            this->sel_pos = 255;
            screen_update = true;
            break;
    }
}

void Screen_Lighting::button_third(byte b){
    switch (b){
        case 0:
            // Up
            if (this->setting->lighting_shift_speed < 100){
                this->setting->lighting_shift_speed++;
                screen_update = true;
            }
            break;
        case 1:
            // Down
            if (this->setting->lighting_shift_speed > 0){
                this->setting->lighting_shift_speed--;
                screen_update = true;
            }
            break;
        case 2:
            // Back
            this->sel_pos = 255;
            screen_update = true;
            break;
        case 3:
            // OK
            this->sel_pos = 255;
            screen_update = true;
            break;
    }
}