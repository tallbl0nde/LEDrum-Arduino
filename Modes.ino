/* #region Idle Shift */
byte idle_drum;
byte idle_state;

void setup_idle_shift(){
    writeRGB(0, 0, 0);
    allOff();
    idle_drum = 0;
    idle_state = 0;
}

void loop_idle_shift(){
    if ((millis() - fade_ms) > setting.idle_animate_speed){
        // Transition through all colours
        switch (idle_state){
            case 0:
                digitalWrite(pin_out[idle_drum], HIGH);
                // Off to Red
                if (rgb[0] < 255){
                    writeRGB(rgb[0] + 5, rgb[1], rgb[2]);
                } else {
                    idle_state++;
                }
                break;
            case 1:
                // Red to Yellow
                if (rgb[1] < 255){
                    writeRGB(rgb[0], rgb[1] + 1, rgb[2]);
                } else {
                    idle_state++;
                }
                break;
            case 2:
                // Yellow to Green
                if (rgb[0] > 0){
                    writeRGB(rgb[0] - 1, rgb[1], rgb[2]);
                } else {
                    idle_state++;
                }
                break;
            case 3:
                // Green to Cyan
                if (rgb[2] < 255){
                    writeRGB(rgb[0], rgb[1], rgb[2] + 1);
                } else {
                    idle_state++;
                }
                break;
            case 4:
                // Cyan to Blue
                if (rgb[1] > 0){
                    writeRGB(rgb[0], rgb[1] - 1, rgb[2]);
                } else {
                    idle_state++;
                }
                break;
            case 5:
                // Blue to Purple
                if (rgb[0] < 255){
                    writeRGB(rgb[0] + 1, rgb[1], rgb[2]);
                } else {
                    idle_state++;
                }
                break;
            case 6:
                // Purple to Red
                if (rgb[2] > 0){
                    writeRGB(rgb[0], rgb[1], rgb[2]- 1);
                } else {
                    idle_state++;
                }
                break;
            case 7:
                // Red to Off
                if (rgb[0] > 0){
                    writeRGB(rgb[0] - 5, rgb[1], rgb[2]);
                } else {
                     idle_state = 0;
                     digitalWrite(pin_out[idle_drum], LOW);
                     idle_drum++;
                     if (idle_drum > 4){
                         idle_drum = 0;
                     }
                }
                break;
        }
        fade_ms = millis();
    }

    // Read values
    for (byte i = 0; i < 5; i++){
        int tmp = analogRead(pin_in[i]);
        piezo_level[i] = analogRead(pin_in[i]);
    }

    // If hit break out of idle mode
    for (byte i = 0; i < 5; i++){
        if (piezo_level[i] > setting.miscellaneous[4]){
            idle_ms = millis();
            is_idle = false;
        }
    }
}
/* #endregion */

/* #region Solid Colour */
void setup_drum_solid(){
    writeRGB(setting.lighting_solid_rgb[0], setting.lighting_solid_rgb[1], setting.lighting_solid_rgb[2]);
    allOff();
}

void loop_drum_solid(){
    if ((millis() - fade_ms) >= setting.lighting_fade_speed){
        // Read values
        for (byte i = 0; i < 5; i++){
            int tmp = analogRead(pin_in[i]);
            piezo_level[i] = analogRead(pin_in[i]);
        }

        // Determine which one hit
        byte idx = 6;
        int mx = 0;
        for (byte i = 0; i < 5; i++){
            if (piezo_level[i] > mx && piezo_level[i] > setting.sensitivity[i]){
                idx = i;
                idle_ms = millis();
            }
        }

        // Set brightnesses
        if (idx != 6){
            drum_level[idx] = setting.miscellaneous[1];
        }
        for (byte i = 0; i < 5; i++){
            analogWrite(pin_out[i], byte(255 * (drum_level[i] / setting.miscellaneous[1])));
            if (drum_level[i] >= setting.miscellaneous[0]){
                drum_level[i] -= 20 * exp(drum_level[i] / 500);
            } else {
                drum_level[i] = 0;
            }
        }

        fade_ms = millis();
    }
}
/* #endregion */

/* #region RGB Shift */
byte shift_state;

void setup_drum_rgb(){
    writeRGB(255, 0, 0);
    allOff();
    shift_state = 0;
}

void loop_drum_rgb(){
    // Set colour
    if ((millis() - shift_ms) > setting.lighting_shift_speed){
        // Transition through all colours
        switch (shift_state){
            case 0:
                // Red to Yellow
                if (rgb[1] < 255){
                    writeRGB(rgb[0], rgb[1] + 1, rgb[2]);
                } else {
                    shift_state++;
                }
                break;
            case 1:
                // Yellow to Green
                if (rgb[0] > 0){
                    writeRGB(rgb[0] - 1, rgb[1], rgb[2]);
                } else {
                    shift_state++;
                }
                break;
            case 2:
                // Green to Cyan
                if (rgb[2] < 255){
                    writeRGB(rgb[0], rgb[1], rgb[2] + 1);
                } else {
                    shift_state++;
                }
                break;
            case 3:
                // Cyan to Blue
                if (rgb[1] > 0){
                    writeRGB(rgb[0], rgb[1] - 1, rgb[2]);
                } else {
                    shift_state++;
                }
                break;
            case 4:
                // Blue to Purple
                if (rgb[0] < 255){
                    writeRGB(rgb[0] + 1, rgb[1], rgb[2]);
                } else {
                    shift_state++;
                }
                break;
            case 5:
                // Purple to Red
                if (rgb[2] > 0){
                    writeRGB(rgb[0], rgb[1], rgb[2]- 1);
                } else {
                    shift_state = 0;
                }
                break;
        }

        shift_ms = millis();
    }

    if ((millis() - fade_ms) >= setting.lighting_fade_speed){
        // Read values
        for (byte i = 0; i < 5; i++){
            int tmp = analogRead(pin_in[i]);
            piezo_level[i] = analogRead(pin_in[i]);
        }

        // Determine which one hit
        byte idx = 6;
        int mx = 0;
        for (byte i = 0; i < 5; i++){
            if (piezo_level[i] > mx && piezo_level[i] > setting.sensitivity[i]){
                idx = i;
                idle_ms = millis();
            }
        }

        // Set brightnesses
        if (idx != 6){
            drum_level[idx] = setting.miscellaneous[1];
        }
        for (byte i = 0; i < 5; i++){
            analogWrite(pin_out[i], byte(255 * (drum_level[i] / setting.miscellaneous[1])));
            if (drum_level[i] >= setting.miscellaneous[0]){
                drum_level[i] -= 20 * exp(drum_level[i] / 500);
            } else {
                drum_level[i] = 0;
            }
        }

        fade_ms = millis();
    }
}
/* #endregion */

/* #region Solid RGB Select */
void setup_drum_select(){
    writeRGB(setting.lighting_solid_rgb[0], setting.lighting_solid_rgb[1], setting.lighting_solid_rgb[2]);
    allOff();
    analogWrite(pin_out[0], 255);
}

void loop_drum_select(){
    if ((millis() - fade_ms) >= setting.lighting_fade_speed){
        if (rgb[0] != setting.lighting_solid_rgb[0] || rgb[1] != setting.lighting_solid_rgb[1] || rgb[2] != setting.lighting_solid_rgb[2]){
            writeRGB(setting.lighting_solid_rgb[0], setting.lighting_solid_rgb[1], setting.lighting_solid_rgb[2]);
        }
        fade_ms = millis();

        // Need this to prevent entering idle mode
        idle_ms = millis();
    }
}
/* #endregion */