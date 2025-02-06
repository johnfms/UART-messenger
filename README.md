# UART Messenger

A two-way messaging system implemented on AVR microcontrollers with LCD display and matrix keyboard input. Created by John.

## Features

- Bi-directional UART communication
- LCD display support (16x2 characters)
- 5x6 matrix keyboard input
- Message framing with line numbers
- Auto-scrolling display
- Interrupt-driven message reception
- Support for special characters (?, !, space)

## Hardware Requirements

- AVR microcontroller (8MHz clock)
- 16x2 LCD display
- 5x6 matrix keyboard
- UART communication interface

### Pin Connections

#### LCD
- Data Port: PORTC
- Control Port: PORTD
  - RS: PD5
  - RW: PD6
  - E: PD7

#### Keyboard
- Row Port: PORTB
- Column Port: PORTA

## Software Dependencies

- AVR-GCC compiler
- AVR Standard Libraries
  - `<avr/io.h>`
  - `<avr/interrupt.h>`
  - `<util/delay.h>`
  - `<string.h>`

## Configuration

The system operates with the following specifications:
- CPU Clock: 8MHz
- UART Baud Rate: 9600
- Maximum Message Length: 16 characters
- Frame Format: `[line_number]-[message]\n`

## Usage

1. Messages can be typed using the matrix keyboard
2. Press '*' to send the message
3. Received messages are automatically displayed on the LCD
4. Messages alternate between the first and second LCD lines
5. Screen automatically clears after 2 seconds when returning to line 1

### Keyboard Layout
```
A B C D E F
G H I J K L
M N O P Q R
S T U V W X
Y Z ? ! [space] *
```

## Message Protocol

Messages are framed in the following format:
- Outgoing: `[line_number]-[message]\n`
- Incoming: Displayed with '=' prefix
- Maximum message length: 16 characters

## Files Structure

- `uart_messenger.c` - Main application code
- `lcd.h` - LCD driver library
- `keyboard.h` - Matrix keyboard driver
- `uart.h` - UART communication library

## Building and Flashing

Compile using AVR-GCC:
```bash
avr-gcc -mmcu=your_mcu -DF_CPU=8000000UL -o uart_messenger.elf uart_messenger.c
avr-objcopy -O ihex uart_messenger.elf uart_messenger.hex
```

## Contributing

Feel free to submit issues and enhancement requests.

## License

This project is released under the MIT License.

## Author

Created by John on September 9, 2024.
