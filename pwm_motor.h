/*
 * pwm_motor.h
 *
 * Created: 2022-08-19 오후 2:20:59
 *  Author: kccistc
 */ 


#ifndef PWM_MOTOR_H_
#define PWM_MOTOR_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define MOTOR_DRIVER_PORT PORTF
#define MOTOR_DRIVER_PORT_DDR DDRF

#define MOTOR_DDR DDRB
#define MOTOR_RIGHT_PORT_DDR 5	// OC1A
#define MOTOR_LEFT_PORT_DDR 6	// OC1B



#endif /* PWM_MOTOR_H_ */