#include "rgbselect.h"

// Need to make this less dodgy
extern int screen_num;
extern bool screen_update;

Screen_RGBSelect::Screen_RGBSelect(struct settings * s) : Screen(s){
    this->pos = 0;
    this->sel_pos = 255;
}

void Screen_RGBSelect::draw(U8G2_ST7920_128X64_F_HW_SPI u8g2){
    u8g2.setFont(u8g2_font_crox2hb_tr);
    u8g2.drawStr(1, 11, "RGB Colour:");
    u8g2.setFont(u8g2_font_rosencrantz_nbp_tr);

    int w = u8g2.drawStr(11, 26, "R:");
    u8g2.drawBox(16 + w, 16, 70, 10);
    u8g2.setDrawColor(0);
    u8g2.drawBox(17 + w, 17, 68, 8);
    u8g2.setDrawColor(1);
    u8g2.drawBox(17 + w, 17, 68*(this->setting->lighting_solid_rgb[0]/255.0), 8);
    u8g2.setCursor(90 + w, 26);
    u8g2.print(this->setting->lighting_solid_rgb[0]);

    w = u8g2.drawStr(11, 41, "G:");
    u8g2.drawBox(16 + w, 31, 70, 10);
    u8g2.setDrawColor(0);
    u8g2.drawBox(17 + w, 32, 68, 8);
    u8g2.setDrawColor(1);
    u8g2.drawBox(17 + w, 32, 68*(this->setting->lighting_solid_rgb[1]/255.0), 8);
    u8g2.setCursor(90 + w, 41);
    u8g2.print(this->setting->lighting_solid_rgb[1]);

    w = u8g2.drawStr(11, 56, "B:");
    u8g2.drawBox(16 + w, 46, 70, 10);
    u8g2.setDrawColor(0);
    u8g2.drawBox(17 + w, 47, 68, 8);
    u8g2.setDrawColor(1);
    u8g2.drawBox(17 + w, 47, 68*(this->setting->lighting_solid_rgb[2]/255.0), 8);
    u8g2.setCursor(90 + w, 56);
    u8g2.print(this->setting->lighting_solid_rgb[2]);

    for (byte i = 0; i < 3; i++){
        if (i == this->sel_pos){
            u8g2.drawStr(1, 26 + (i*15), "*");
        } else if (i == this->pos){
            u8g2.drawStr(1, 26 + (i*15), ">");
        }
    }
}

void Screen_RGBSelect::button_pressed(byte b){
    switch (this->sel_pos){
        case 0:
            this->button_first(b);
            break;
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
                    screen_num = 1;
                    screen_update = true;
                    // reset light mode
                    break;
                case 3:
                    // OK
                    this->sel_pos = this->pos;
                    screen_update = true;
                    break;
            }
    }
}

void Screen_RGBSelect::button_first(byte b){
    switch (b){
        case 0:
            // Up
            if (this->setting->lighting_solid_rgb[0] < 255){
                this->setting->lighting_solid_rgb[0]++;
            screen_update = true;
            }
            break;
        case 1:
            // Down
            if (this->setting->lighting_solid_rgb[0] > 0){
                this->setting->lighting_solid_rgb[0]--;
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

void Screen_RGBSelect::button_second(byte b){
    switch (b){
        case 0:
            // Up
            if (this->setting->lighting_solid_rgb[1] < 255){
                this->setting->lighting_solid_rgb[1]++;
            screen_update = true;
            }
            break;
        case 1:
            // Down
            if (this->setting->lighting_solid_rgb[1] > 0){
                this->setting->lighting_solid_rgb[1]--;
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

void Screen_RGBSelect::button_third(byte b){
    switch (b){
        case 0:
            // Up
            if (this->setting->lighting_solid_rgb[2] < 255){
                this->setting->lighting_solid_rgb[2]++;
            screen_update = true;
            }
            break;
        case 1:
            // Down
            if (this->setting->lighting_solid_rgb[2] > 0){
                this->setting->lighting_solid_rgb[2]--;
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