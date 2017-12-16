/*********************************************************************


This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

/*
 *
 *    Author: Michael Li (Self - independent developer)
 *    Date: 10/27/2017
 *    Company: Self
 *    Project: Renesas S5D9 Tutorial.
 *
 *    Description: This file is based on Adafruit work and modified extensively for Renesas S5D9.
 *
 *  Develop the C code from scratch for the S5D9 board
 *
 *  Read the current temperature (F) from onboard AMS EN210 sensor (I2C interface).
 *  Display the current temperature (F) on OLED display (I2C interface).
 *  Display the temperature graphically over time on OLED display.
 *
 *
 */

//#include "hal_data.h"
#include "oled_ssd1306_thread.h"    // g_i2c_oled api calls
#include <string.h>                 // for memset
#include <system_thread_entry.h>    // for APP_TRAP
#include "ssd1306_oled.h"


// the memory buffer for the LCD

static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8] = { 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80,
0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0xFF,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00,
0x80, 0xFF, 0xFF, 0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x8C, 0x8E, 0x84, 0x00, 0x00, 0x80, 0xF8,
0xF8, 0xF8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80,
0x00, 0xE0, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xC7, 0x01, 0x01,
0x01, 0x01, 0x83, 0xFF, 0xFF, 0x00, 0x00, 0x7C, 0xFE, 0xC7, 0x01, 0x01, 0x01, 0x01, 0x83, 0xFF,
0xFF, 0xFF, 0x00, 0x38, 0xFE, 0xC7, 0x83, 0x01, 0x01, 0x01, 0x83, 0xC7, 0xFF, 0xFF, 0x00, 0x00,
0x01, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0xFF, 0xFF, 0x07, 0x01, 0x01, 0x01, 0x00, 0x00, 0x7F, 0xFF,
0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xFF,
0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x03, 0x0F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x8F,
0x8F, 0x9F, 0xBF, 0xFF, 0xFF, 0xC3, 0xC0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x00, 0x01, 0x03, 0x03, 0x03,
0x03, 0x03, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01,
0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 0x00, 0x00,
0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x03,
0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#if (SSD1306_LCDHEIGHT == 64)
0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x0F,
0x87, 0xC7, 0xF7, 0xFF, 0xFF, 0x1F, 0x1F, 0x3D, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0x7C, 0x7D, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x07, 0x00, 0x30, 0x30, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xC0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xC0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F, 0x1F,
0x0F, 0x07, 0x1F, 0x7F, 0xFF, 0xFF, 0xF8, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xE0,
0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00,
0x00, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x0E, 0xFC, 0xF8, 0x00, 0x00, 0xF0, 0xF8, 0x1C, 0x0E,
0x06, 0x06, 0x06, 0x0C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFC,
0xFE, 0xFC, 0x00, 0x18, 0x3C, 0x7E, 0x66, 0xE6, 0xCE, 0x84, 0x00, 0x00, 0x06, 0xFF, 0xFF, 0x06,
0x06, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x06, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0xC0, 0xF8,
0xFC, 0x4E, 0x46, 0x46, 0x46, 0x4E, 0x7C, 0x78, 0x40, 0x18, 0x3C, 0x76, 0xE6, 0xCE, 0xCC, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x0F, 0x03,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00,
0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x03, 0x07, 0x0E, 0x0C,
0x18, 0x18, 0x0C, 0x06, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x01, 0x0F, 0x0E, 0x0C, 0x18, 0x0C, 0x0F,
0x07, 0x01, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00,
0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x07,
0x07, 0x0C, 0x0C, 0x18, 0x1C, 0x0C, 0x06, 0x06, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#endif
};

