/*
 * led.h
 *
 * Created: 2022-08-09 오후 2:39:59
 *  Author: kccistc
 */ 


#ifndef LED_H_
#define LED_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define AUTO_RUN_LED_PORT		PORTG
#define AUTO_RUN_LED_PORT_DDR	DDRG
#define AUTO_RUN_LED_PIN		4




#endif /* LED_H_ */