#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include <SPI.h>
#include <SpotifyCredentials.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

#define RGB_PIN 18
#define RGB_LED_NUM 20
#define BRIGHTNESS 230
#define CHIP_SET WS2812B
#define COLOR_CODE GRB
#define UPDATES_PER_SECOND 100
#define SHUFFLE 4
#define VOLUME_DEC 5
#define VOLUME_INC 12
#define LOOP 13
#define BACK 14
#define PAUSE_PLAY 25
#define SKIP 26
#define LED 18
#define SCL 19
#define SDA 21
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// 'wifi-logo', 16x16px
const unsigned char wifi_logo[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xc1, 0x83, 0x9f, 0xf1, 0x38,
    0x1c, 0xe1, 0x87, 0xcf, 0xf3, 0xd8, 0x1b, 0xf1, 0x8f, 0xf3, 0xcf,
    0xfe, 0x7f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff};

// 'wifi-logo2', 16x16px
const unsigned char wifi_logo2[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8,
    0x1f, 0xe1, 0x87, 0xcf, 0xf3, 0xd8, 0x1b, 0xf1, 0x8f, 0xf3, 0xcf,
    0xfe, 0x7f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff};

const unsigned char wifi_logo3[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xf1, 0x8f, 0xf3, 0xcf,
    0xfe, 0x7f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff};

WiFiClientSecure bClient;
TwoWire I2CBME = TwoWire(0);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2CBME, OLED_RESET);

bool shuffleState = false;
byte repeatState = 0;
int rssi = 0;

const int keys[7] = {SHUFFLE, VOLUME_DEC, VOLUME_INC, LOOP,
                     BACK,    PAUSE_PLAY, SKIP};
bool keyPrevState[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool keyState[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

// Define the array of LEDs
CRGB LEDs[RGB_LED_NUM];

// define 3 byte for the random color
byte a, b, c;

// WIFI STUFF
unsigned long nextRSSICheck = 0;
unsigned long RSSIDelay = 10000;
int lastRSSI = 0;

// Curret check
unsigned long nextCurrentCheck = 0;
unsigned long currentDelay = 5000;

void setup() {
    for (int i = 0; i < 7; i++) {
        pinMode(keys[i], INPUT_PULLUP);
    }

    pinMode(LED, OUTPUT);
    pinMode(SCL, INPUT_PULLUP);
    pinMode(SDA, INPUT_PULLUP);

    Serial.begin(115200);
    delay(2000);
    I2CBME.begin(SDA, SCL, 400000);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;  // Don't proceed, loop forever
    }
    delay(2000);

    display.clearDisplay();

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, SSID_PASS);

    FastLED.addLeds<CHIP_SET, LED, COLOR_CODE>(LEDs, RGB_LED_NUM);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
    FastLED.clear();
    FastLED.show();

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    display.println("Connected to WiFi");
    display.display();

    bClient.setCACert(benzServerCert);

    // Finished All of the setup, now acc say starting
    delay(2000);
    display.clearDisplay();
    drawScreenOutline();
    display.setCursor(0, 16);
    display.setTextSize(2);
    display.println("  Starting  ");
    display.display();
    updateCurrent();
}

void drawScreenOutline() {
    if (lastRSSI == rssi) return;
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.setTextWrap(false);

    display.setTextSize(2);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 16; j++) {
            display.drawPixel(i, j, WHITE);
        }
    }
    for (int i = 2; i < 18; i++) {
        for (int j = 0; j < 16; j++) {
            display.drawPixel(i, j, BLACK);
        }
    }
    for (int i = 18; i < 128; i++) {
        for (int j = 0; j < 16; j++) {
            display.drawPixel(i, j, WHITE);
        }
    }

    if (rssi == 0) {
        display.drawBitmap(2, 0, wifi_logo3, 16, 16, WHITE);
    } else if (rssi < -70) {
        display.drawBitmap(2, 0, wifi_logo2, 16, 16, WHITE);
    } else {
        display.drawBitmap(2, 0, wifi_logo, 16, 16, WHITE);
    }

    display.display();
}

void extractValue(const String& key, const String& json, String& result) {
    String keyWithQuotes = "\"" + key + "\":";
    int start = json.indexOf(keyWithQuotes);
    if (start != -1) {
        int end = json.indexOf(",", start + keyWithQuotes.length());
        if (end == -1) {
            end = json.indexOf("}", start + keyWithQuotes.length());
        }
        result = json.substring(start + keyWithQuotes.length(), end);
        result.trim();
        result.remove(0, 1);
        result.remove(result.length() - 1);
    }
    yield();
}