static const uint8_t fontbuffer_char[30][FONTHEIGHT * FONTWIDTH / 8] = {
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // Blank
        {0x00,0x30,0x48,0x48,0x78,0x48,0x84,0x84},  // A
        {0x00,0xF0,0x88,0x88,0xF0,0x88,0x88,0xF0},  // B
        {0x00,0x70,0x88,0x80,0x80,0x80,0x88,0x70},  // C
        {0x00,0xF0,0x88,0x88,0x88,0x88,0x88,0xF0},  // D
        {0x00,0xF8,0x80,0x80,0xF8,0x80,0x80,0xF8},  // E
        {0x00,0xF8,0x80,0x80,0xF8,0x80,0x80,0x80},  // F
        {0x00,0x70,0x88,0x80,0x9C,0x88,0x88,0x70},  // G
        {0x00,0x88,0x88,0x88,0xF8,0x88,0x88,0x88},  // H
        {0x00,0x38,0x10,0x10,0x10,0x10,0x10,0x38},  // I
        {0x00,0x38,0x10,0x10,0x10,0xD0,0x90,0x70},  // J
        {0x00,0x88,0x90,0xA0,0xC0,0xA0,0x90,0x88},  // K
        {0x00,0x80,0x80,0x80,0x80,0x80,0x88,0xF8},  // L
        {0x00,0xC6,0xC6,0xAA,0xAA,0x92,0x92,0x92},  // M
        {0x00,0xC4,0xA4,0x94,0x94,0x8C,0x8C,0x84},  // N
        {0x00,0x78,0x84,0x84,0x84,0x84,0x84,0x38},  // O
        {0x00,0xFC,0x84,0x84,0xFC,0x80,0x80,0x80},  // P
        {0x00,0x78,0x84,0x84,0xA4,0x94,0x88,0x74},  // Q
        {0x00,0xF8,0x84,0x84,0xF8,0xA0,0x90,0x8C},  // R
        {0x00,0x78,0x84,0x80,0x78,0x04,0x84,0x78},  // S
        {0x00,0xFE,0x10,0x10,0x10,0x10,0x10,0x10},  // T
        {0x00,0xC6,0x82,0x82,0x82,0x82,0x82,0x7C},  // U
        {0x00,0xC6,0x82,0x44,0x28,0x28,0x28,0x10},  // V
        {0x00,0x82,0x82,0x92,0x54,0x54,0x38,0x28},  // W
        {0x00,0x84,0x48,0x30,0x30,0x48,0x48,0x84},  // X
        {0x00,0x82,0x44,0x44,0x38,0x10,0x10,0x10},  // Y
        {0x00,0xF8,0x84,0x08,0x10,0x20,0x44,0xFC},  // Z
        {0x00,0x00,0x00,0x7C,0x00,0x7C,0x00,0x00},  // =
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30},  // .
        {0x00,0x08,0x08,0x10,0x20,0x40,0x80,0x80}   // /
};

static const uint8_t fontbuffer_num[10][FONTHEIGHT * FONTWIDTH / 8] = {
        {0x00,0x78,0x84,0x84,0x84,0x84,0x84,0x38},   // 0
        {0x00,0x60,0xA0,0x20,0x20,0x20,0x20,0x70},   // 1
        {0x00,0x78,0x84,0x04,0x04,0x04,0x78,0x84},   // 2
        {0x00,0x78,0x84,0x04,0x38,0x04,0x84,0x78},   // 3
        {0x00,0x24,0x44,0x84,0xFC,0x04,0x04,0x04},   // 4
        {0x00,0xF8,0x80,0xF0,0x08,0x04,0x08,0xF0},   // 5
        {0x00,0x78,0x80,0x80,0x78,0x84,0x88,0x70},   // 6
        {0x00,0xF8,0x84,0x08,0x08,0x10,0x10,0x20},   // 7
        {0x00,0x70,0x88,0x88,0x70,0x88,0x88,0x70},   // 8
        {0x00,0x70,0x88,0x88,0x78,0x08,0x08,0x08}    // 9
};

#define BLANKINDEX 0
#define AINDEX     1
#define EQINDEX    27
#define DOTINDEX   28
#define SLASHINDEX 29



