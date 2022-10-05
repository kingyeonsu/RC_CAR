/*
 * ultrasonic.c
 *
 * Created: 2022-08-10 오후 4:22:18
 *  Author: kccistc
 */ 
#include "ultrasonic.h"


extern int count_50ms;
extern int ultra_sonic_state;

void ultrasonic_trigger();

// 왼쪽 초으파 PE4 : 외부 INT4 초음파 센서 상승 하강 엣지 둘자 이쪽으로 들어온다.
// 초음파 : 1cm를 이동하는데 29us 소요 --> 1cm 왕복 : 29 * 2 = 58us 소요
// 16000000Hz / 1024 = 15,625Hz
// T = 1/f --> 6.4us
ISR(INT4_vect)
{
	if (ECHO_PIN_LEFT & (1 << ECHO_LEFT))	// 상승엣지
	{
		TCNT3 = 0;
	}
	else   // 하강엣지
	{
		// 에코핀의 펄스 길이를 ms 단위로 환산
		// 1cm : 58us 소요
		ultrasonic_left_distance = 1000000.0 * TCNT3 * 1024 / F_CPU / 58;
	}
}

// 외부 INT5: 전방 초음파
ISR(INT5_vect)
{
	if (ECHO_PIN_FORWARD & (1 << ECHO_FORWARD))	// 상승엣지
	{
		TCNT3 = 0;
	}
	else   // 하강엣지
	{
		// 에코핀의 펄스 길이를 ms 단위로 환산
		// 1cm : 58us 소요
		ultrasonic_forward_distance = 1000000.0 * TCNT3 * 1024 / F_CPU / 58;
	}
}

// 외부 INT6: 오른쪽 초음파
ISR(INT6_vect)
{
	if (ECHO_PIN_RIGHT & (1 << ECHO_RIGHT))	// 상승엣지
	{
		TCNT3 = 0;
	}
	else   // 하강엣지
	{
		// 에코핀의 펄스 길이를 ms 단위로 환산
		// 1cm : 58us 소요
		ultrasonic_right_distance = 1000000.0 * TCNT3 * 1024 / F_CPU / 58;
	}
}

// 외부 INT7: 후방 초음파
ISR(INT7_vect)
{
	if (ECHO_PIN_BACKWARD & (1 << ECHO_BACKWARD))	// 상승엣지
	{
		TCNT3 = 0;
	}
	else   // 하강엣지
	{
		// 에코핀의 펄스 길이를 ms 단위로 환산
		// 1cm : 58us 소요
		ultrasonic_backward_distance = 1000000.0 * TCNT3 * 1024 / F_CPU / 58;
	}
}


