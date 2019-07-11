// Defining Pins
#define pin_snare 6
#define pin_high 7
#define pin_low 10
#define pin_floor 11
#define pin_bass 12

#define pin_red 4
#define pin_green 3
#define pin_blue 2

#define pzo_snare A0
#define pzo_high A1
#define pzo_low A2
#define pzo_floor A4
#define pzo_bass A3

// Defining settings
#define drum_bright_min 40.0
#define drum_bright_max 700.0
#define drum_bright_fix 200.0
#define drum_hit_fix 80.0
#define drum_idle_fix 110.0
#define update_ms 15
#define idle_ms 15000

// Define default colours
#define red 0
#define green 130
#define blue 250

// Declaring variables
const byte pin_out[5] = {pin_snare, pin_high, pin_low, pin_floor, pin_bass};
const byte pin_in[5] = {pzo_snare, pzo_high, pzo_low, pzo_floor, pzo_bass};
const int drum_hit_min[5] = {120,110,100,300,40};
int drum_level[5] = {0,0,0,0,0};
int piezo_level[5] = {0,0,0,0,0};

byte rgb_r = red;
byte rgb_g = green;
byte rgb_b = blue;

unsigned long dec_millis = 0;
unsigned long idle_millis = 0;

int idle_mode = -1;
byte idle_drum = 0;

void setup(){
    // Setup Pins
    for (byte i = 0; i < 5; i++){
        pinMode(pin_out[i], OUTPUT);
        pinMode(pin_in[i], INPUT);
    }
    pinMode(pin_red, OUTPUT);
    pinMode(pin_green, OUTPUT);
    pinMode(pin_blue, OUTPUT);

    // Set values
    analogWrite(pin_red, rgb_r);
    analogWrite(pin_green, rgb_g);
    analogWrite(pin_blue, rgb_b);
    for (byte i = 0; i < 5; i++){
        analogWrite(pin_out[i], 0);
    }

    dec_millis = millis();
    idle_millis = millis();
}

void loop(){
    // Idle
    if ((millis() - idle_millis) > idle_ms){
        if (idle_mode == -1){
            // Set variables
            rgb_r = 0;
            rgb_g = 0;
            rgb_b = 0;
            analogWrite(pin_red, rgb_r);
            analogWrite(pin_green, rgb_g);
            analogWrite(pin_blue, rgb_b);
            idle_mode = 0;
        }
        updateIdle();

        // Read values
        for (byte i = 0; i < 5; i++){
            int tmp = analogRead(pin_in[i]);
            piezo_level[i] = analogRead(pin_in[i]);
        }

        // If hit break out of idle mode
        for (byte i = 0; i < 5; i++){
            if (piezo_level[i] > drum_idle_fix){
                idle_millis = millis();
            }
        }

    // Not idle
    } else {
        if (rgb_r != red || rgb_g != green || rgb_b != blue){
            // Reset variables
            rgb_r = red;
            rgb_g = green;
            rgb_b = blue;
            analogWrite(pin_red, rgb_r);
            analogWrite(pin_green, rgb_g);
            analogWrite(pin_blue, rgb_b);
            idle_mode = -1;
            idle_drum = 0;

            for (byte i = 0; i < 5; i++){
                analogWrite(pin_out[i], 0);
            }

            idle_millis = millis();
        }

        // Actually the drumming part
        if ((millis() - dec_millis) > update_ms){
            // Read values
            for (byte i = 0; i < 5; i++){
                int tmp = analogRead(pin_in[i]);
                piezo_level[i] = analogRead(pin_in[i]);
            }

            // Determine which one hit
            byte idx = 6;
            int mx = 0;
            for (byte i = 0; i < 5; i++){
                if (piezo_level[i] > mx && piezo_level[i] > drum_hit_min[i]){
                    idx = i;
                    idle_millis = millis();
                }
            }

            // Write brightnesses
            if (idx != 6){
                drum_level[idx] = drum_bright_max;
            }
            for (byte i = 0; i < 5; i++){
                analogWrite(pin_out[i], byte(255 * (drum_level[i]/drum_bright_max)));
                if (drum_level[i] >= drum_bright_min){
                    drum_level[i] -= 20*exp(drum_level[i]/500);
                } else {
                    drum_level[i] = 0;
                }
            }

            dec_millis = millis();
        }
    }
}

void updateIdle(){
    if ((millis() - dec_millis) > update_ms){
        switch (idle_mode){
        case 0:
            // Off to red
            if (rgb_r == 0){
                digitalWrite(pin_out[idle_drum], HIGH);
            }
            if (rgb_r < 150){
                rgb_r++;
                analogWrite(pin_red, rgb_r);
            } else {
                idle_mode++;
            }
            break;
        case 1:
            // Red to Yellow
            if (rgb_g < 150){
                rgb_g++;
                analogWrite(pin_green, rgb_g);
            } else {
                idle_mode++;
            }
            break;
        case 2:
            // Yellow to Green
            if (rgb_r > 0){
                rgb_r--;
                analogWrite(pin_red, rgb_r);
            } else {
                idle_mode++;
            }
            break;
        case 3:
            // Green to Cyan
            if (rgb_b < 150){
                rgb_b++;
                analogWrite(pin_blue, rgb_b);
            } else {
                idle_mode++;
            }
            break;
        case 4:
            // Cyan to Blue
            if (rgb_g > 0){
                rgb_g--;
                analogWrite(pin_green, rgb_g);
            } else {
                idle_mode++;
            }
            break;
        case 5:
            // Blue to Purple
            if (rgb_r < 150){
                rgb_r++;
                analogWrite(pin_red, rgb_r);
            } else {
                idle_mode++;
            }
            break;
        case 6:
            // Purple to Red
            if (rgb_b > 0){
                rgb_b--;
                analogWrite(pin_blue, rgb_b);
            } else {
                idle_mode++;
            }
            break;
        case 7:
            // Red to Off
            if (rgb_r > 0){
                rgb_r--;
                analogWrite(pin_red, rgb_r);
            } else {
                idle_mode = 0;
                digitalWrite(pin_out[idle_drum], LOW);
                idle_drum++;
                if (idle_drum > 4){
                    idle_drum = 0;
                }
            }
            break;
        }

        dec_millis = millis();
    }
}