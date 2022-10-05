/*
 * button.c
 *
 * Created: 2022-08-04 오후 4:12:18
 *  Author: kccistc
 */

// button toggle 

#include "button.h"
#include "led.h"

int get_button1();
void auto_mode_check();

int button1_state=0;

void auto_mode_check()
{
	if (get_button1())
	{
		button1_state = !button1_state;	// toggle
	}
	if (button1_state)
	{
		AUTO_RUN_LED_PORT |= 1 << AUTO_RUN_LED_PIN;
	}
	else
	{
		AUTO_RUN_LED_PORT &= ~(1 << AUTO_RUN_LED_PIN);
	}
}

void init_button()
{
	BUTTON_DDR &= ~((1 << BUTTON1)); // input --> 0 setting
}

int get_button1()
{
	static uint8_t prev_state=0; // 버튼의 상태 0: off 1: on
	uint8_t current_state;	// unsigend char 와 같다. (8비트) <open source type>
	
	current_state = BUTTON_PIN & (1 << BUTTON1); // button1 값을 읽는다. on --> 0x10
	
	if (prev_state == 0 && current_state != 0)
	{
		_delay_ms(60);	// ** NOISE가 지나가길 기다린다.
		prev_state = 1; // 처음 눌러진 상태가 아니다.
		
		return 0;	// 아직 진짜가 아니다. --> 다시 버튼이 눌린 효과 --> 초기값 1로 설정
	}
	else if (prev_state == 1 && current_state == 0) // 버튼이 눌렸다prev_state == 1 떠이진 상태 current_state == 0 
	{
		prev_state = 0;
		_delay_ms(30);	// 노이즈가 지나가기를 기다린다.
		return 1;	// 완전히 눌렸다 떼어진 상태로 인정 
	}
	
	return 0; // 어작 S/W를 완전히 눌렀다 때지 않은 상태
}

