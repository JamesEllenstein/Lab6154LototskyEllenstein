/*
 * ECE 153B - Spring 2023
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "DMA.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
static uint32_t uwExpectedCRCValue = 0x5A60861E;	
static volatile uint32_t ComputedCRC;	
static volatile uint32_t ComputationDone = 0;	

void completeCRC(uint32_t crc){
	ComputedCRC = crc;
	ComputationDone = 1;
}

int main(void) {
  uint32_t time;
	
	System_Clock_Init(); 
	SysTick_Init();
	LED_Init();	
	CRC_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
	DMA_Init();
	while(1) {
		LED_Toggle();
		startTimer();
		ComputationDone = 0;
		CRC->CR |= CRC_CR_RESET;
		DMA1_Channel6->CNDTR = BUFFER_SIZE;
		DMA1_Channel6->CCR |= DMA_CCR_EN;
		while (ComputationDone == 0) {}
		DMA1_Channel6->CCR &= ~DMA_CCR_EN;
		time = endTimer();
		// if CRC doesn't match expected CRC, turn LED off, break
		if (ComputedCRC != uwExpectedCRCValue) {
			LED_Off();
			break;
		}
		printf("%d \n" , time);
		// delay
		delay(1000);
	}
	
  while (1); 
}


