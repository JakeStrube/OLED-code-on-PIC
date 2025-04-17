/*
 * ssd1306.c

	Driver for SSD1306 controlled dot matrix OLED/PLED 128x64 displays

	Based on work by Neven Boyanov, Tejashwi Kalp Taru, Bob Tidey

	https://github.com/martind69/SSD1306
	Modified 2024, Martin Dittrich
	All rights reserved.
 * 
 * Edited by Jake Strube for PIC18F47Q10 for EGR 314
 * 4/15/2025

  
  
  
  
  
 * IMPORTANT: Font changing and scaling does not work right now. trying to change the scale flips the whole screen upside down.
 *            and trying to change the font fills the screen with lines
	
*/
#include "mcc_generated_files/system/system.h"
#include "ssd1306.h"

static const uint8_t ssd1306_init_sequence[] = { // Initialization for the OLED screen
    
    0xAE,
    0x20,
    0x00,
    0x40,
    0xA1,
    0xA8,
    0x3F,
    0xC8,
    0xD3,
    0x00,
    0xDA,
    0x12,
    0xD5,
    0x80,
    0xD9,
    0xF1,
    0xDB,
    0x30,
    0x81,
    0xFF,
    0xA4,
    0xA6,
    0x8D,
    0x14,
    0xAF,

};

static uint8_t i2cBuff[132];

void ssd1306_init(uint8_t saddr) {
	oledAddr = saddr;
	scale = SCALE;
    while(I2C1_IsBusy()); // sit here until we get the I2C connection. Since OLED is the only connection should not have to wait here
	I2C1_Write(oledAddr, ssd1306_init_sequence, sizeof(ssd1306_init_sequence)); // Write the intializing from above to the OLED screen
	ssd1306_fillscreen(0);
    }

void ssd1306_sleep(bool s) {
	i2cBuff[0] = SSD1306_COMMAND;
	if(s) {
		i2cBuff[1] = 0xAE; // display off
		i2cBuff[2] = 0x8D; // disable charge pump
		i2cBuff[3] = 0x10;
	}
	else {
		i2cBuff[1] = 0x8D; // enable charge pump
		i2cBuff[2] = 0x14;
		i2cBuff[3] = 0xAF; // display on
	}
	I2C1_Write(oledAddr, i2cBuff, 4);
}


void ssd1306_setpos(uint8_t x, uint8_t y) {
	xpos = x;
	ypos = y;
	i2cBuff[0] = SSD1306_COMMAND; 
	i2cBuff[1] = 0xb0 | (y & 0x07);
	i2cBuff[2] = 0x10 | ((x & 0xf0) >> 4);
	i2cBuff[3] = x & 0x0f; 
	I2C1_Write(oledAddr, i2cBuff, 4);
    __delay_ms(1); // Delay is necessary or else the position cannot be set correctly
    
}

void ssd1306_setscale(uint8_t s) {
	scale = s;
}

void ssd1306_fillscreen(unsigned char ch) {
	uint8_t i, j;
	for(i = 0; i <= 7; i++) {
		ssd1306_setpos(0, i);
		i2cBuff[0] = SSD1306_DATA;
		for(j = 1; j <= 128; j++) {
			i2cBuff[j] = ch;
		}
		I2C1_Write(oledAddr, i2cBuff, 129);
        
	}
}

void ssd1306_clear(void){ // Sets the screen full of just spaces which clears the noise
    ssd1306_string_pos(0,0,"                                                                                                                                                                           ");
}

void ssd1306_flipscreen(bool f) {
	i2cBuff[0] = SSD1306_COMMAND;
	i2cBuff[1] = f ? 0xA0 : 0xA1;
	i2cBuff[2] = f ? 0xC0 : 0xC8;
	I2C1_Write(oledAddr, i2cBuff, 3);
}

void ssd1306_invert(bool i) {
	i2cBuff[0] = SSD1306_COMMAND;
	i2cBuff[1] = i ? 0xA7 : 0xA6;
	I2C1_Write(oledAddr, i2cBuff, 2);
}

void ssd1306_char_move(char ch, uint8_t shift, uint16_t offset) {
#if FONT < 2
	uint8_t i, j = 1;
	char c = ch - 32;
	unsigned char f;
	i2cBuff[0] = SSD1306_DATA;
	for(i = 0; i < FONT_WIDTH; i++) {
		f = (unsigned char)(ssd1306xled_font[c * FONT_WIDTH + i + offset]);
		switch(scale) {
			case 0: i2cBuff[j++] = f;
					break;
			case 1: f = (unsigned char)(nibbleDouble[(f >> shift) & 0x0f]);
					i2cBuff[j++] = f;
					i2cBuff[j++] = f;
					break;
			case 2: f = (unsigned char)(diBitQuad[(f >> shift) & 0x03]);
					i2cBuff[j++] = f;
					i2cBuff[j++] = f;
					i2cBuff[j++] = f;
					i2cBuff[j++] = f;
					break;
		}
	}
	I2C1_Write(oledAddr, i2cBuff, j);

#endif
}

void ssd1306_char(char ch) {
#if FONT == 0
	switch(scale) {
		case 0: ssd1306_char_move(ch, 0, 0);
				break;
		case 1: ssd1306_char_move(ch, 0, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, 0);
				ssd1306_setpos(xpos + 12, ypos - 1);
				break;
		case 2: ssd1306_char_move(ch, 0, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 2, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 6, 0);
				ssd1306_setpos(xpos + 24, ypos - 3);
				break;
	}
#elif FONT == 1

    
	switch(scale) {
		case 0: ssd1306_char_move(ch, 0, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 0, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos+8, ypos-1);
				break;
		case 1: ssd1306_char_move(ch, 0, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 0, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos+16, ypos - 3);
				break;
		case 2: ssd1306_char_move(ch, 0, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 2, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 6, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 0, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 2, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 6, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos+32, ypos - 7);
				break;
	}
#endif
    
}

void ssd1306_char_pos(uint8_t x, uint8_t y, char *ch) {
#if FONT < 2
	ssd1306_setpos(x, y);
	ssd1306_char(*ch);
#endif
}

void ssd1306_string(char *s) {
#if FONT < 2
	while(*s) {
		ssd1306_char(*s++);
        __delay_ms(1); // Delay is needed otherwise text doesn't write properly
	}
#endif
}

void ssd1306_string_pos(uint8_t x, uint8_t y, char *s) {
#if FONT < 2
	ssd1306_setpos(x, y);
	ssd1306_string(s);
#endif
}

// This function probably does not work was not tested
void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t bitmap_loc) {
	uint16_t j = bitmap_loc;
	uint8_t x, y;
	y0 /= 8;
	y1 /= 8;
	//y1 % 8 ? NULL : y1++;
	for(y = y0; y < y1; y++) {
		ssd1306_setpos(x0, y);
		i2cBuff[0] = SSD1306_DATA;
		for(x = 1; x <= (x1 - x0); x++) {
			//i2cBuff[x] = DATAEE_ReadByte(j++);
		}
		// write in blocks of max 128 bytes
		I2C1_Write(oledAddr, i2cBuff, x);
	}
}
