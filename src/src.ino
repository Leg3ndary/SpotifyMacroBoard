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

const int keys[7] = {SHUFFLE, VOLUME_DEC, VOLUME_INC, LOOP, BACK, PAUSE_PLAY, SKIP};
bool keyPrevState[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool keyState[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};


void setup() {
    // pinMode(SHUFFLE, INPUT_PULLUP);
    // pinMode(VOLUME_DEC, INPUT_PULLUP);
    // pinMode(VOLUME_INC, INPUT_PULLUP);
    // pinMode(LOOP, INPUT_PULLUP);
    // pinMode(BACK, INPUT_PULLUP);
    // pinMode(PAUSE_PLAY, INPUT_PULLUP);
    // pinMode(SKIP, INPUT_PULLUP);

    for (int i = 0; i < 7; i++) [
        pinMode(keys[i], INPUT_PULLUP);
    }

    pinMode(LED, OUTPUT);
    pinMode(SCL, INPUT_PULLUP);
    pinMode(SDA, INPUT_PULLUP);
}

void shuffle();
void volumeDecrease();
void volumeIncrease();
void repeat();
void back();
void pausePlay();
void skip();

void shuffle() {

}

void volumeDecrease() {

}

void volumeIncrease() {

}

void repeat() {

}

void back() {

}

void pausePlay() {

}

void skip() {

}


void (*funcs[7])() = {shuffle, volumeDecrease, volumeIncrease, repeat, back, pausePlay, skip};

void loop() {
    for (int i = 0; i < 7; i++) {
        keyState[i] = digitalRead(keys[i]);
        if (keyState[i] == LOW && keyPrevState[i] == HIGH) {
            funcs[i]();
        }
        keyPrevState[i] = keyState[i];
    }
}