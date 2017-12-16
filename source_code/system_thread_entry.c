#include "system_thread.h"
#include "system_thread_entry.h"

/* LED type structure */
bsp_leds_t g_leds;

/* System Thread entry function */
void system_thread_entry(void)
{

    /* Get LED information for this board */
    R_BSP_LedsGet(&g_leds);

    for (uint16_t i=0; i<g_leds.led_count; i++){
        g_ioport.p_api->pinWrite(g_leds.p_leds[i], OFF);
    }
    g_ioport.p_api->pinWrite(g_leds.p_leds[GRN], ON);
    while (1)
    {
        tx_thread_sleep (100);
    }
}
