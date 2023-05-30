/*
 * ECE 153B - Spring 2023
 *
 * Name(s):
 * Section:
 * Lab: 6A
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
#define uwExpectedCRCValue 0x5A60861E

static volatile uint32_t Software_ComputedCRC;

int main(void) {
	
	// Switch System Clock = 80 MHz
	System_Clock_Init();
	SysTick_Init();
	LED_Init();
	UART2_Init();
	UART2_GPIO_Init();
	USART_Init(USART2);

	// TODO initialize modules
	while(1) {
		uint32_t tim = 0;
		LED_Toggle();
		// initialize CRC
		uint32_t crc = INITIAL_CRC_VALUE;
		// start timer
		startTimer();
		// compute CRC
		for (int i = 0; i < BUFFER_SIZE; i++) {
			crc = CrcSoftwareFunc(crc,DataBuffer[i],POLYNOME);
			
		// check CRC

		
		}
		// end timer
		tim = endTimer();
		if (crc != uwExpectedCRCValue) {
			LED_Off();
			break;
		}
		// print time
		printf("%d \n" , tim);
		// delay
		delay(1000);
	}
}
