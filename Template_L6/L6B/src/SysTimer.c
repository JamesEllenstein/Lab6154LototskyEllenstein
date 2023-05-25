/*
 * ECE 153B - Spring 2023
 *
 * Name(s):
 * Section:
 * Lab: 6A
 */

#include "SysTimer.h"

static uint32_t volatile step;
static volatile uint32_t msTicks;
void SysTick_Init(void) {
	// SysTick Control & Status Register
	SysTick->CTRL = 0; // Disable SysTick IRQ and SysTick Counter
	SysTick->LOAD = 9999;    // [Done] 1ms, 80Mhz
	
	// SysTick Current Value Register
	SysTick->VAL = 0;
	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Select clock source
	// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
	// If CLKSOURCE = 1, the processor clock is used.
	// TODO
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		
	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1
}

void SysTick_Handler(void){
	++msTicks;
}
	
//******************************************************************************************
// Delay in ms
//******************************************************************************************
void delay (uint32_t T){
	msTicks = 0;
	SysTick->VAL = 0;
	SysTick->LOAD = 79999;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while (msTicks < T) {}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void startTimer(void) {
	msTicks = 0;
	SysTick->VAL = 0;
	SysTick->LOAD = 79999;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

uint32_t endTimer(void) {
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	
	return (79999 - SysTick->VAL) / 80 + 1000 * msTicks;
}
