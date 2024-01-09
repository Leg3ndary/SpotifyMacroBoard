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

#include <ArduinoJson.h>
#include <FastLED.h>
#include <SpotifyArduino.h>
#include <SpotifyArduinoCert.h>
#include <SpotifyCredentials.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#define RGB_PIN 18
#define RGB_LED_NUM 100   // 300 total
#define BRIGHTNESS 200    // brightness range [0..255]
#define CHIP_SET WS2812B
#define COLOR_CODE RGB // Enum
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

WiFiClientSecure sClient;
WiFiClientSecure bClient;

SpotifyArduino spotify(sClient, spotifyId, spotifySecret,
                       SPOTIFY_REFRESH_TOKEN);

unsigned long delayBetweenRequests = 60000;
unsigned long requestDueTime;

const int keys[7] = {SHUFFLE, VOLUME_DEC, VOLUME_INC, LOOP,
                     BACK,    PAUSE_PLAY, SKIP};
bool keyPrevState[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool keyState[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

// Define the array of LEDs
CRGB LEDs[RGB_LED_NUM];

// define 3 byte for the random color
byte a, b, c;

void setup() {
    // pinMode(SHUFFLE, INPUT_PULLUP);
    // pinMode(VOLUME_DEC, INPUT_PULLUP);
    // pinMode(VOLUME_INC, INPUT_PULLUP);
    // pinMode(LOOP, INPUT_PULLUP);
    // pinMode(BACK, INPUT_PULLUP);
    // pinMode(PAUSE_PLAY, INPUT_PULLUP);
    // pinMode(SKIP, INPUT_PULLUP);

    for (int i = 0; i < 7; i++) {
        pinMode(keys[i], INPUT_PULLUP);
    }

    pinMode(LED, OUTPUT);
    pinMode(SCL, INPUT_PULLUP);
    pinMode(SDA, INPUT_PULLUP);

    Serial.begin(115200);
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

    sClient.setCACert(spotify_server_cert);
    bClient.setCACert(benzServerCert);

    if (!spotify.refreshAccessToken()) {
        Serial.println("Failed to get access tokens");
    }
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
    Serial.println(response);
}

void shuffle();
void volumeDecrease();
void volumeIncrease();
void repeat();
void back();
void pausePlay();
void skip();

void shuffle() {}

void volumeDecrease() {}

void volumeIncrease() {}

void repeat() {}

void back() {}

void pausePlay() {}

void skip() {}

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

    if (millis() > requestDueTime) {
        spotify.getCurrentlyPlaying(getColor, SPOTIFY_MARKET);
        requestDueTime = millis() + delayBetweenRequests;
    }
}