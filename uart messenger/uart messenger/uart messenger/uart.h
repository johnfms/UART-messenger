/*
 * uart.h
 *
 * Created: 9/9/2024 7:43:16 PM
 *  Author: john
 */ 


#ifndef UART_H_
#define UART_H_
#define F_CPU 8000000UL
void uart_init( unsigned int baud ){
	unsigned int ubrr = F_CPU / 16 / baud - 1; // Correct baud rate formula
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	// 8 data bits, 1 stop bit
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}
void uart_tx( unsigned char data ){
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}

unsigned char uart_rx( void ){
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) );
	/* Get and return received data from buffer */
	return UDR;
}
void uart_tx_str(const char* str) {
	for (int i=0;str[i] !='\0';i++){
		uart_tx(str[i]);
		_delay_ms(100);
	}
}
void uart_rx_str(char* buffer, int max_length) {
	int i = 0;
	while (i < max_length - 1) {
		char c = uart_rx();
		if (c == '\n' || c == '\r') {
			break;
		}
		buffer[i++] = c;
	}
	buffer[i] = '\0';
}
#endif /* UART_H_ */