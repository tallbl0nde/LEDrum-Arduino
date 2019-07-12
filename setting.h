#ifndef SETTING_H
#define SETTING_H

// Struct to hold settings
struct settings {
    int lighting_mode;
    int lighting_solid_rgb[3];
    int lighting_fade_speed;
    int lighting_shift_speed;
    int idle_timeout;
    int idle_mode;
    int idle_animate_speed;
    int sensitivity[5];
    int backlight_brightness;
};

#endif