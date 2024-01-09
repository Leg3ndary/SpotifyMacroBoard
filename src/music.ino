#include <ArduinoJson.h>
#include <SpotifyArduino.h>
#include <SpotifyArduinoCert.h>
#include <SpotifyCredentials.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

WiFiClientSecure client;
WiFiClientSecure benzclient;

SpotifyArduino spotify(client, spotifyId, spotifySecret, SPOTIFY_REFRESH_TOKEN);

unsigned long delayBetweenRequests = 60000;
unsigned long requestDueTime;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, SSID_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    client.setCACert(spotify_server_cert);
    benzclient.setCACert(benzServerCert);

    if (!spotify.refreshAccessToken()) {
        Serial.println("Failed to get access tokens");
    }
}

void getColor(CurrentlyPlaying current) {
    if (!benzclient.connect("benzhou.tech", 443)) {
        Serial.println("Connection failed");
        return;
    }

    String albumImageUrl = current.albumImages[0].url;
    int lastIndex = albumImageUrl.lastIndexOf('/');
    String imageUrl = albumImageUrl.substring(lastIndex + 1);

    String url = "/api/getColor/";
    url += imageUrl;

    benzclient.print("GET " + url + " HTTP/1.1\r\n" + "Host: benzhou.tech\r\n" +
                     "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (!benzclient.available()) {
        if (millis() - timeout > 5000) {
            Serial.println("No response");
            benzclient.stop();
            return;
        }
    }

    char endOfHeaders[] = "\r\n\r\n";
    if (!benzclient.find(endOfHeaders)) {
        Serial.println("Invalid response");
        return;
    }

    String response = "";
    while (benzclient.available()) {
        char c = benzclient.read();
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