# DMA-Demystified

### 26/08/26
Today I learned about master and slave communication and types of DMA tansfer(fly-by and fetch-and-deposit)

### 27/28/26
Today I learned about the masters and slaves available in my stm32 board.
### Doubt - Is dma1 and dma2 wired differently?\
It seems so is the case, dma1 peripheral bus is not a master.\
M2M fails to init on dma1 or returns failed configuration errors\ 
### Confirmed the fact on Multi-AHB matrix diagram, dma1 periph is directly wired to 

### Doubt - what is the difference between ahb and apb?
AHB - Advanced High-Performance Bus(High bandwidth system modules)
APB - Advanced Peripheral Bus(Low bandwidth and low power peripherals)

### Doubt - what is I,D and S bus?
I - Instruction code(Fetches instructions)
D - Data code(Reads constants/literals and vector tables from flash memory)
S - System bus(Reads or writes data to RAM and peripherals or executes code from RAM)


### Try initialising M2M on dma1 during practical