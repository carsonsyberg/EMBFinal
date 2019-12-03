/*
 * main.c
 *
 * Created: 11/14/2019 12:29:39 PM
 *  Author: Carson
 */ 

#include <asf.h>

#define FOSC 1843200 // Clock Speed
#define BAUD 9600 // Baud Rate
#define MYUBRR FOSC/16/BAUD-1

void config_UART(void);
void config_GPIO(void);
void write_data(uint8_t message);
uint8_t read_data(void);

int final(void) {
	config_GPIO();
	config_UART();
	
	int numPeople = 8;
	
	while(1){
		
		switch(numPeople) {
			case 1: //display 1
				PORTC = 0b00000110;
				PORTB = 0b00000000;
				break;
		    case 2: //display 2
				PORTC = 0b01011011;
				PORTB = 0b00000000;
				break;
			case 3: //display 3
				PORTC = 0b01001111;
				PORTB = 0b00000000;
				break;
			case 4: //display 4
				PORTC = 0b01100110;
				PORTB = 0b00000000;
				break;
			case 5: //display 5
				PORTC = 0b01101101;
				PORTB = 0b00000000;
				break;
			case 6: //display 6
				PORTC = 0b01111101;
				PORTB = 0b00000000;
				break;
			case 7: //display 7
				PORTC = 0b00000111;
				PORTB = 0b00000000;
				break;
			case 8: //display 8
				PORTC = 0b01111111;
				PORTB = 0b00000000;
				break;
			case 9: //display 9
				PORTC = 0b01101111;
				PORTB = 0b00000000;
				break;
			case 10: //display 10
				PORTC = 0b00111111;
				PORTB = 0b00000110;
				break;
			case 11: //display 11
				PORTC = 0b00000110;
				PORTB = 0b00000110;
				break;
			case 12: //display 12
				PORTC = 0b01011011;
				PORTB = 0b00000110;
				break;
			case 13: //display 13
				PORTC = 0b01001111;
				PORTB = 0b00000110;
				break;
			case 14: //display 14
				PORTC = 0b01100110;
				PORTB = 0b00000110;
				break;
			case 15: //display 15
				PORTC = 0b01101101;
				PORTB = 0b00000110;
				break;
			case 16: //display 16
				PORTC = 0b01111101;
				PORTB = 0b00000110;
				break;
			case 17: //display 17
				PORTC = 0b00000111;
				PORTB = 0b00000110;
				break;
			case 18: //display 18
				PORTC = 0b01111111;
				PORTB = 0b00000110;
				break;
		}
	}
}

void config_GPIO(void) {
	// initialize PC.0:7 as outputs 13.2.1
	DDRC = 0b11111111; // all port C are outputs
	// initialize PB.0:7 as outputs
	DDRB = 0b11111111; // all port B are outputs
	
	// UART Pins
	DDRD |= 0b100;
	//	DDRx = output TX pin
	//	DDRx = input RX pin
}

void config_UART(void) {
	// bits 7:6 00 = normal asynchronous mode
	// bits 5:4 00 = parity disabled
	// bit 3 0 = 1 stop bit
	// C bits 2:1 11 and B bit 2 0 = 8-bit size of data
	// bit 0 0 data changed on rising XCKn edge
	UCSR0C = 0b00000110;
	UCSR0B &= ~UCSZ02;
	
	// setting the Baud Rate
	UBRR0H = (unsigned char)(MYUBRR>>8);
	UBRR0L = (unsigned char)MYUBRR;
	//U2X0 = 0; // configures clock for asynchronous normal
	
	// Enables Receiver and Transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
}

void write_data(uint8_t message) {
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1<<UDRE0)));
	// Put data into buffer, sends the data
	UDR0 = message;
}

uint8_t read_data(void) {
	// Wait for start bit
	while (!(UCSR0A & (1<<RXC0)));
	// Get data from buffer
	return UDR0;
}

