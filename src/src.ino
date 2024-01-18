/*
Sketch uses 886189 bytes (67%) of program storage space. Maximum is 1310720
bytes. Global variables use 45808 bytes (13%) of dynamic memory, leaving 281872
bytes for local variables. Maximum is 327680 bytes.
*/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include <SMBCredentials.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

WiFiClientSecure wifiClient;
TwoWire I2C = TwoWire(0);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C, OLED_RESET);

const int keys[7] = {SHUFFLE, VOLUME_DEC, VOLUME_INC, LOOP,
                     BACK,    PAUSE_PLAY, SKIP};
bool keyPrevState[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool keyState[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

const unsigned long RSSIDelay = 10000;
const unsigned long currentDelay = 5000;
const unsigned long timeDelay = 1000;
const unsigned long fadeDelay = 4;
unsigned long nextRSSICheck, nextCurrentCheck, nextTimeCheck, nextFade;

String title, artist, album, color, durationRaw, progressRaw, pausedRaw,
    volumeRaw, lastTitle;
int progress, duration, volume, lastVolume;
bool paused;

int rssi = 0;

CRGB LEDs[RGB_LED_NUM];
int red, blue;
int green = 255;
bool shouldFade = false;
int prevRed, prevGreen, prevBlue;
int step;

void setup() {
    for (int i = 0; i < 7; i++) {
        pinMode(keys[i], INPUT_PULLUP);
    }

    pinMode(LED, OUTPUT);
    pinMode(SCL, INPUT_PULLUP);
    pinMode(SDA, INPUT_PULLUP);

    delay(2000);

    FastLED.addLeds<CHIP_SET, LED, COLOR_CODE>(LEDs, RGB_LED_NUM);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);

    for (int i = 0; i < RGB_LED_NUM; i++) LEDs[i] = CRGB::White;
    FastLED.show();

    I2C.begin(SDA, SCL, 400000);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        for (int i = 0; i < RGB_LED_NUM; i++) {
            LEDs[i] = CRGB::OrangeRed;
        }
        FastLED.show();
        for (;;) {
            ;
        }
    }
    delay(2000);
    display.clearDisplay();

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, SSID_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        for (int times = 0; times < 20; times++) {
            for (int i = 0; i < RGB_LED_NUM; i++) {
                byte brightness = 140 + 110 * sin(millis() / 250.0);
                LEDs[i] = CRGB::White;
                LEDs[i].fadeToBlackBy(255 - brightness);
            }
            delay(25);
            FastLED.show();
        }
    }
    for (int step = 0; step < 256; step++) {
        for (int i = 0; i < RGB_LED_NUM; i++) {
            LEDs[i] = blend(CRGB::White, CRGB::Green, step);
        }
        delay(3);
        FastLED.show();
    }

    wifiClient.setCACert(benzServerCert);
}

void updateScreen() {
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
    for (int i = 18; i < 64; i++) {
        for (int j = 0; j < 16; j++) {
            display.drawPixel(i, j, WHITE);
        }
    }

    if (rssi == 0) {
        display.drawBitmap(2, 0, wifi_0, 16, 16, WHITE);
    } else if (rssi < -70) {
        display.drawBitmap(2, 0, wifi_1, 16, 16, WHITE);
    } else {
        display.drawBitmap(2, 0, wifi_2, 16, 16, WHITE);
    }

    for (int i = 64; i < 108; i++) {
        for (int j = 0; j < 16; j++) {
            display.drawPixel(i, j, WHITE);
        }
    }
    for (int i = 108; i < 124; i++) {
        for (int j = 0; j < 16; j++) {
            display.drawPixel(i, j, BLACK);
        }
    }
    if (volume > 66) {
        display.drawBitmap(108, 0, volume_3, 16, 16, WHITE);
    } else if (volume > 33) {
        display.drawBitmap(108, 0, volume_2, 16, 16, WHITE);
    } else if (volume > 0) {
        display.drawBitmap(108, 0, volume_1, 16, 16, WHITE);
    } else {
        display.drawBitmap(108, 0, volume_0, 16, 16, WHITE);
    }
    for (int i = 124; i < 128; i++) {
        for (int j = 0; j < 16; j++) {
            display.drawPixel(i, j, WHITE);
        }
    }

    display.display();
}

void extractValue(const String& key, const String& json, String& result) {
    String quoteKey = "\"" + key + "\":";
    int start = json.indexOf(quoteKey);
    if (start != -1) {
        int end = json.indexOf(",", start + quoteKey.length());
        if (end == -1) {
            end = json.indexOf("}", start + quoteKey.length());
        }
        result = json.substring(start + quoteKey.length(), end);
        result.trim();
        result.remove(0, 1);
        result.remove(result.length() - 1);
    }
}

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
    if (!wifiClient.connected()) {
        if (!wifiClient.connect("benzhou.tech", 443)) {
            for (int i = 0; i < RGB_LED_NUM; i++) LEDs[i] = CRGB::Red;
            FastLED.show();
            return;
        }
        yield();
    }
    wifiClient.print("GET /api/manageState/" + PASSWORD + "/" + actionString +
                     " HTTP/1.1\r\n" + "Host: benzhou.tech\r\n" +
                     "Connection: Keep-Alive\r\n\r\n");
    wifiClient.flush();
}