void writedisplaybuffer(uint16_t xpos, uint16_t ypos, char str[], uint8_t len){
    uint16_t xloc = xpos;
    uint8_t  index;

    for (uint8_t i=0; i<len; i++){
        if (str[i] >= '0' && str[i] <= '9') {
            index = (uint8_t)(str[i]-(char)'0');
            writefontnum(xloc,ypos,index);
            xloc = (uint16_t) (xloc + FONTWIDTH);
        }
        if (str[i] >= 'A' && str[i] <= 'Z') {
            index = (uint8_t) (str[i]-(char)'A'+ AINDEX);
            writefontchar(xloc,ypos,index);
            xloc = (uint16_t) (xloc + FONTWIDTH);
        }
        if (str[i] == ' ') {
            index = BLANKINDEX;
            writefontchar(xloc,ypos,index);
            xloc = (uint16_t) (xloc + FONTWIDTH);
        }
        if (str[i] == '.') {
            index = DOTINDEX;
            writefontchar(xloc,ypos,index);
            xloc = (uint16_t) (xloc + FONTWIDTH);
        }
        if (str[i] == '=') {
            index = EQINDEX;
            writefontchar(xloc,ypos,index);
            xloc = (uint16_t) (xloc + FONTWIDTH);
        }
        if (str[i] == '/') {
            index = SLASHINDEX;
            writefontchar(xloc,ypos,index);
            xloc = (uint16_t) (xloc + FONTWIDTH);
        }

    }
}


void writefontnum(uint16_t xpos, uint16_t ypos, uint8_t number) {
     for (uint16_t y=0; y<FONTHEIGHT; y++){
         for (uint16_t x=0; x<FONTWIDTH; x++){
             if (fontbuffer_num[number][y] & (uint8_t) 1<<(7-x)){
                 drawADot2((uint16_t)(xpos+x),(uint16_t)(ypos+y),1);
             } else {
                 drawADot2((uint16_t)(xpos+x),(uint16_t)(ypos+y),0);
             }
         }
     }
}


void writefontchar(uint16_t xpos, uint16_t ypos, uint8_t choice) {
     for (uint16_t y=0; y<FONTHEIGHT; y++){
         for (uint16_t x=0; x<FONTWIDTH; x++){
             if (fontbuffer_char[choice][y] & (uint8_t) 1<<(7-x)){
                 drawADot2((uint16_t)(xpos+x),(uint16_t)(ypos+y),1);
             } else {
                 drawADot2((uint16_t)(xpos+x),(uint16_t)(ypos+y),0);
             }
         }
     }
}



void oled_begin(uint8_t vccstate) {

    // Init sequence for 128x64 OLED module
    ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
//    ssd1306_command(0x00);//---set low column address
//    ssd1306_command(0x10);//---set high column address
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    ssd1306_command(0x80);                                  // the suggested ratio 0x80
    ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
    ssd1306_command(0x3F);
    ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    ssd1306_command(0x0);                                   // no offset
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
    ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
    if (vccstate == SSD1306_EXTERNALVCC) 
      { ssd1306_command(0x10); }
    else 
      { ssd1306_command(0x14); }
    ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
    ssd1306_command(0x00);                                  // 0x0 act like ks0108
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);
    ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
    ssd1306_command(0x12);
    ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
    if (vccstate == SSD1306_EXTERNALVCC) 
      { ssd1306_command(0x9F); }
    else 
      { ssd1306_command(0xCF); }
    ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
    if (vccstate == SSD1306_EXTERNALVCC) 
      { ssd1306_command(0x22); }
    else 
      { ssd1306_command(0xF1); }
    ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
    
    ssd1306_command(0xb0);
    ssd1306_command(0x10);
    ssd1306_command(0x01);//Set original position to (0,0)
  
  ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel
}



void ssd1306_command(uint8_t c) { 
    // I2C
    uint8_t cmd[2];
    static ssp_err_t err;

    cmd[0] = 0x00;      // Co = 0, D/C = 0
    cmd[1] = c;         // command
    err = g_i2c_oled.p_api->write(g_i2c_oled.p_ctrl, cmd, 2, false);
    APP_ERROR_TRAP(err)
}


void ssd1306_data(uint8_t c) {
    // I2C
    uint8_t cmd[2];
    ssp_err_t err;

    cmd[0] = 0x40;      // Co = 0, D/C = 1
    cmd[1] = c;         // data

    err = g_i2c_oled.p_api->write(g_i2c_oled.p_ctrl, cmd, 2, false);
    APP_ERROR_TRAP(err)
}

