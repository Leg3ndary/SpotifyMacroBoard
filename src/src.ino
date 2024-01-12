/*
Plans:
On any button press map to a func

Have screen update on any button press

Have LED's animate and react to button presses

On each cycle do three things
1. Check buttons
2. Update screen
3. Update Led's
*/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <SPI.h>
#include <SpotifyArduino.h>
#include <SpotifyArduinoCert.h>
#include <SpotifyCredentials.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

#define RGB_PIN 18
#define RGB_LED_NUM 20
#define BRIGHTNESS 230  // brightness range [0..255]
#define CHIP_SET WS2812B
#define COLOR_CODE GRB  // Enum
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

const unsigned char wifi_logo3 [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf8, 0x1f, 0xf1, 0x8f, 0xf3, 0xcf, 0xfe, 0x7f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff
};

WiFiClientSecure sClient;
WiFiClientSecure bClient;
TwoWire I2CBME = TwoWire(0);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2CBME, OLED_RESET);

SpotifyArduino spotify(sClient, spotifyId, spotifySecret,
                       SPOTIFY_REFRESH_TOKEN);

unsigned long delayBetweenRequests = 10000;
unsigned long requestDueTime;

int volume = 50;
bool shuffleState = false;
byte repeatState = 0;
bool playerState = false;
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

void setup() {
    for (int i = 0; i < 7; i++) {
        pinMode(keys[i], INPUT_PULLUP);
    }

    pinMode(LED, OUTPUT);
    pinMode(SCL, INPUT_PULLUP);
    pinMode(SDA, INPUT_PULLUP);

    Serial.begin(115200);
    delay(1500);
    I2CBME.begin(SDA, SCL, 400000);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;  // Don't proceed, loop forever
    }

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

    sClient.setCACert(spotify_server_cert);
    bClient.setCACert(benzServerCert);

    if (!spotify.refreshAccessToken()) {
        Serial.println("Failed to get access tokens");
    }
}

void drawScreenOutline() {
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.setTextWrap(false);

    display.setTextSize(2);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 16; j++) {
            display.drawPixel(i, j, WHITE);
        }
    }
    // for (int i = 2; i < 18; i++) {
    //     for (int j = 0; j < 16; j++) {
    //         display.drawPixel(i, j, BLACK);
    //     }
    // }
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

void getColor(CurrentlyPlaying current) {
    if (!bClient.connect("benzhou.tech", 443)) {
        Serial.println("Connection failed");
        return;
    }

    String albumImageUrl = current.albumImages[0].url;
    int lastIndex = albumImageUrl.lastIndexOf('/');
    String imageUrl = albumImageUrl.substring(lastIndex + 1);

    String url = "/api/getColor/";
    url += imageUrl;

    bClient.print("GET " + url + " HTTP/1.1\r\n" + "Host: benzhou.tech\r\n" +
                  "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (!bClient.available()) {
        if (millis() - timeout > 5000) {
            Serial.println("No response");
            return;
        }
    }

    char endOfHeaders[] = "\r\n\r\n";
    if (!bClient.find(endOfHeaders)) {
        Serial.println("Invalid response");
        return;
    }

    String response = "";
    while (bClient.available()) {
        char c = bClient.read();
        response += c;
    }
    // Serial.println(response);
    bClient.flush();

    display.clearDisplay();
    drawScreenOutline();
    display.setCursor(0, 16);
    display.setTextSize(2);
    display.println(current.trackName);
    display.setTextSize(1);
    display.println(current.artists[0].artistName);
    display.display();
    display.startscrollright(2, 5);

    int bracketIndex = response.indexOf('[');
    int firstCommaIndex = response.indexOf(',');
    int secondCommaIndex = response.indexOf(',', firstCommaIndex + 1);

    int red = response.substring(bracketIndex + 1, firstCommaIndex).toInt();
    int green =
        response.substring(firstCommaIndex + 1, secondCommaIndex).toInt();
    int blue =
        response.substring(secondCommaIndex + 1, response.length() - 1).toInt();
    for (int i = 0; i < RGB_LED_NUM; i++) LEDs[i] = CRGB(red, green, blue);
    FastLED.show();
}

void shuffle() {
    shuffleState = !shuffleState;
    spotify.toggleShuffle(shuffleState);
}

void volumeDecrease() {
    volume = max(volume - 5, 0);
    spotify.setVolume(volume);
}

void volumeIncrease() {
    volume = min(volume + 5, 100);
    spotify.setVolume(volume);
}

void repeat() {
    repeatState = (repeatState + 1) % 3;
    if (repeatState == 0) {
        spotify.setRepeatMode(repeat_off);
    } else if (repeatState == 1) {
        spotify.setRepeatMode(repeat_track);
    } else {
        spotify.setRepeatMode(repeat_context);
    }
}

void back() {
    spotify.previousTrack();
    spotify.getCurrentlyPlaying(getColor, SPOTIFY_MARKET);
}

void pausePlay() {
    playerState = !playerState;
    if (playerState) {
        spotify.play();
    } else {
        spotify.pause();
    }
}

void skip() {
    spotify.nextTrack();
    spotify.getCurrentlyPlaying(getColor, SPOTIFY_MARKET);
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
}