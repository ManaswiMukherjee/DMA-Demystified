#include <stdint.h>
#include "stm32f4xx.h"

// function prototypes
void button_init(void);
void uart2_init(void);
void dma1_init(void);

// main function
int main(void)
{
	// setup
	button_init();
	uart2_init();
	dma1_init();


	while(1)
	{
		//loop code
	}
}




void button_init(void)
{
	// enabling GPIOA peripheral clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// user button (KEY) is connected to PA0 pin in black pill, configure as input
	GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;

	// configuring pull-up for PA0
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0;

	// enabling AHB2 clock for SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// configuring exti0 for GPIOA using SYSCFG
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0_Msk;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA; // redundant line but good practice

	// clearing the pending register
	EXTI->PR |= EXTI_PR_PR0; // bit is cleared by programming it to 1

	// configure edge detection (falling edge)
	EXTI->RTSR &= ~EXTI_RTSR_TR0_Msk;		// clearing rising register as precaution
	EXTI->FTSR |= EXTI_FTSR_TR0;

	// enable interrupt
	EXTI->IMR |= EXTI_IMR_IM0;

	// enable the IRQ related to GPIO in NVIC
	NVIC_EnableIRQ(EXTI0_IRQn);
}

void uart2_init(void)
{
	// enabling clock for UART peripheral
	//RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // redundant but doing for safety

	// configuring alternate function for GPIO
	//GPIOA->MODER |= GPIO_AF7_USART2;

	//

	//
}

void dma1_init(void)
{
	// enabling dma1 peripheral lock
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	//

	//

	//
}