void updateCurrent() {
    if (!wifiClient.connected()) {
        if (!wifiClient.connect("benzhou.tech", 443)) {
            for (int i = 0; i < RGB_LED_NUM; i++) LEDs[i] = CRGB::Red;
            FastLED.show();
            return;
        }
        yield();
    }

    wifiClient.print("GET /api/getCurrent/" + PASSWORD + " HTTP/1.1\r\n" +
                     "Host: benzhou.tech\r\n" +
                     "Connection: Keep-Alive\r\n\r\n");

    unsigned long timeout = millis();
    while (!wifiClient.available()) {
        if (millis() - timeout > 5000) {
            for (int i = 0; i < RGB_LED_NUM; i++) LEDs[i] = CRGB::Yellow;
            FastLED.show();
            return;
        }
    }
    yield();

    char endOfHeaders[] = "\r\n\r\n";
    if (!wifiClient.find(endOfHeaders)) {
        for (int i = 0; i < RGB_LED_NUM; i++) LEDs[i] = CRGB::Orange;
        FastLED.show();
        return;
    }
    wifiClient.find("{\"ti");

    String response = "{\"ti";
    while (wifiClient.available()) {
        char c = wifiClient.read();
        response += c;
    }
    wifiClient.flush();

    lastTitle = title;
    extractValue("title", response, title);
    extractValue("artist", response, artist);
    extractValue("album", response, album);
    extractValue("duration", response, durationRaw);
    extractValue("progress", response, progressRaw);
    extractValue("paused", response, pausedRaw);
    extractValue("volume", response, volumeRaw);

    paused = pausedRaw == "true";
    lastVolume = volume;
    volume = volumeRaw.toInt();
    duration = durationRaw.toInt();
    progress = progressRaw.toInt();

    if (title != lastTitle) {
        color = response.substring(response.indexOf("[") + 1,
                                   response.indexOf("]"));

        int comma1 = color.indexOf(",");
        int comma2 = color.indexOf(",", comma1 + 1);

        shouldFade = true;
        prevRed = red;
        prevGreen = green;
        prevBlue = blue;

        red = color.substring(0, comma1 + 1).toInt();
        green = color.substring(comma1 + 1, comma2 + 1).toInt();
        blue = color.substring(comma2 + 1, color.length()).toInt();

        display.setTextColor(WHITE);
        display.setTextWrap(false);
        display.clearDisplay();
        updateScreen();
        display.setCursor(0, 16);
        display.setTextSize(2);
        display.println(title);
        display.setTextSize(1);
        display.println(artist);
    }

    display.display();
}

bool fadeLED() {
    if (step > 255) {
        step = 0;
        return false;
    }
    for (int i = 0; i < RGB_LED_NUM; i++) {
        LEDs[i] = blend(CRGB(prevRed, prevGreen, prevBlue),
                        CRGB(red, green, blue), step);
    }
    FastLED.show();
    step += 1;
    return true;
}

void updateTime(int total, int current) {
    display.fillRect(0, 48, 128, 16, BLACK);

    if (current > total) {
        current = total;
    }

    int barWidth = 100;
    int barHeight = 6;
    int barX = 14;
    int barY = 56;

    display.drawRect(barX, barY, barWidth, barHeight, WHITE);
    if (total <= 0) {
        total = 1;
    }
    int percent = (100 * current) / total;

    int progressWidth = (barWidth * percent) / 100;
    display.fillRect(barX, barY, progressWidth, barHeight, WHITE);

    int currentMinutes = current / 60;
    int currentSeconds = current % 60;
    int totalMinutes = total / 60;
    int totalSeconds = total % 60;

    display.setTextSize(1);
    display.setCursor(32, 48);
    display.print(currentMinutes);
    display.print(":");
    if (currentSeconds < 10) display.print("0");
    display.print(currentSeconds);
    display.print("/");
    display.print(totalMinutes);
    display.print(":");
    if (totalSeconds < 10) display.print("0");
    display.println(totalSeconds);

    display.display();
}

void shuffle() { updateState('f'); }

void volumeDecrease() {
    updateState('v', 0);
    updateScreen();
}

void volumeIncrease() {
    updateState('v', 1);
    updateScreen();
}

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
        updateScreen();
    }

    if (millis() > nextCurrentCheck) {
        nextCurrentCheck = millis() + currentDelay;
        updateCurrent();
    }

    if (millis() > nextTimeCheck) {
        nextTimeCheck = millis() + timeDelay;
        updateTime(duration, progress);
        if (!paused) {
            progress++;
        }
    }

    if (shouldFade && millis() > nextFade) {
        nextFade = millis() + fadeDelay;
        shouldFade = fadeLED();
    }
}