#include <stdint.h>
#include "stm32f4xx.h"

// variables
char data[] = "Hello World\r\n";


// function prototypes
void button_init(void);
void usart6_init(void);
void send_data(void);
void dma2_init(void);
void dma2_en_stream(void);
void dma2_int_conf(void);

// callback prototypes
void HT_cplt_callback(void);
void TC_cplt_callback(void);
void TE_cplt_callback(void);
void DME_cplt_callback(void);
void FE_cplt_callback(void);


// main function
int main(void)
{
	// setup
	button_init();
	usart6_init();
	dma2_init();
	dma2_int_conf();
	dma2_en_stream();


	while(1)
	{
		//loop code
	}
}

// test function for uart
void send_data(void)
{
	uint32_t i = 0;
	/* condition to check whether the TXE is 1 or not
	 * if it is 1 then transferring the byte to the TDR.
	 */
	for(i=0; i<sizeof(data); i++)
	{
		while(!(USART6->SR & (1 << 7)));
		USART6->DR = data[i];
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

void usart6_init(void)
{
	/* Using pins 11 and 12 for UART */
	// USART6

	// enabling clock for GPIOA peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // redundant but doing for safety

	// enabling clock for UART peripheral
	RCC->APB2ENR |= RCC_APB2ENR_USART6EN;

	// configuring alternate function for GPIO
	GPIOA->MODER &= ~(GPIO_MODER_MODE11_Msk | GPIO_MODER_MODE12_Msk);
	GPIOA->MODER |= (GPIO_MODER_MODE11_1 | GPIO_MODER_MODE12_1);

	// specifying the alternate function for the pins
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL11_Msk | GPIO_AFRH_AFSEL12_Msk);
	GPIOA->AFR[1] |= (GPIO_AFRH_AFSEL11_3 | GPIO_AFRH_AFSEL12_3);

	// configuring pull up for the pins
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD11_Msk | GPIO_PUPDR_PUPD12_Msk);
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPD11_1 | GPIO_PUPDR_PUPD12_1);

	// These steps are given in the RM
	// 1.Enable the USART by writing the UE bit in USART_CR1 register to 1.
	USART6->CR1 |= USART_CR1_UE;

	// 2.Program the M bit in USART_CR1 to define the word length.
	// Using word length 8
	USART6->CR1 &= ~USART_CR1_M_Msk;

	// 3.Program the number of stop bits in USART_CR2.
	// By default stop bit is 1 so not configured separately here.

	/* 4.Select DMA enable (DMAT) in USART_CR3 if Multi-buffer Communication is to take
		place. Configure the DMA register as explained in multi-buffer communication.*/
	// 5.Select the desired baud rate using the USART_BRR register.
	// using value given in data-sheet for 115200
	USART6->BRR = 0x8B;

	// 6.Set the TE bit in USART_CR1 to send an idle frame as first transmission.
	USART6->CR1 |= USART_CR1_TE;

	/* 7.Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
	for each data to be transmitted in case of single buffer.*/
	/*8.After writing the last data into the USART_DR register, wait until TC=1. This indicates
	that the transmission of the last frame is complete. This is required for instance when
	the USART is disabled or enters the Halt mode to avoid corrupting the last
	transmission.*/
}

void dma2_init(void)
{
	// 1. enabling dma2 peripheral clock
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	// 2. identify the stream
	/* USING STREAM 6 */

	// 3. identify the channel number on uart sends dma request
	DMA2_Stream6->CR &= ~DMA_SxCR_CHSEL_Msk;
	DMA2_Stream6->CR |= (5 << 25);

	// 4. program the source address (memory)
	DMA2_Stream6->M0AR = (uint32_t)data;

	// 5. program the destination address (UART)
	DMA2_Stream6->PAR = (uint32_t)&USART6->DR;

	// 6. program the number of data items to send
	uint32_t len = sizeof(data);
	DMA2_Stream6->NDTR = len;

	// 7. direction of data transfer
	DMA2_Stream6->CR &= ~DMA_SxCR_DIR_Msk;
	DMA2_Stream6->CR |= DMA_SxCR_DIR_0;

	// 8. program the source and destination data width
	/* Programming these as 8bit as USART DR supports 8 bits only */
	DMA2_Stream6->CR &= ~(DMA_SxCR_MSIZE_Msk | DMA_SxCR_PSIZE_Msk);

	// 8a. Enabling memory auto increment
	DMA2_Stream6->CR |= DMA_SxCR_MINC;

	// 9. select whether direct or FIFO mode
	DMA2_Stream6->FCR &= ~DMA_SxFCR_DMDIS_Msk;

	// 10. normal or circular mode

	// 11. single transfer or burst transfer

	// 12. priority

}

void dma2_en_stream(void)
{
	// 13. enable the stream
	DMA2_Stream6->CR |= DMA_SxCR_EN;
}

void dma2_int_conf(void)
{
	// enabling the IE bits for interrupt generation
	// enabled all 4 types of interrupts
	DMA2_Stream6->CR |= (DMA_SxCR_DMEIE | DMA_SxCR_TEIE | DMA_SxCR_HTIE | DMA_SxCR_TCIE);

	// enable the IRQ for dma2 stream6 in NVIC
	NVIC_EnableIRQ(DMA2_Stream6_IRQn);
}

void HT_cplt_callback(void)
{

}

void TC_cplt_callback(void)
{
	// putting NDTR value again to start again on button press
	uint32_t len = sizeof(data);
	DMA2_Stream6->NDTR = len;

	// disabling transfer before enabling stream
	USART6->CR3 &= ~USART_CR3_DMAT;

	dma2_en_stream();
}

void TE_cplt_callback(void)
{

}

void DME_cplt_callback(void)
{

}

void FE_cplt_callback(void)
{

}
