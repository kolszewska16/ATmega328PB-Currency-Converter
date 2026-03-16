# ATmega328PB-Currency-Converter
Embedded firmware project implementing a simple currency converter on the **ATmega328PB** microcontroller. The system converts values between predefined currencies using stored exchange rates and displays the result through a simple embedded interface.

This project was developed as part of the **Microprocessor Technology 1** course at **AGH University of Krakow**.

### Features
- Currency conversion using predefined exchange rates
- Implementation in C for AVR microcontrollers
- Designed for **ATmega328PB**
- Lightweight embedded architecture
- Simple user interaction
- Clear modular firmware structure

### Hardware
- Target microcontroller: **ATmega328PB**
- 8-bit AVR RISC architecture
- 32 KB Flash memory
- 2 KB SRAM
- Multiple peripherals (UART, SPI, timers, ADC)

The firmware can be adapted to run on:
- ATmega328PB development boards
- Custom AVR-based hardware
- Arduino-compatible boards (with minor modifications)

### Software Stack
- Language: **C**
- Toolchain: **AVR-GCC**
- IDE: **Microchip Studio / Atmel Studio / AVR toolchain**
- Target architecture: **AVR 8-bit**

### Project Structure
```
.
├── src/        # Source files
├── include/    # Header files
├── main.c      # Main application entry point
└── README.md
```

### How It Works
1. The system initializes the microcontroller and required peripherals.
2. A base currency value is entered or selected.
3. The program applies predefined exchange rates.
4. The converted value is calculated.
5. The result is presented through the output interface.