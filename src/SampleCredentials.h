const String PASSWORD = "A Password.";

const char SSID[] = "Network SSID";
const char SSID_PASS[] = "Network Password";

const char *benzServerCert =
    "-----BEGIN CERTIFICATE-----\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "-----END CERTIFICATE-----";


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

const unsigned char wifi_2[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xc1, 0x83, 0x9f, 0xf1, 0x38,
    0x1c, 0xe1, 0x87, 0xcf, 0xf3, 0xd8, 0x1b, 0xf1, 0x8f, 0xf3, 0xcf,
    0xfe, 0x7f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff};

const unsigned char wifi_1[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8,
    0x1f, 0xe1, 0x87, 0xcf, 0xf3, 0xd8, 0x1b, 0xf1, 0x8f, 0xf3, 0xcf,
    0xfe, 0x7f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff};

const unsigned char wifi_0[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xf1, 0x8f, 0xf3, 0xcf,
    0xfe, 0x7f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff};

const unsigned char volume_3[] PROGMEM = {
    0xff, 0xff, 0xff, 0xf7, 0xff, 0xf3, 0xf9, 0xf9, 0xf1, 0xcd, 0x01, 0xe4,
    0x01, 0xb4, 0x01, 0x96, 0x01, 0x96, 0x01, 0xb4, 0x01, 0xe4, 0xf1, 0xcd,
    0xf9, 0xd9, 0xff, 0xf3, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

const unsigned char volume_2[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xf1, 0xcf, 0x01,
    0xe7, 0x01, 0xb7, 0x01, 0x97, 0x01, 0x97, 0x01, 0xb7, 0x01, 0xe7,
    0xf1, 0xcf, 0xf9, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

const unsigned char volume_1[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xf1, 0xff, 0x01,
    0xff, 0x01, 0xbf, 0x01, 0x9f, 0x01, 0x9f, 0x01, 0xbf, 0x01, 0xff,
    0xf1, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

const unsigned char volume_0[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xf1, 0xff, 0x01,
    0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff,
    0xf1, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};