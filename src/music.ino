#include <ArduinoJson.h>
#include <SpotifyArduino.h>
#include <SpotifyArduinoCert.h>
#include <SpotifyCredentials.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

WiFiClientSecure sClient;
WiFiClientSecure bClient;

SpotifyArduino spotify(sClient, spotifyId, spotifySecret, SPOTIFY_REFRESH_TOKEN);

unsigned long delayBetweenRequests = 60000;
unsigned long requestDueTime;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, SSID_PASS);

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

void loop() {
    if (millis() > requestDueTime) {
        int status = spotify.getCurrentlyPlaying(getColor, SPOTIFY_MARKET);
        requestDueTime = millis() + delayBetweenRequests;
    }
}