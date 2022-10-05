/*
 * led.c
 *
 * Created: 2022-08-09 오후 2:39:48
 *  Author: kccistc
 */ 

#include "led.h"

void init_led()
{
	AUTO_RUN_LED_PORT_DDR |= 1 << AUTO_RUN_LED_PIN;
}
