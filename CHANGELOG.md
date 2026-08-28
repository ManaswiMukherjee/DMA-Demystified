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