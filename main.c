/*
 * RC_CAR.c
 *
 * Created: 2022-08-18 오전 9:48:45
 * Author : kccistc
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h> 
#include <avr/interrupt.h>
#include <stdio.h>		// printf, scanf 등이 정의 되어 있다.
#include <string.h>		// strcpy, strncmp, strcat 등이 들어 있다.

// #include "UART0.h" --> 중복 선언으로 사용 불가

void init_timer0();
void auto_drive(void);

extern volatile uint8_t rx_ready_flag;
extern int button1_state;

extern void init_led();
extern void init_button();
extern void init_uart0(void);
extern void init_UART1();
extern void UART0_transmit(uint8_t data);
//extern void pc_command_processing();	// pc_command_processing은 다른 파일에 있다고 complier 에게 알려준다.
extern void init_pwm_motor(void);
extern void manual_mode_run(void);
extern void init_ultrasonic();
extern void distance_check(void);
extern void auto_mode_check();
extern void forward(int speed);
extern void backward(int speed);
extern void turn_left(int speed);
extern void turn_right(int speed);
extern void back_left(int speed);
extern void back_right(int speed);
extern void stop();
extern void turn_left_auto(int speed1, int speed2);
extern void turn_right_auto(int speed1, int speed2);

// 1. for printf 
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int count_50ms=0;
int ultra_sonic_state=0;

extern volatile int ultrasonic_left_distance;
extern volatile int ultrasonic_forward_distance;
extern volatile int ultrasonic_right_distance;
extern volatile int ultrasonic_backward_distance;

ISR(TIMER0_OVF_vect)		// ineterrupt --> main 함수 앞에 배치
{
	// 분주비를 64로 설정했기 때문에 6부터 시작하면 ms 단위로 사용 가능
	TCNT0 = 6;		//	--> TIMER COUNT 0 = 6ms --> 6ms 부터 시작
	
	count_50ms++;

	if (count_50ms > 50)
	{
		count_50ms=0;
		ultra_sonic_state++;
		
		if (ultra_sonic_state > 3)
		{
			ultra_sonic_state=0;
			count_50ms=0;
		}
	}
	
}

int main(void)
{
	DDRF = 0x01 + 0x02 + 0x04;	// PF0, PF1, PF2 출력으로 설정
	PORTF = 0x00;		// LED를 끈 상태에서 시작	
	
	init_led();
	init_button();
	init_uart0();	// uart0 초기화
	init_UART1();	// uart1 초기화
	init_timer0();
	init_pwm_motor();
	init_ultrasonic();
	
	stdout = &OUTPUT;	// 2. for printf	fprintf(stdout, "test"); --> printf		stdin --> scanf
	sei();						// 전역적으로 인터럽트 허용
	
    while (1) 
    {
		distance_check();	//	초음파 거리 측정
		manual_mode_run();	//	BLUETOOTH CAR COMMAMD RUN
		auto_mode_check();	//	button1 check
		
		if (button1_state)
		{	
			auto_drive();
		}
    }
	
}

// 자율 주행 프로그램 
void auto_drive(void)
{
	int limit = 10;
	
	if (ultrasonic_forward_distance > limit*2.5)
	{
		if (ultrasonic_left_distance > ultrasonic_right_distance)
		{
			turn_left_auto(1023, 200);
		}
		else
		{
			turn_right_auto(1023, 200);
		}
	}
	else
	{
		if (ultrasonic_left_distance > ultrasonic_right_distance)
		{
			back_right(1023);
		}
		else
		{
			back_left(1023);
		}
	}
/*	if (ultrasonic_backward_distance < limit*1.5)
	{
		forward(1023);
	}
	
	if ((ultrasonic_backward_distance < limit) && (ultrasonic_left_distance < limit) && (ultrasonic_right_distance < limit))
	{
		stop();
	}*/
}

void init_timer0()
{
	TCNT0 = 6; // TCNT 6 ~ 256 --> 정확히 1ms 마다 TIMT 0 OVF INT
		
	// 분주비 64로 설정
	TCCR0 |= (1 << CS02) | (0 << CS01) | (0 << CS00);
		
	//	인터럽트 활성화 비트 TIMSK (Timer/Counter Interrupt Mask Register)
	//	그림 13-5 참고
	//	오버플로 인터럽트 활성화(허용) 비트 TOIE0 (Timer/Counter0 Overflow Interrupt Enable)
	//	TIMER0_OVF_vect -->	TOIE0
	TIMSK |= (1 << TOIE0);		// 오버플로 인터럽트 허용
}

