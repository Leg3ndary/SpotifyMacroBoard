
#include <FastLED.h>

#define RGB_PIN A0        // LED DATA PIN
#define RGB_LED_NUM 300   // 10 LEDs [0...9]
#define BRIGHTNESS 200    // brightness range [0..255]
#define CHIP_SET WS2812B  // types of RGB LEDs
#define COLOR_CODE RGB    // sequence of colors in data stream

// Define the array of LEDs
CRGB LEDs[RGB_LED_NUM];

// define 3 byte for the random color
byte a, b, c;
#define UPDATES_PER_SECOND 100

char iByte = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("WS2812B LEDs strip Initialize");
    Serial.println(
        "Please enter the 1 to 6 value.....Otherwise no any effect show");
    FastLED.addLeds<CHIP_SET, RGB_PIN, COLOR_CODE>(LEDs, RGB_LED_NUM);
    randomSeed(analogRead(0));
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
    FastLED.clear();
    FastLED.show();
}

void loop() {
    if (Serial.available() > 0) {
        // read the incoming byte:

        iByte = Serial.read();

        switch (iByte) {
            case '1':
                Toggle_RED_LED();
                Serial.println("1.Toggle LED Complete");
                break;
            case '2':
                Scrolling_RED_LED();
                Serial.println("2.Scrolling RED LED Complete");
                break;
            case '3':
                O_W_G_scroll();
                Serial.println("3.Flag Show Complete");
                break;
            case '4':
                Rotate_color();
                Serial.println("4.Rotate color Complete");
                break;
            case '5':
                r_g_b();
                Serial.println("5.R_G_B color Complete");
                break;
            case '6':
                random_color();
                Serial.println("6.Random color Show Complete");
                break;
        }
    }
}

// RED LED TOGGLE
void Toggle_RED_LED(void) {
    // Red Green Blue
    for (int i = 0; i < RGB_LED_NUM; i++) LEDs[i] = CRGB(255, 0, 0);
    FastLED.show();
    delay(1000);
    for (int i = 0; i < RGB_LED_NUM; i++) LEDs[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(1000);
}
// Move the Red LED
void Scrolling_RED_LED(void) {
    for (int i = 0; i < RGB_LED_NUM; i++) {
        LEDs[i] = CRGB::Red;
        FastLED.show();
        delay(500);
        LEDs[i] = CRGB::Black;
        FastLED.show();
        delay(500);
    }
}
// Orange/White/Green color green
void O_W_G_scroll() {
    for (int i = 0; i < RGB_LED_NUM; i++) {
        LEDs[i] = CRGB::Orange;
        delay(50);
        FastLED.show();
    }
    for (int i = 0; i < RGB_LED_NUM; i++) {
        LEDs[i] = CRGB::Black;
        delay(50);
        FastLED.show();
    }
    for (int i = 0; i < RGB_LED_NUM; i++) {
        LEDs[i] = CRGB::White;
        delay(50);
        FastLED.show();
    }
    for (int i = 0; i < RGB_LED_NUM; i++) {
        LEDs[i] = CRGB::Black;
        delay(50);
        FastLED.show();
    }
    for (int i = 0; i < RGB_LED_NUM; i++) {
        LEDs[i] = CRGB::Green;
        delay(50);
        FastLED.show();
    }
    for (int i = 0; i < RGB_LED_NUM; i++) {
        LEDs[i] = CRGB::Black;
        delay(50);
        FastLED.show();
    }
}

// Red/Green/Blue color Rotate
void Rotate_color(void) {
    for (int clr = 0; clr < RGB_LED_NUM; clr++) {
        LEDs[clr] = CRGB::Red;
        LEDs[clr + 1] = CRGB::Green;
        LEDs[clr + 2] = CRGB::Blue;
        FastLED.show();
        delay(100);
        for (int clr = 0; clr < RGB_LED_NUM; clr++) {
            LEDs[clr] = CRGB::Black;
            delay(5);
        }
    }
}
// Blue, Green , Red
void r_g_b() {
    for (int i = 0; i < RGB_LED_NUM; i++) {
        LEDs[i] = CRGB(0, 0, 255);
        FastLED.show();
        delay(50);
    }
    for (int i = RGB_LED_NUM; i >= 0; i--) {
        LEDs[i] = CRGB(0, 255, 0);
        FastLED.show();
        delay(50);
    }
    for (int i = 0; i < RGB_LED_NUM; i++) {
        LEDs[i] = CRGB(255, 0, 0);
        FastLED.show();
        delay(50);
    }
    for (int i = RGB_LED_NUM; i >= 0; i--) {
        LEDs[i] = CRGB(0, 0, 0);
        FastLED.show();
        delay(50);
    }
}
// random color show
void random_color(void) {
    // loop over the NUM_LEDS
    for (int i = 0; i < RGB_LED_NUM; i++) {
        // choose random value for the r/g/b
        a = random(0, 255);
        b = random(0, 255);
        c = random(0, 255);
        // Set the value to the led
        LEDs[i] = CRGB(a, b, c);
        // set the colors set into the physical LED
        FastLED.show();

        // delay 50 millis
        FastLED.delay(200);
    }
}