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

void setup() {
    pinMode(SHUFFLE, INPUT_PULLUP);
    pinMode(VOLUME_DEC, INPUT_PULLUP);
    pinMode(VOLUME_INC, INPUT_PULLUP);
    pinMode(LOOP, INPUT_PULLUP);
    pinMode(BACK, INPUT_PULLUP);
    pinMode(PAUSE_PLAY, INPUT_PULLUP);
    pinMode(SKIP, INPUT_PULLUP);
    pinMode(LED, OUTPUT);
    pinMode(SCL, INPUT_PULLUP);
    pinMode(SDA, INPUT_PULLUP);
}

void loop() {

}