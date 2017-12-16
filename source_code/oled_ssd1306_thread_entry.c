#include "oled_ssd1306_thread.h"
#include "ssd1306_oled.h"
#include <stdio.h>                     // sprintf
#include <system_thread_entry.h>
#define MSGLENMAX 32

extern volatile float g_temperatureF_sht31;


/* OLED SSD1306 Thread entry function */
void oled_ssd1306_thread_entry(void)
{
    ssp_err_t err;
    int count = 0;

    char msg[MSGLENMAX];
    uint16_t xhist=HISTXPOS;   // The x position of data histogram

    err = g_i2c_oled.p_api->open(g_i2c_oled.p_ctrl, g_i2c_oled.p_cfg);
    APP_ERROR_TRAP(err)

    // Show Adafruit logo to credit their work
    oled_begin(SSD1306_SWITCHCAPVCC);  // initialize with the I2C addr 0x3D (for the 128x64)
    oled_display();

     tx_thread_sleep (500); // 5 sec
     clearBuffer(); // clears the screen and buffer
     sprintf(msg,"=IOT COMMUNITY=");
     writedisplaybuffer(0, 0, msg,(uint8_t)strlen(msg));
     sprintf(msg,"    PRESENTS   ");
     writedisplaybuffer(0, 16, msg,(uint8_t)strlen(msg));
     sprintf(msg,"RENESAS S5D9 MCU");
     writedisplaybuffer(0, 32, msg,(uint8_t)strlen(msg));
     sprintf(msg,"  SHT31 TEMP");
     writedisplaybuffer(0, 48, msg,(uint8_t) strlen(msg));
     sprintf(msg,"  SENSOR");
     writedisplaybuffer(0, 56, msg,(uint8_t) strlen(msg));

     tx_thread_sleep (200); // 2 sec
     oled_display();

     tx_thread_sleep (500); // 5 sec
     clearBuffer(); // clears the screen and buffer
     // Display all the character
     for (uint16_t i=0, j=0, k=0; i<29; i++, k++){
          if (i != 0 && i%16 == 0)
          {
              k=0;
              j++;
          }
          writefontchar((uint16_t)(k*8), (uint16_t)(j*8), (uint8_t) i);
     }
     for (uint8_t i=0; i<10; i++){
           writefontnum((uint16_t)(i*8),16, i);
     }
     sprintf(msg,"MICHAEL LI");
     writedisplaybuffer(0, 32, msg, (uint8_t) strlen(msg));
     sprintf(msg,"OLED DISPLAY");
     writedisplaybuffer(0, 40, msg, (uint8_t) strlen(msg));
     sprintf(msg,"PROJECT");
     writedisplaybuffer(0, 48, msg, (uint8_t) strlen(msg));
     sprintf(msg,"11/19/2017");
     writedisplaybuffer(0, 56, msg, (uint8_t) strlen(msg));
     oled_display(); // draw the new image.

     tx_thread_sleep (500); // 5 sec

     clearBuffer(); // clears the screen and buffer


    /* TODO: add your own code here */
    while (1)
    {
        sprintf(msg,"%1dTEMP SHT31=%3d",count,(uint8_t) g_temperatureF_sht31);

        if (count > 0)
            count = 0;
        else
            count++;

        writedisplaybuffer(0, 0, msg, (uint8_t) strlen(msg));

        drawverticalline(HISTXPOS-1,HISTYPOS-(1+HISTHEIGHT+1),HISTHEIGHT+1);
        drawhorizontalline(HISTXPOS-1,HISTYPOS-1,HISTWIDTH+1);

        for (uint16_t i=0; i< HISTHEIGHT/10+1; i++) {
             drawADot2(HISTXPOS-2, (uint16_t)(HISTYPOS-1-i*10), 1);
             drawADot2(HISTXPOS-3, (uint16_t)(HISTYPOS-1-i*10), 1);
             drawADot2(HISTXPOS-4, (uint16_t)(HISTYPOS-1-i*10), 1);
             displayNumber (0,(uint16_t)(HISTYPOS-1-i*10-4), (uint16_t)(MINTEMPHISTF + i * 10));
        }

        displayHistogram(xhist, (uint16_t) g_temperatureF_sht31);
        if (xhist < SSD1306_LCDWIDTH-1){
             xhist++;
        } else {
             xhist = HISTXPOS;
        }
        oled_display(); // draw the new image.
        //tx_thread_sleep (10);
        tx_thread_sleep (1);
     }
}
