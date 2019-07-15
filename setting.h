#ifndef SETTING_H
#define SETTING_H

// Struct to hold settings
struct settings {
    byte lighting_mode;
    byte lighting_solid_rgb[3];
    byte lighting_fade_speed;
    byte lighting_shift_speed;
    int idle_timeout;
    int idle_animate_speed;
    int sensitivity[5];
    byte backlight_brightness;
};

#endif