/*
 * ultrasonic.h
 *
 * Created: 2022-08-10 오후 4:22:34
 *  Author: kccistc
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define FALSE 0
#define TRUE 1

//====================LEFT ULTRASONIC==============
#define TRIG_LEFT 0
#define TRIG_DDR_LEFT DDRA	// TRIGGER PIN
#define TRIG_PORT_LEFT PORTA

// HW EXTERNAL INT 4 : PE4
#define ECHO_LEFT 4
#define ECHO_DDR_LEFT DDRE
#define ECHO_PIN_LEFT PINE

//====================FORWARD ULTRASONIC==============
#define TRIG_FORWARD 1
#define TRIG_DDR_FORWARD DDRA	// TRIGGER PIN
#define TRIG_PORT_FORWARD PORTA

// HW EXTERNAL INT 5 : PE5
#define ECHO_FORWARD 5
#define ECHO_DDR_FORWARD DDRE
#define ECHO_PIN_FORWARD PINE

//====================RIGHT ULTRASONIC==============
#define TRIG_RIGHT 2
#define TRIG_DDR_RIGHT DDRA	// TRIGGER PIN
#define TRIG_PORT_RIGHT PORTA

// HW EXTERNAL INT 6 : PE6
#define ECHO_RIGHT 6
#define ECHO_DDR_RIGHT DDRE
#define ECHO_PIN_RIGHT PINE

//====================BACKWARD ULTRASONIC==============
#define TRIG_BACKWARD 3
#define TRIG_DDR_BACKWARD DDRA	// TRIGGER PIN
#define TRIG_PORT_BACKWARD PORTA

// HW EXTERNAL INT 7 : PE7
#define ECHO_BACKWARD 7
#define ECHO_DDR_BACKWARD DDRE
#define ECHO_PIN_BACKWARD PINE

volatile int ultrasonic_left_distance=0;
volatile int ultrasonic_forward_distance=0;
volatile int ultrasonic_right_distance=0;
volatile int ultrasonic_backward_distance=0;

#endif /* ULTRASONIC_H_ */