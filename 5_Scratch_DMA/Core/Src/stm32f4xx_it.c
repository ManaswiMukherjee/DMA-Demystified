#include "stm32f4xx.h"

// interrupt handler for button
void EXTI0_IRQHandler(void)
{

	// clearing pending register bit when interrupt is called
	if(EXTI->PR & 1)
	{
		EXTI->PR |= EXTI_PR_PR0;
	}
}
