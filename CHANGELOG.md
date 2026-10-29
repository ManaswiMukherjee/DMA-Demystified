# DMA-Demystified

## 26/08/26
Today I learned about master and slave communication and types of DMA tansfer(fly-by and fetch-and-deposit)

## 27/28/26
Today I learned about the masters and slaves available in my stm32 board.
### Is dma1 and dma2 wired differently?\
It seems so is the case, dma1 peripheral bus is not a master.\
M2M fails to init on dma1 or returns failed configuration errors\ 
### Confirmed the fact on Multi-AHB matrix diagram, dma1 periph is directly wired to 

### Try initialising M2M on dma1 during practical

### what is the difference between ahb and apb?
AHB - Advanced High-Performance Bus(High bandwidth system modules)
APB - Advanced Peripheral Bus(Low bandwidth and low power peripherals)

### what is I,D and S bus?
I - Instruction code(Fetches instructions)
D - Data code(Reads constants/literals and vector tables from flash memory)
S - System bus(Reads or writes data to RAM and peripherals or executes code from RAM)


## 28/08/26
### What is arbitration?
It is the situation where two bus masters simulataneously try to take control of the bus, then the priority of the masters is taken into account. And as dma has FIFO it can wait and store those with data after the cpu is done accessing the bus.

### DMA does not use intstruction to tranfer memory, instead it used specialized hardware to do so.

## 29/08/26

### Wierd naming convention of ST for DMA
MEM TO MEM - It is used for sram to peripheral as MEM to PERI is used when we transfer data from the memory to directly the apb bus and not through the ahb bus.\
ahb peripheral is considered as memory and not a peripheral.

## 30/08/26
LED does not blink with code


### changing data-size from 1 to 2 wouldn't help because auto increment for source and destination was turned off.

```c
	//turning led off
	if(HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t)&led_dat[0], (uint32_t)&GPIOC->ODR, 1) != HAL_OK){
		while(1){__NOP();}
	}
	if(HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY) != HAL_OK){
		while(1){__NOP();}
	}

	HAL_Delay(1000);

	//turning led on
	if(HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t)&led_dat[1], (uint32_t)&GPIOC->ODR, 1) != HAL_OK){
		while(1){__NOP();}
	}
	if(HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY) != HAL_OK){
		while(1){__NOP();}
	}

	HAL_Delay(1000);
```

## Register problem - Code will not be same as kiran's

### Had to change the dma configuration in cubemx from byte transfer to word transfer and changed led_dat from `{0x00, 0xff0}` to `{0x0000, 0x2000}`

### And changed from `HAL_Delay(1000)` to non blocking delay structure.

```c
cur_ticks = HAL_GetTick();
	while(HAL_GetTick() < cur_ticks + 1000){__NOP();}
```

### Incorrect `HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream0, HAL_DMA_XFER_CPLT_CB_ID, &dma_full_transfer_cplt(*_hdma));`

### Correct `HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream0, HAL_DMA_XFER_CPLT_CB_ID, &dma_full_transfer_cplt);`

We are passing the funciton as a pointer.


## Assignment SRAM to SRAM, transfer data from one array to another

### Initial code does not work, only one value gets transferred.
Error: Target not available
Try to see where the flow is going

### Periodic transfer or one-shot transfer? ANS - Periodic 5 sends with 1sec gaps
### Increment works for only single dma transfer calls not seperate

### Keep in mind the size of the data to be transferred in a single dma start call

### SRAM to SRAM works is verified led blink and not through debugger as it is creating problems. The debugger works for max upto 4 then stops and shows target not available but the current LED test works.

## 04/09/26
### 2_IT_UART
### The memory was updating with old messages when debugger was refreshed and desired condition was not found.
#### This was caused due to stm32cubeide's non auto refresh nature.
#### When `*(uint8_t[100]*)0x20000800` is added as a live expression it does show up immediately when sent from serial monitor terminal.

### In the course Keil is used which auto refreshes memory.

## 12/09/26
### Completed and tested SRAM to UART exercise and started the scratch project for the same (Last section).

## Starting dma scratch exercise section
### While doing bare-metal/CMSIS programming it is not strictly necessary to configure the clock of the mcu as when the board boots up it automatically selects the default clock config and operates with it if nothing is configured through code.

## 13/09/26 - SRAM to UART scratch programming
## How to confiure edge detection?
### To do this we need to configure interrupts for edge detection which is done by the EXTI(External Interrupt) Falling Trigger selection register.

## There are only 22 exti bits in the ftsr register are they for A,B,C all at once?
### Not all at once but only one. They bits are by line number and multiplexed like bit0 - MUX(PA0, PB0, PC0, PD0, PE0, PH0)

## How to select the pins for the exti line
### This is done by using the SYSCFG register(See RM).

### Example
```c
// configuring exti0 for GPIOA using SYSCFG
SYSCFG->EXTICR[0] &= ~(0xF << 0);
```

## What is interrupt mask?
### This is the register to enable the interrupts for the specific line.

## What is event mask then?
### EMR is the same idea as IMR, but for events instead of interrupts.

   * IMR lets the EXTI line generate an interrupt
   * EMR lets the EXTI line generate an event

An event is a lighter signal used by some MCU features, often to wake the CPU or trigger hardware behavior without going through the normal interrupt handler. If you’re just handling a button press in an ISR, you usually only need `IMR`, not `EMR`. 

## How to enable the IRQ related to GPIO in NVIC?
### `NVIC_EnableIRQ(EXTI0_IRQn);` is the way to do that. Note that this is not a pointer but a function that cmsis provides for portability and safety.

## Missing SYSCFG Clock Enable

### Additions to code written by self
* configure PA0 with a pull-up.
* clear any pending EXTI0 flag before enabling.
* optionally clear the rising trigger bit if you only want falling edge.

## Interrupt gets called without even pressing the button
### WE are configuring the edge detection for the exti without first telling which pin to multiplex. But my code already follows this standard, `then why`?
### This was happening due to the uart function that was defined, problem was solved by commenting out the function.


## 27/10/26
## Why does 0x08 come up as a expansion of `GPIO_AF8_USART6`?
### Wrong should have written `GPIO_MODER_...`
silly mistake

## How many stop bits?

## How to configure the baud rate?
### Using the formula `USARTDIV = USART_CLK / (16 × desired_baud_rate)`

## Now from where to find the USART_CLK?
### It can be found from the clock tree given in `6.2` and specifically the APB2 bus feeds the USART6 peripheral 

## What is Mantissa?
### This is the integer portion of a decimal number


## Do we need pull-up or pull-down on the pins?
### In any type of communication be it I2C, SPI and UART we always need pull up because we need the voltage to be kept high.

## Output was not comming when testing for usart check
### Send logic was inverted for send_data function

## Can one enable the ue before configuring the usart cr registers or does one have to strictly do them after configuring?
### TE bit was commented out