#include "idle.h"

// Need to make this less dodgy
extern int screen_num;
extern bool screen_update;

Screen_Idle::Screen_Idle(struct settings * s) : Screen(s){
    this->pos = 0;
    this->sel_pos = 255;
}

void Screen_Idle::draw(U8G2_ST7920_128X64_F_HW_SPI u8g2){
    u8g2.setFont(u8g2_font_crox2hb_tr);
    u8g2.drawStr(1, 11, "Idle:");
    u8g2.setFont(u8g2_font_rosencrantz_nbp_tr);
    int w = u8g2.drawStr(11, 26, "Timeout (s): ");
    u8g2.setCursor(11 + w, 26);
    u8g2.print(this->setting->idle_timeout/1000);
    w = u8g2.drawStr(11, 39, "Animate Speed (ms): ");
    u8g2.setCursor(11 + w, 39);
    u8g2.print(this->setting->idle_animate_speed);
    for (byte i = 0; i < 2; i++){
        if (i == this->sel_pos){
            u8g2.drawStr(1, 13 + ((i+1)*13), "*");
        } else if (i == this->pos){
            u8g2.drawStr(1, 13 + ((i+1)*13), ">");
        }
    }
}

void Screen_Idle::button_pressed(byte b){
    switch (this->sel_pos){
        case 0:
            this->button_first(b);
            break;
        case 1:
            this->button_second(b);
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
                    if (this->pos < 1){
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
                    this->sel_pos = this->pos;
                    screen_update = true;
                    break;
            }
    }
}

void Screen_Idle::button_first(byte b){
    switch (b){
        case 0:
            // Up
            if (this->setting->idle_timeout < 60000){
                this->setting->idle_timeout+=1000;
                screen_update = true;
            }
            break;
        case 1:
            // Down
            if (this->setting->idle_timeout > 1000){
                this->setting->idle_timeout-=1000;
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

void Screen_Idle::button_second(byte b){
    switch (b){
        case 0:
            // Up
            if (this->setting->idle_animate_speed < 100){
                this->setting->idle_animate_speed++;
                screen_update = true;
            }
            break;
        case 1:
            // Down
            if (this->setting->idle_animate_speed > 0){
                this->setting->idle_animate_speed--;
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