/*
 * uart messenger.c
 *
 * Created: 9/9/2024 7:52:44 PM
 * Author : john
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "lcd.h"
#include "keyboard.h"
#include "uart.h"

#define MAX_MESSAGE_LENGTH 16
#define FRAME_OVERHEAD 4  // 1 for line number, 2 for direction, 1 for null terminator

volatile char received_message[MAX_MESSAGE_LENGTH + FRAME_OVERHEAD];
volatile uint8_t rx_index = 0;
volatile uint8_t message_received = 0;

ISR(USART_RXC_vect) {
    char received = UDR;
    if (rx_index < MAX_MESSAGE_LENGTH + FRAME_OVERHEAD - 1) {
        received_message[rx_index] = received;
        rx_index++;
        if (received == '\n') {
            received_message[rx_index] = '\0';
            message_received = 1;
            rx_index = 0;
        }
    }
}

void send_framed_message(uint8_t lcd_line, const char* message) {
    char frame[MAX_MESSAGE_LENGTH + FRAME_OVERHEAD];
    snprintf(frame, sizeof(frame), "%d-%s\n", lcd_line, message);//1-hello\n
    uart_tx_str(frame);
}

void display_message(const char* message, uint8_t is_received, uint8_t line) {
    lcd_cmd(line == 0 ? 0x80 : 0xC0);  // Set cursor to beginning of appropriate line
    lcd_char(is_received ? '=' : '-');
    lcd_str(message);
}

int main(void) {
    lcd_init();
    keyboard_init();
    uart_init(9600);
    
    // Enable UART receive interrupt
    UCSRB |= (1 << RXCIE);
    sei();  // Enable global interrupts

    lcd_str("UART Messenger");
	lcd_cmd(0xC0);
	lcd_str("    john fms");
    _delay_ms(500);
    lcd_cmd(0x01);

    char buff[17];
    uint8_t pos = 0;
    uint8_t lcd_line = 0;
    
    while (1) {
        if (message_received) {
            display_message(received_message + 2, 1, lcd_line);  // +2 to skip line number and '='
            lcd_line = !lcd_line;  // Switch to other line
            if (lcd_line == 0) {
	            _delay_ms(2000);  // Wait before clearing screen
	            lcd_cmd(0x01);  // Clear screen
	            }else if (lcd_line == 1) {
	            lcd_cmd(0xC0);  // Clear screen
            }
            message_received = 0;
        }

        char ch = keyboard_read();
        
        if (ch != 0) {
            if (ch == '*') {
                buff[pos] = '\0';
                send_framed_message(lcd_line, buff);
                display_message(buff, 0, lcd_line);

                lcd_line = !lcd_line;  // Switch to other line
                if (lcd_line == 0) {
                    _delay_ms(2000);  // Wait before clearing screen
                    lcd_cmd(0x01);  // Clear screen
                }else if (lcd_line == 1) {
                lcd_cmd(0xC0);  // Clear screen
                }
                pos = 0;
                memset(buff, ' ', sizeof(buff) - 1);//clean buffer
                buff[16] = '\0';
            } else {
                if (pos < 16) {
                    buff[pos] = ch;
                    pos++;
                    lcd_char(ch);
                }
                if (pos == 16) {
                    pos = 0;
                    lcd_line = !lcd_line;
                    if (lcd_line == 0) {
                        lcd_cmd(0x01);  // Clear screen
                    }
                    lcd_cmd(lcd_line == 0 ? 0x80 : 0xC0);  // Move to the beginning of the new line
                }
            }
        }
    }
}