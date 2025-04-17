/*
 * ssd1306.h

	Driver for SSD1306 controlled dot matrix OLED/PLED 128x64 displays

	Based on work by Neven Boyanov, Tejashwi Kalp Taru, Bob Tidey

	https://github.com/martind69/SSD1306
	Modified 2024, Martin Dittrich
	All rights reserved.
 * 
 * Edited by Jake Strube for PIC18F47Q10 for EGR 314
 * 4/15/2025
  
  
 * IMPORTANT: Use the function ssd1306_clear(); in the main section right before the while loop to clear the noise off of the screen.
 * 
 * 
 * PSA: For any pos functions pos range with X(first unit) from 0 to 128, and Y(second unit) from 0 to 7.
 *      Going to 128 on the x will move the character to the next row because the character does not fit within the last row alone.
 *      So the number will need to be lower ex: 120 to get a character right on the edge.
	
*/

#ifndef oled_H
#define oled_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "mcc_generated_files/i2c_host/mssp1.h"
#include "ssd1306font.h"

#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

void ssd1306_init(uint8_t saddr);
void ssd1306_sleep(bool s);
void ssd1306_setpos(uint8_t x, uint8_t y);
void ssd1306_setscale(uint8_t s);
void ssd1306_fillscreen(unsigned char ch);
void ssd1306_flipscreen(bool f);
void ssd1306_invert(bool i);
void ssd1306_char_move(char ch, uint8_t mode, uint16_t offset);
void ssd1306_char(char ch);
void ssd1306_clear(void);
void ssd1306_char_pos(uint8_t x, uint8_t y, char *ch);
void ssd1306_string(char *s);
void ssd1306_string_pos(uint8_t x, uint8_t y, char *s);
void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t bitmap_loc);

uint8_t oledAddr;
uint8_t xpos;
uint8_t ypos;
uint8_t scale;

#endif
