/*
 * UART0.c
 *
 * Created: 2022-08-09 오전 10:45:08
 *  Author: kccistc
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>		// strncmp, strcpy ...

#include "UART0.h"

extern int led_command;
extern int count_300ms;

// 받기 --> 읽기, 쓰기
ISR(USART0_RX_vect)
{
	unsigned char data;
	
	data = UDR0;	// UART0 HW register 로 1byte 씩 읽어 들인다.
	
	if (data == '\r' || data == '\n')
	{
		rx_buff[i] = '\0';	// 문장의 끝을 가리키는 NULL
		i = 0;		// 개선점 : circular queue (환형큐)로 개선 해야한다.
					// 이렇게 하면 새로운 메세지가 오면 덮어 쓴다.
		rx_ready_flag = 1;	// 완전한 문장이 들어 왔을떄 1로 set 된다.
	}
	else
	{
		rx_buff[i++] = data;	// 1. rx_buff[i] = data		2. i++
	}
}
// 1. 전송속도 : 9600bps	총 byte(글자) 수 : 9600 / 10 --> 960자 (1글자 송수신 소요 시간 약 1ms)
// 2. 비동기식, data 8bit, none parity
// 3. Rx interrupt 활성화
//
void init_uart0(void)
{
	UBRR0H = 0x00;
	UBRR0L = 207;	// 9600bps
	
	UCSR0A |= (1 << U2X0);	// 2배속 통신
	UCSR0C |= 0x06;		// ASYNC(비동기) data 8 bit, none parity
	
	// RXEN0 : UART0로 부터 수신이 가능하도록 설정
	// TXEN0 : UART0로 부터 송신이 가능하도록 설정
	// RXCIE0 : UART0로 부터 1 BYTE가 들어오면 RX(수신) interrupt 발생
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);	 
}

// UART0로 1 byte 보내는 함수
void UART0_transmit(uint8_t data)
{
	while( !(UCSR0A & (1 << UDRE0)))	// data가 전송 중이면 data가 다 전송될 때까지 기다린다.
		;	// no operation | 아무 action도 취하지 않는다.
	UDR0 = data;	// HW 전송 register에 data를 쏴준다.
}

// 1. command를 함수화 하여 led.c에 배치한다.
// ledallon
// ledalloff
// ledalltoggle : 300ms 주기로 led 전체를 on/off 반복한다.
void pc_command_processing()
{
	if (rx_ready_flag)	// if (rx_ready_flag
	{
		rx_ready_flag = 0;
		printf("%s\n", rx_buff);
		if (strncmp(rx_buff,"reset", strlen("reset")-1) == 0)
		{
			led_command = 0;
			count_300ms = 0;
		}
		if (strncmp(rx_buff,"led_all_toggle", strlen("led_all_toggle")-1) == 0)
		{
			led_command = 1;
			count_300ms = 0;
		}
		if (strncmp(rx_buff,"shift_left_led_on", strlen("shift_left_led_on")-1) == 0)
		{
			led_command = 2;
			count_300ms = 0;
		}
		if (strncmp(rx_buff,"shift_right_led_on", strlen("shift_right_led_on")-1) == 0)
		{
			led_command = 3;
			count_300ms = 0;
			
		}
		if (strncmp(rx_buff,"shift_left_hold_led_on", strlen("shift_left_hold_led_on")-1) == 0)
		{
			led_command = 4;
			count_300ms = 0;			
		}
		if (strncmp(rx_buff,"shift_right_hold_led_on", strlen("shift_right_hold_led_on")-1) == 0)
		{
			led_command = 5;
			count_300ms = 0;			
		}
		if (strncmp(rx_buff,"flower_off", strlen("flower_off")-1) == 0)
		{
			led_command = 6;
			count_300ms = 0;			
		}
		if (strncmp(rx_buff,"f_on", strlen("f_on")-1) == 0)
		{
			led_command = 7;
			count_300ms = 0;
		}
		
	}
	switch(led_command)
	{
		case 0:
			reset();
		break;
		case 1:
			led_all_toggle();
		break;
		case 2:
			shift_left_led_on();
		break;
		case 3:
			shift_right_led_on();
		break;
		case 4:
			shift_left_hold_led_on();
		break;
		case 5:
			shift_right_hold_led_on();
		break;
		case 6:
			flower_off();
		break;
		case 7:
			flower_on();
		break;
	}
}

