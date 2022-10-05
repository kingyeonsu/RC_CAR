/*
 * UART0.h
 *
 * Created: 2022-08-09 오전 10:45:22
 *  Author: kccistc
 */ 


#ifndef UART0_H_
#define UART0_H_

void init_uart0(void);
void pc_command_processing();
void UART0_transmit(uint8_t data);

// UART0로 부터 1 byte 들어오면 rx(수신)
// P278 12-3 참조

volatile unsigned char rx_buff[100];
volatile int i = 0;
volatile uint8_t rx_ready_flag = 0;	// 완전한 문장이 들어 왔을때 1로 set


#endif /* UART0_H_ */