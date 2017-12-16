# Renesas_s5d9_i2c_oled_display_threadx_part3
This is Part 3 of the I2C tutorial with the OLED display.  This tutorial will use the real time OS, ThreadX to build the firmware.


Develop the C code from scratch for the S5D9 board
Read the current temperature (F) from an external SHT31 sensor (I2C interface).
Display the current temperature (F) on OLED display (I2C interface).
Display the temperature graphically over time on OLED display.

In this firmware, we will have three threads.

File Description in the source_code folder
1.  hal_entry.c  (empty function)
2.  system_thread_entry.c/h  (Setup LED for normal operation and warning purpose)
3.  sht31_thread.c/h  (SHT31.c/h + all the SHT31 related code in the part 2 hal_entry.c + i2c.c)
3.  oled_ssd1306_thread.c/h  (all the oled ssd1306 related code in the part 2 hal_entry.c)
4.  ssd1306_oled.c/h (unchanged except for the iic driver name from part 2)

S5D9_I2C_SHT31_OLED_TX_UARTBUS_v4.7z is the project file in zip format.  Use 7-Zip to unzip the file.

Datasheet:
SENSIRION SHT3x-DIS 314010179_SMD Temperature and Humidity Sensor Datasheets.pdf (Temperature sensor datasheet)
OLED Sepecification.pdf (OLED display module spec sheet)
SSD1306.pdf (OLED display driver chip datasheet)

https://www.miketechuniverse.com/single-post/2017/12/14/I2C-Tutorial-for-Renesas-S5D9-board-Add-ThreadX-real-time-OS-support-Part-3
