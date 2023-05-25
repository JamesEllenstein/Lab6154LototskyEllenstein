/*
 * ECE 153B - Spring 2023
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */
 
#include "DMA.h"
#include "CRC.h"
#include "SysTimer.h"

void DMA_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	delay(1);
	////////////////////////////(DMA1_BASE + 0x08 + 0xd20 * 5) &= ~DMA_CCR_EN;
	DMA1_Channel6->CCR &= ~DMA_CCR_EN;
	DMA1_Channel6->CCR &= ~DMA_CCR_PL; 
	DMA1_Channel6->CCR |= DMA_CCR_PL_1;
	DMA1_Channel6->CCR &= ~DMA_CCR_PSIZE; 
	DMA1_Channel6->CCR |= DMA_CCR_PSIZE_1;
	DMA1_Channel6->CCR &= ~DMA_CCR_MSIZE; 
	DMA1_Channel6->CCR |= DMA_CCR_MSIZE_1;
	DMA1_Channel6->CCR &= ~DMA_CCR_PINC;
	DMA1_Channel6->CCR |= DMA_CCR_MINC;
	DMA1_Channel6->CCR &= ~DMA_CCR_CIRC;
	DMA1_Channel6->CCR |= DMA_CCR_DIR;
	DMA1_Channel6->CCR &= ~DMA_CCR_HTIE;
	DMA1_Channel6->CCR &= ~DMA_CCR_TEIE;
	DMA1_Channel6->CCR |= DMA_CCR_TCIE;
	DMA1_Channel6->CCR |= DMA_CCR_MEM2MEM;
	
	DMA1_Channel6->CMAR = (uint32_t)DataBuffer;
	DMA1_Channel6->CPAR = (uint32_t)&(CRC->DR);
	
	NVIC_EnableIRQ(DMA1_Channel6_IRQn); //enable irq
	NVIC_SetPriority(DMA1_Channel6_IRQn, 0);

}

void DMA1_Channel6_IRQHandler(void){
	NVIC_ClearPendingIRQ(DMA1_Channel6_IRQn);	
	if ((DMA1->ISR & DMA_ISR_TCIF6) == DMA_ISR_TCIF6) {
		DMA1->IFCR |= DMA_IFCR_CTCIF6;
	}
	if ((DMA1->ISR &  DMA_ISR_GIF6) ==  DMA_ISR_GIF6) {
		DMA1->IFCR |= DMA_IFCR_CGIF6;
	}
	
	completeCRC(CRC->DR);
}

