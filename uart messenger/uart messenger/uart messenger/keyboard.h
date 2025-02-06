/*
 * keyboard.h
 *
 * Created: 9/9/2024 8:32:43 PM
 *  Author: john
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_
#include <avr/io.h>
#include <util/delay.h>

// Define the port for rows and columns
#define ROW_PORT PORTB
#define ROW_DDR  DDRB
#define ROW_PIN  PINB
#define COL_PORT PORTA
#define COL_DDR  DDRA

// Define the number of rows and columns
#define ROWS 5
#define COLS 6

// Define the keyboard layout
const char keyboard[ROWS][COLS] = {
	{'A', 'B', 'C', 'D', 'E', 'F'},
	{'G', 'H', 'I', 'J', 'K', 'L'},
	{'M', 'N', 'O', 'P', 'Q', 'R'},
	{'S', 'T', 'U', 'V', 'W', 'X'},
	{'Y', 'Z', '?', '!', ' ', '*'}  // '*' represents ENTER
};

// Function to initialize the keyboard
void keyboard_init() {
	// Set rows as inputs with pull-ups
	ROW_DDR = 0x00;
	ROW_PORT = 0xFF;
	
	// Set columns as outputs
	COL_DDR = 0xFF;
	COL_PORT = 0xFF;
}

// Function to scan the keyboard
char keyboard_read() {
	for (int col = 0; col < COLS; col++) {
		// Activate current column
		COL_PORT &= ~(1 << col);
		
		// Check each row
		for (int row = 0; row < ROWS; row++) {
			if (!(ROW_PIN & (1 << row))) {
				// Key pressed, wait for release
				_delay_ms(20);  // Debounce
				while (!(ROW_PIN & (1 << row)));
				
				// Deactivate column
				COL_PORT |= (1 << col);
				
				return keyboard[row][col];
			}
		}
		
		// Deactivate column
		COL_PORT |= (1 << col);
	}
	
	return 0;  // No key pressed
}

char keyboard_scan() {
	char key = keyboard_read();  // Read the pressed key
	if (key != 0) {
		// Display the pressed key on the LCD (assuming lcd_char is defined elsewhere)
		lcd_char(key);
	}
	return key;  // Return the pressed key
}
#endif /* KEYBOARD_H_ */