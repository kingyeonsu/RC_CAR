/*
 * pwm_motor.c
 *
 * Created: 2022-08-19 오후 2:20:03
 *  Author: kccistc
 */ 

#include "pwm_motor.h"

extern volatile unsigned char UART1_RX_data;

void forward(int speed);
void backward(int speed);
void turn_left(int speed);
void turn_right(int speed);
void back_left(int speed);
void back_right(int speed);
void stop();
void turn_left_auto(int speed1, int speed2);
void turn_right_auto(int speed1, int speed2);

void manual_mode_run(void)
{
	switch (UART1_RX_data)
	{
		case 'F':
		forward(512);	// 4us x 500 = 0.002sec (2ms)
		break;
		case 'B':
		backward(500);
		break;
		case 'L':
		turn_left(700);
		break;
		case 'R':
		turn_right(700);
		break;
		case 'S':
		stop();
		break;
		default:
		break;
	}
}

void forward(int speed)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0); // 자동차 전진 모드로 세팅
	
	OCR1A = speed;	// PB5 PWM  OUTPUT	LEFT
	OCR1B = speed;	// PB6 PWM	OUTPUT	RIGHT
}

void backward(int speed)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	MOTOR_DRIVER_PORT |= (1 << 3) | (1 << 1); // 자동차 후진 모드로 세팅
	
	OCR1A = speed;	// PB5 PWM  OUTPUT	LEFT
	OCR1B = speed;	// PB6 PWM	OUTPUT	RIGHT
}

void turn_left_auto(int speed1, int speed2)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0); // 자동차 전진 좌측 조향 모드로 세팅
	
	OCR1A = speed1;			// PB5 PWM  OUTPUT	LEFT
	OCR1B = speed2;			// PB6 PWM	OUTPUT	RIGHT
}

void turn_right_auto(int speed1, int speed2)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0); // 자동차 전진 우측 조향 모드로 세팅
	
	OCR1A = speed2;				// PB5 PWM  OUTPUT	LEFT
	OCR1B = speed1;				// PB6 PWM	OUTPUT	RIGHT
}


void turn_left(int speed)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0); // 자동차 전진 좌측 조향 모드로 세팅
	
	OCR1A = speed;			// PB5 PWM  OUTPUT	LEFT
	OCR1B = 0;			// PB6 PWM	OUTPUT	RIGHT
}

void turn_right(int speed)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0); // 자동차 전진 우측 조향 모드로 세팅
	
	OCR1A = 0;				// PB5 PWM  OUTPUT	LEFT
	OCR1B = speed;				// PB6 PWM	OUTPUT	RIGHT
}

void back_left(int speed)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 1); // 자동차 후진 모드로 세팅
	
	OCR1A = speed;			// PB5 PWM  OUTPUT	LEFT
	OCR1B = speed;			// PB6 PWM	OUTPUT	RIGHT
}

void back_right(int speed)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	MOTOR_DRIVER_PORT |= (1 << 3) | (1 << 0); // 자동차 후진 모드로 세팅
	
	OCR1A = speed;					// PB5 PWM  OUTPUT	LEFT
	OCR1B = speed;				// PB6 PWM	OUTPUT	RIGHT
}

void stop()
{
	OCR1A = 0;	// PB5 PWM  OUTPUT	LEFT
	OCR1B = 0;	// PB6 PWM	OUTPUT	RIGHT
	
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	MOTOR_DRIVER_PORT |= (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // 자동차 정지 모드로 세팅
}

//=================================
/*
	1.	LEFT MOTOR
		PORTF.0 : 1N1
		PORTF.1 : IN2
	2.	RIGHT MOTOR
		PORTF.2 : IN3
		PORTF.3 : IN4
		===============
		IN1,IN3	IN2,IN4
		===============
			0	|	1 : 역회전
			1	|	0 : 정회전
			1	|	1 : STOP
*/


void init_pwm_motor(void)
{
	MOTOR_DRIVER_PORT_DDR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);		// motor diver port
	MOTOR_DDR |= (1 << MOTOR_RIGHT_PORT_DDR) | (1 << MOTOR_LEFT_PORT_DDR);	// PWM port
	
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);	// 64분주 설정 clock setting
	// 16000000Hz / 64 --> 250000Hz (timer 1에 공급되는 clock)
	// 256 / 250000 --> 1.02ms	(펄스 256개 count --> 1.02ms)
	// 127 / 250000 --> 0.5ms
	// 0x3ff(1023) --> 4ms --> full speed
	
	TCCR1B |= (1 << WGM13) | (1 << WGM12);	 // mode 14 고속 PWM ICR1
	TCCR1A |= (1 << WGM11) | (0 << WGM10);	 // mode 14 고속 PWMS ICR1
	TCCR1A |= (1 << COM1A1) | (0 << COM1A0);	// 비반전 모드 : OCR LOW TOP : HIGH
	TCCR1A |= (1 << COM1B1) | (0 << COM1B0);	// 비반전 모드 : OCR LOW TOP : HIGH
	
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0); // 자동차 전진 모드로 세팅
	ICR1 = 0x3ff;	
}