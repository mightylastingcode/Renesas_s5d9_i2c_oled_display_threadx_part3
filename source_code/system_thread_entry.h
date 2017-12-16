/*
 * system_thread_entry.h
 *
 *  Created on: Mar 25, 2017
 *      Author: gjacobso01
 */

#ifndef SYSTEM_THREAD_ENTRY_H_
#define SYSTEM_THREAD_ENTRY_H_
#include "system_thread.h"

#define APP_ERROR_TRAP(x)    if (x) {\
                                   g_ioport.p_api->pinWrite(g_leds.p_leds[RED], ON);\
                                   g_ioport.p_api->pinWrite(g_leds.p_leds[YEL], OFF);\
                                   g_ioport.p_api->pinWrite(g_leds.p_leds[GRN], OFF);\
                                    __BKPT();}

#define GRN (BSP_LED_LED1)
#define YEL (BSP_LED_LED2)
#define RED (BSP_LED_LED3)

#define OFF (IOPORT_LEVEL_LOW)
#define ON (IOPORT_LEVEL_HIGH)

extern bsp_leds_t g_leds;

#endif /* SYSTEM_THREAD_ENTRY_H_ */