/* Actions are as follows
p = playPause
b = back
s = skip
r = repeat
v = volume
    0 = decrease
    1 = increase
l = loop
f = shuffle
*/
void updateState(char action, int subAction = 0) {
    String actionString = "";
    if (action == 'p') {
        actionString = "playPause";
    } else if (action == 'b') {
        actionString = "back";
    } else if (action == 's') {
        actionString = "skip";
    } else if (action == 'r') {
        actionString = "loop";
    } else if (action == 'v') {
        if (subAction == 0) {
            actionString = "vdec";
        } else {
            actionString = "vinc";
        }
    } else if (action == 'l') {
        actionString = "loop";
    } else if (action == 'f') {
        actionString = "shuffle";
    }
    if (!bClient.connected()) {
        if (!bClient.connect("benzhou.tech", 443)) {
            Serial.println("Connection failed");
            return;
        }
        yield();
    }
    bClient.print("GET /api/manageState/" + PASSWORD + "/" + actionString +
                  " HTTP/1.1\r\n" + "Host: benzhou.tech\r\n" +
                  "Connection: Keep-Alive\r\n\r\n");
    bClient.flush();
}

void updateCurrent() {
    if (!bClient.connected()) {
        if (!bClient.connect("benzhou.tech", 443)) {
            Serial.println("Connection failed");
            return;
        }
        yield();
    }

    bClient.print("GET /api/getCurrent/" + PASSWORD + " HTTP/1.1\r\n" +
                  "Host: benzhou.tech\r\n" + "Connection: Keep-Alive\r\n\r\n");

    unsigned long timeout = millis();
    while (!bClient.available()) {
        if (millis() - timeout > 5000) {
            Serial.println("No response");
            return;
        }
    }
    yield();

    char endOfHeaders[] = "\r\n\r\n";
    if (!bClient.find(endOfHeaders)) {
        Serial.println("Invalid response");
        return;
    }
    bClient.find("{\"title");

    String response = "{\"title";
    while (bClient.available()) {
        char c = bClient.read();
        response += c;
    }
    bClient.flush();

    String title, artist, album, color, duration, progress, pausedRaw,
        volumeRaw, repeatRaw, shuffleRaw;

    extractValue("title", response, title);
    extractValue("artist", response, artist);
    extractValue("album", response, album);
    extractValue("duration", response, duration);
    extractValue("progress", response, progress);
    extractValue("paused", response, pausedRaw);
    extractValue("volume", response, volumeRaw);
    extractValue("loop", response, repeatRaw);
    extractValue("shuffle", response, shuffleRaw);

    bool paused = pausedRaw == "true";
    int volume = volumeRaw.toInt();
    bool shuffle = shuffleRaw == "true";

    color =
        response.substring(response.indexOf("[") + 1, response.indexOf("]"));

    int commaIndex1 = color.indexOf(",");
    int commaIndex2 = color.indexOf(",", commaIndex1 + 1);

    int red = color.substring(0, commaIndex1 + 1).toInt();
    int green = color.substring(commaIndex1 + 1, commaIndex2 + 1).toInt();
    int blue = color.substring(commaIndex2 + 1, color.length()).toInt();

    for (int i = 0; i < RGB_LED_NUM; i++) LEDs[i] = CRGB(red, green, blue);
    FastLED.show();

    display.clearDisplay();
    drawScreenOutline();
    display.setTextColor(WHITE);
    display.setCursor(0, 16);
    display.setTextSize(2);
    display.println(title);
    display.setTextSize(1);
    display.println(artist);
    delay(500);
    display.display();
    display.startscrollright(2, 5);
    // bClient.stop();
}

void shuffle() { updateState('f'); }

void volumeDecrease() { updateState('v', 0); }

void volumeIncrease() { updateState('v', 1); }

void repeat() { updateState('r'); }

void back() {
    updateState('b');
    nextCurrentCheck = millis() + 100;
}

void pausePlay() {
    updateState('p');
    nextCurrentCheck = millis() + 100;
}

void skip() {
    updateState('s');
    nextCurrentCheck = millis() + 100;
    updateCurrent();
}

void (*funcs[7])() = {
    shuffle, volumeDecrease, volumeIncrease, repeat, back, pausePlay, skip};

void loop() {
    for (int i = 0; i < 7; i++) {
        keyState[i] = digitalRead(keys[i]);
        if (keyState[i] == LOW && keyPrevState[i] == HIGH) {
            funcs[i]();
        }
        keyPrevState[i] = keyState[i];
    }

    if (millis() > nextRSSICheck) {
        nextRSSICheck = millis() + RSSIDelay;
        rssi = WiFi.RSSI();
        drawScreenOutline();
    }

    if (millis() > nextCurrentCheck) {
        nextCurrentCheck = millis() + currentDelay;
        updateCurrent();
    }
}