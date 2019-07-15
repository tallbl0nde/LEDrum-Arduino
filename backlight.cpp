#include "backlight.h"

// Need to make this less dodgy
extern int screen_num;
extern bool screen_update;
extern void setBacklight(byte);

Screen_Backlight::Screen_Backlight(struct settings * s) : Screen(s){
    this->selected = false;
}

void Screen_Backlight::draw(U8G2_ST7920_128X64_F_HW_SPI u8g2){
    u8g2.setFont(u8g2_font_crox2hb_tr);
    u8g2.drawStr(1, 11, "Backlight:");
    u8g2.setFont(u8g2_font_rosencrantz_nbp_tr);
    int w = u8g2.drawStr(11, 26, "Brightness (%): ");
    u8g2.setCursor(11 + w, 26);
    u8g2.print(this->setting->backlight_brightness);
    if (this->selected){
        u8g2.drawStr(1, 26, "*");
    } else {
        u8g2.drawStr(1, 26, ">");
    }
}

void Screen_Backlight::button_pressed(byte b){
    switch (b){
        case 0:
            // Up
            if (this->selected && this->setting->backlight_brightness < 100){
                setBacklight(this->setting->backlight_brightness+1);
                screen_update = true;
            }

            break;
        case 1:
            // Down
            if (this->selected && this->setting->backlight_brightness > 0){
                setBacklight(this->setting->backlight_brightness-1);
                screen_update = true;
            }
            break;
        case 2:
            // Back
            (this->selected) ? this->selected = false : screen_num = 0;
            screen_update = true;
            break;
        case 3:
            // OK
            (this->selected) ? this->selected = false : this->selected = true;
            screen_update = true;
            break;
    }
}