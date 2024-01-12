// 'wifi-logo', 16x16px
const unsigned char wifi_logo [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xc1, 0x83, 0x9f, 0xf1, 0x38, 0x1c, 0xe1, 0x87, 0xcf, 0xf3, 
	0xd8, 0x1b, 0xf1, 0x8f, 0xf3, 0xcf, 0xfe, 0x7f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff
};

// 'wifi-logo2', 16x16px
const unsigned char wifi_logo2 [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xe1, 0x87, 0xcf, 0xf3, 
	0xd8, 0x1b, 0xf1, 0x8f, 0xf3, 0xcf, 0xfe, 0x7f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff
};

const unsigned char wifi_logo3 [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf8, 0x1f, 0xf1, 0x8f, 0xf3, 0xcf, 0xfe, 0x7f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 48)
const int bitmapsLength = 3;
const unsigned char* bitmaps[3] = {
	wifi_logo,
    wifi_logo2,
    wifi_logo3
};