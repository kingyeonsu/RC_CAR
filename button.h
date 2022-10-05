/*
 * button.h
 *
 * Created: 2022-08-04 오후 4:10:39
 *  Author: kccistc
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// 상황에 맞게, 목적에 맞게 ㅍ로그래밍하기 편하기 위해
// COMPILER 명령어를 할당

#define BUTTON_DDR	DDRC
#define BUTTON_PIN	PINC	// PORTD read register

// button1~4에 대한 값만 바꾸면 코딩이 훨씬 간단해진다. 같은 포트의 다른 핀의 값은 유지하기 위한 포석
#define BUTTON1	 7	// PORTC.7


#endif /* BUTTON_H_ */