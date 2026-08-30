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