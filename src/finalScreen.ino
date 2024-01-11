/*********
  Complete project details at https://randomnerdtutorials.com

  This is an example for our Monochrome OLEDs based on SSD1306 drivers. Pick one
up today in the adafruit shop! ------> http://www.adafruit.com/category/63_98
  This example is for a 128x32 pixel display using I2C to communicate 3 pins are
required to interface (two I2C and one reset). Adafruit invests time and
resources providing this open source code, please support Adafruit and
open-source hardware by purchasing products from Adafruit! Written by Limor
Fried/Ladyada for Adafruit Industries, with contributions from the open source
community. BSD license, check license.txt for more information All text above,
and the splash screen below must be included in any redistribution.
*********/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#define I2C_SDA 21
#define I2C_SCL 19

TwoWire I2CBME = TwoWire(0);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2CBME, OLED_RESET);

void setup() {
    Serial.begin(115200);
    I2CBME.begin(I2C_SDA, I2C_SCL, 400000);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;  // Don't proceed, loop forever
    }

    display.display();

    // Clear the buffer
    display.clearDisplay();

    generateCurrent();
}

void generateCurrent() {
    char fakeWifi[] = "Fake Wifi     ";
    char fakeVolume[] = "||||||";
    char trackName[] = "Love Story";
    char artistName[] = "Taylor Swift";

    delay(2000);

    display.clearDisplay();
    display.setTextSize(1);  // Normal 1:1 pixel scale
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.invertDisplay(true);
    delay(1000);
    display.print(fakeWifi);
    display.print(fakeVolume);
    delay(1000);
    display.invertDisplay(false);
    delay(1000);
    display.setCursor(0, 8);
    display.print(artistName);
    display.setCursor(0, 16);
    display.setTextSize(2);
    display.print(trackName);

    display.display();
}

void loop() {}