void oled_display(void) {
  ssp_err_t err;
  uint8_t tempbuf[17];

  ssd1306_command(SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
  ssd1306_command(SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
  ssd1306_command(SSD1306_SETSTARTLINE | 0x0); // line #0

    // I2C
    for (uint16_t y=0; y<SSD1306_LCDHEIGHT; y++) {
      // send a bunch of data in one xmission
      tempbuf[0] = 0x40;
      for (uint8_t x=0; x<16; x++) {
          tempbuf[x+1] = buffer[y*16+x];
      }

      err = g_i2c_oled.p_api->write(g_i2c_oled.p_ctrl, tempbuf, 17, false); // 0x40 + 16 data bytes
      APP_ERROR_TRAP(err)
    }
}

void clearBuffer(void){
    for (uint16_t i=0; i<(SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8); i++) {
          buffer[i] = 0;
    }
}


/*
 *
 *   Basic Drawing Subroutines
 *
 */


// y = the row position (range: 0 to 63 Display Length)
// x = the column position (range : 0 128 Display Width)
void drawADot1(uint16_t x, uint16_t y,  uint8_t bitval) {
    uint8_t temp;
    if (y < SSD1306_LCDHEIGHT && x < SSD1306_LCDWIDTH) {
        if (bitval == 1){
            buffer[ y*16+x/8] |= (uint8_t)(1<<(x%8));
        } else {
            temp = (uint8_t)(1<<(x%8));
            temp = (uint8_t) ~temp;
            buffer[ y*16+x/8] &= temp;
        }

    }
}

// y = the row position (range: 0 to 63 Display Length)
// x = the column position (range : 0 128 Display Width)

/*
 * x, y logic map
 *
 *   x --> (horizontal)  x = 0 ...  x = 127
 *                       1st byte #0  : MSbit x = 7 LSbit x = 0
 *                       last byte #15: MSbit x = 127 LSbit x = 120
 *   Y (vertical)
 *   y = 0   (  128 bits or columns or 16 bytes) row #1
 *   ...
 *   y = 63  (  128 bits or columns or 16 bytes) row #64
 *
 * xp, yp physical map
 *
 *   xp--> (horizontal)
 *
 *   mapping
 *
 *   A. (logical row 0 : y = 0, x = 0 to 127)
 *
 *      yp=0)
 *      byte #0.bit0 : yp = 0, xp =0  (y = 0, x = 0)
 *      byte #1.bit0 : yp = 0, xp =16 (y = 0, x = 1)
 *      byte #15.bit0 : yp = 0 xp = 120 (y = 0, x = 15)
 *
 *     (yp=1)
 *      byte #0.bit0 : yp = 1, xp = 0 (y = 0, x = 16)
 *      byte #1.bit0 : yp = 1, xp = 16 (y = 0, x = 17)
 *      byte #15.bit0 : yp = 1, xp = 120 (y = 0, x = 31)
 *
 *     (yp=7)
 *      byte #0.bit0 : yp = 7, xp = 0 (y = 0, x = 120)
 *      byte #1.bit0 : yp = 7, xp = 16 (y = 0, x = 121)
 *      byte #15.bit0 : yp = 7, xp = 120 (y = 0, x = 127)

 *   A. (logical row 1 : y = 0, x = 0 to 127)
 *
 *      yp=0)
 *      byte #0.bit0 : yp = 0, xp =1  (y = 1, x = 0)
 *      byte #1.bit0 : yp = 0, xp =17 (y = 1, x = 1)
 *      byte #15.bit0 : yp = 0 xp = 121 (y = 1, x = 15)
 *
 *     (yp=1)
 *      byte #0.bit0 : yp = 1, xp = 1 (y = 1, x = 16)
 *      byte #1.bit0 : yp = 1, xp = 17 (y = 1, x = 17)
 *      byte #15.bit0 : yp = 1, xp = 121 (y = 1, x = 31)
 *
 *     (yp=7)
 *      byte #0.bit0 : yp = 7, xp = 1 (y = 1, x = 120)
 *      byte #1.bit0 : yp = 7, xp = 17 (y = 1, x = 121)
 *      byte #15.bit0 : yp = 7, xp = 121 (y = 1, x = 127)
 *
 *   C.  Generalization for the first 7 rows (y=0 to 7).
 *
 *      yp = 0  xp = 0 --> y=0 x = 0
 *              xp = 1 --> y=1 x = 0
 *              xp = 7 --> y=7 x = 0
 *
 *      yp = 7  xp = 120 --> y=0 x = 127
 *              xp = 121 --> y=1 x = 127
 *              xp = 127 --> y=7 x = 127
 *
 *   D.  Generalization for the next 7 rows (y=8 to 15).
 *
 *      yp = 8  xp = 0 --> y=8 x = 0
 *              xp = 1 --> y=9 x = 0
 *              xp = 7 --> y=15 x = 0
 *
 *      yp = 15 xp = 120 --> y=8 x = 127
 *              xp = 121 --> y=9 x = 127
 *              xp = 127 --> y=15 x = 127
 *
 *   E.  Generalization for the last 7 rows (y=56 to 63).
 *
 *      yp = 56 xp = 0 --> y=56 x = 0
 *              xp = 1 --> y=57 x = 0
 *              xp = 7 --> y=63 x = 0
 *
 *      yp = 63 xp = 120 --> y=56 x = 127
 *              xp = 121 --> y=57 x = 127
 *              xp = 127 --> y=63 x = 127
 *
 */

void drawADot2(uint16_t x, uint16_t y, uint8_t bitval) {
    uint16_t xp, yp;

    uint8_t temp;

        xp = (uint16_t)((x%16)*8 + y%8);  // re-map x position
        yp = (uint16_t)(x/16 + (y/8)*8);  // re-map y position
        if (bitval == 1){
            buffer[ yp*16+xp/8] |= (uint8_t)(1<<(xp%8));
        } else {
            temp = (uint8_t)(1<<(xp%8));
            temp = (uint8_t)~temp;
            buffer[ yp*16+xp/8] &= temp;
        }

}

/*
 *
 *    Display A number
 *
 */

void displayNumber (uint16_t xpos, uint16_t ypos, uint16_t value){
    uint8_t numdigits = 0;
    uint16_t x = xpos;
    uint16_t y = ypos;
    uint16_t tempvalue = value;
    uint16_t num = 0;

    if (tempvalue >99) {
        numdigits = 3;
    } else if (tempvalue > 9) {
        numdigits = 2;
    } else {
        numdigits = 1;
    }
    if (numdigits == 3) {
        num = tempvalue/100;
        writefontnum(x, y, (uint8_t)num);
        tempvalue %= 100;
        x = (uint16_t) (x + FONTWIDTH);
    }
    if (numdigits >= 2) {
        num = tempvalue/10;
        writefontnum(x, y, (uint8_t) num);
        tempvalue %= 10;
        x = (uint16_t) (x + FONTWIDTH);
    }
    writefontnum(x, y, (uint8_t) tempvalue);
}

void drawverticalline(uint16_t x, uint16_t y, uint16_t length) {
    for (uint16_t i=0; i<length; i++){
        drawADot2(x, (uint16_t)(y+i), 1);
    }
}

void drawhorizontalline(uint16_t x, uint16_t y, uint16_t length) {
    for (uint16_t i=0; i<length; i++){
         drawADot2((uint16_t)(x+i),y,1);
    }
}

void displayHistogram(uint16_t xhist, uint16_t temperatureF) {
    //uint16_t  yhist = 63 - (temperatureF - 60);
    uint16_t yhist = (uint16_t)((uint16_t)HISTYPOS - 1 - (temperatureF - (uint16_t)MINTEMPHISTF));

    for (int i=0; i<HISTHEIGHT+2; i++){
        drawADot2(xhist, (uint16_t)(HISTYPOS-i), 0);
    }
    if (temperatureF <= MINTEMPHISTF) {
        yhist = (uint16_t)HISTYPOS-1;
    }
    if (temperatureF >= MAXTEMPHISTF) {
        yhist = (uint16_t) HISTYPOS - 1 - HISTHEIGHT;
    }
    drawADot2(xhist, yhist, 1);

}