// 초음파 센서 초기화 함수
void init_ultrasonic()
{
	//======================== LEFT ULTRA SONIC===========================
	TRIG_DDR_LEFT |= (1 << TRIG_LEFT);	// output mode
	ECHO_DDR_LEFT &= ~(1 << ECHO_LEFT);	//  input mode
	
	// EICRA : INT 0~3
	// EICRB : INT 4~7
	// 0 1 : 어떠한 신호와 변화가 있든 신호 발생
	// RISING/FALLING EDGE 모두 INT 발생
	EICRB |= (0 << ISC41) | (1 << ISC40);
	TCCR3B |= (1 << CS32) | (1 << CS30);	// 1024분주
	// 16비트 timer 1번을 1024분주 해서 공급
	// 16MHz 를 1024분주 --> 16000000/1024 --> 15625Hz --> 15.625kHz
	// 1 pulse time : T =  1/f = 1/15625 = 0.000064sec = 64us
	
	EIMSK |= (1 << INT4);	// 외부 INT4(ECHO_LEFT) 사용
	
	//============================== FORWARD ULTRA SONIC========================
	TRIG_DDR_FORWARD |= (1 << TRIG_FORWARD);	// output mode
	ECHO_DDR_FORWARD &= ~(1 << ECHO_FORWARD);	//  input mode
	
	// EICRA : INT 0~3
	// EICRB : INT 4~7
	// 0 1 : 어떠한 신호와 변화가 있든 신호 발생
	// RISING/FALLING EDGE 모두 INT 발생
	EICRB |= (0 << ISC51) | (1 << ISC50);
	TCCR3B |= (1 << CS32) | (1 << CS30);	// 1024분주
	// 16비트 timer 1번을 1024분주 해서 공급
	// 16MHz 를 1024분주 --> 16000000/1024 --> 15625Hz --> 15.625kHz
	// 1 pulse time : T =  1/f = 1/15625 = 0.000064sec = 64us
	
	EIMSK |= (1 << INT5);	// 외부 INT5(ECHO_FORWARD) 사용
	
	//===================== RIGHT ULTRA SONIC =================================
	TRIG_DDR_RIGHT |= (1 << TRIG_RIGHT);	// output mode
	ECHO_DDR_RIGHT &= ~(1 << ECHO_RIGHT);	//  input mode
	
	// EICRA : INT 0~3
	// EICRB : INT 4~7
	// 0 1 : 어떠한 신호와 변화가 있든 신호 발생
	// RISING/FALLING EDGE 모두 INT 발생
	EICRB |= (0 << ISC61) | (1 << ISC60);
	TCCR3B |= (1 << CS32) | (1 << CS30);	// 1024분주
	// 16비트 timer 1번을 1024분주 해서 공급
	// 16MHz 를 1024분주 --> 16000000/1024 --> 15625Hz --> 15.625kHz
	// 1 pulse time : T =  1/f = 1/15625 = 0.000064sec = 64us
	
	EIMSK |= (1 << INT6);	// 외부 INT6(ECHO_RIGHT) 사용
	
	//===================== BACKWARD ULTRA SONIC =================================
	TRIG_DDR_BACKWARD |= (1 << TRIG_BACKWARD);	// output mode
	ECHO_DDR_BACKWARD &= ~(1 << ECHO_BACKWARD);	//  input mode
	
	// EICRA : INT 0~3
	// EICRB : INT 4~7
	// 0 1 : 어떠한 신호와 변화가 있든 신호 발생
	// RISING/FALLING EDGE 모두 INT 발생
	EICRB |= (0 << ISC71) | (1 << ISC70);
	TCCR3B |= (1 << CS32) | (1 << CS30);	// 1024분주
	// 16비트 timer 1번을 1024분주 해서 공급
	// 16MHz 를 1024분주 --> 16000000/1024 --> 15625Hz --> 15.625kHz
	// 1 pulse time : T =  1/f = 1/15625 = 0.000064sec = 64us
	
	EIMSK |= (1 << INT7);	// 외부 INT7(ECHO_BACKWARD) 사용
}

void distance_check(void)
{
	ultrasonic_trigger();
	
	printf("left: %3d cm\n", ultrasonic_left_distance);
	printf("forward: %3d cm\n", ultrasonic_forward_distance);
	printf("right: %3d cm\n", ultrasonic_right_distance);
	printf("backward: %3d cm\n", ultrasonic_backward_distance);
}

void ultrasonic_trigger()
{
	// us 단위는 상관 X // ms 단위만 변경
	switch (ultra_sonic_state)
	{
		case 0:
			//============= LEFT ULTRA SONIC ==========
			TRIG_PORT_LEFT &= ~(1 << TRIG_LEFT); // LOW
			_delay_us(1);
			TRIG_PORT_LEFT |= (1 << TRIG_LEFT); // HIGH
			_delay_us(15);
			TRIG_PORT_LEFT &= ~(1 << TRIG_LEFT); // LOW
			count_50ms=0;
		break;
		case 1:
			//============= CENTER ULTRA SONIC ==========
			TRIG_PORT_FORWARD &= ~(1 << TRIG_FORWARD); // LOW
			_delay_us(1);
			TRIG_PORT_FORWARD |= (1 << TRIG_FORWARD); // HIGH
			_delay_us(15);
			TRIG_PORT_FORWARD &= ~(1 << TRIG_FORWARD); // LOW
			count_50ms=0;
		break;
		case 2:
			//============= RIGHT ULTRA SONIC ==========
			TRIG_PORT_RIGHT &= ~(1 << TRIG_RIGHT); // LOW
			_delay_us(1);
			TRIG_PORT_RIGHT |= (1 << TRIG_RIGHT); // HIGH
			_delay_us(15);
			TRIG_PORT_RIGHT &= ~(1 << TRIG_RIGHT); // LOW
			count_50ms=0;
		break;
		case 3:
		//============= RIGHT ULTRA SONIC ==========
		TRIG_PORT_BACKWARD &= ~(1 << TRIG_BACKWARD); // LOW
		_delay_us(1);
		TRIG_PORT_BACKWARD |= (1 << TRIG_BACKWARD); // HIGH
		_delay_us(15);
		TRIG_PORT_BACKWARD &= ~(1 << TRIG_BACKWARD); // LOW
		count_50ms=0;
		break;
	}
//	_delay_ms(50);	// 초음파 센서 ECHO 최대 응답 시간 38ms
					// TIMER INTERRUPT로 처리 --> DELAY가 있어서는 안된다.
}




