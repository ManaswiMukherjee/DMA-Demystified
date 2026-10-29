#include "stm32f4xx.h"

extern void HT_cplt_callback(void);
extern void TC_cplt_callback(void);
extern void TE_cplt_callback(void);
extern void DME_cplt_callback(void);
extern void FE_cplt_callback(void);




#define is_it_HT() DMA2->HISR & DMA_HISR_HTIF6
#define is_it_TC() DMA2->HISR & DMA_HISR_TCIF6
#define is_it_TE() DMA2->HISR & DMA_HISR_TEIF6
#define is_it_DME() DMA2->HISR & DMA_HISR_DMEIF6
#define is_it_FE() DMA2->HISR & DMA_HISR_FEIF6

// interrupt handler for button
void EXTI0_IRQHandler(void)
{
	// sending the data
	USART6->CR3 |= USART_CR3_DMAT;


	// clearing pending register bit when interrupt is called
	if(EXTI->PR & 1)
	{
		EXTI->PR = EXTI_PR_PR0;
	}
}

void DMA2_Stream6_IRQHandler(void)
{
	if(is_it_HT()){
		DMA2->HIFCR |= DMA_HIFCR_CHTIF6;
		HT_cplt_callback();
	}
	else if(is_it_TC()){
		DMA2->HIFCR |= DMA_HIFCR_CTCIF6;
		TC_cplt_callback();
	}
	else if(is_it_TE()){
		DMA2->HIFCR |= DMA_HIFCR_CTEIF6;
		TE_cplt_callback();
	}
	else if(is_it_DME()){
		DMA2->HIFCR |= DMA_HIFCR_CDMEIF6;
		DME_cplt_callback();
	}
	else{
		DMA2->HIFCR |= DMA_HIFCR_CFEIF6;
		FE_cplt_callback();
	}
}
