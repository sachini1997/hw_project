/////////////////////////////////////////
// v1.1
// Serial Port uart Buffered, Interrupt driven I/O
// Tested with ATmega8. Register names for non-Mega chips differ.
// See uart.H for the API (function calls)
//
// Adapted from the "simpleuart" project uploaded to avrfreaks.net
//    adaptation: stevech@san.rr.com 4/06

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "uart.h"

#define TMASK		(TBUFSIZE-1)
#define RMASK		(RBUFSIZE-1)

volatile unsigned char tbuf[TBUFSIZE];	// buffers
volatile unsigned char rbuf[RBUFSIZE];

volatile unsigned char t_in;		//buffer head/tail indicies
volatile unsigned char t_out;		// NOTE! these are 1 byte; no atomic for double byte needed

volatile unsigned char r_in;
volatile unsigned char r_out;

///////////////////////////////////////////////////   
/* Receive interrupt */   
ISR(USART_RXC_vect) {  
	unsigned char c;
	
	c = UDR0;					// Get received char
	rbuf[r_in & RMASK] = c;		// put in buffer (no check for overflow)
	r_in++;		// RMASK makes it unnecessary to range limit this
}
///////////////////////////////////////////////////
/* Transmit complete interrupt */ 
ISR(USART_UDRE_vect) {  /* USART transmit Data Register Empty */
	
	if(t_in != t_out) {		// is buffer empty?
		UDR0 = tbuf[t_out & TMASK];
		t_out++;	 // TMASK makes it unnecessary to range limit this
	}
	else {
		UCSR0B &= ~(1<<UDRIE0);  // buffer is empty, disable interrupt
	}
}

//============================================================================
//void lcd::clear()
//{
	//_delay_ms(100);
//}

//============================================================================

///////////////////////////////////////////////////////////////
// Get string that ends with a \r  but subject to max chars
int uart_gets(char *p)
{
	int n = 0;
	do
	{ 
		*p = uart_getc();
		n++;
	}while ((*p++ != '\r') && (n < RBUFSIZE)); //If received character is the ENTER character, break this loop.
	*p = 0;
	if(r_in == r_out) //If receive buffer is read, clear it.
		r_in = r_out = 0;
	return(n);
}
///////////////////////////////////////////////////////////////
// Print string from RAM
void uart_puts(char *p)  {
	
	while (*p)
		uart_putc(*p++);
}
///////////////////////////////////////////////////////////////
// Print string from flash memory
void uart_puts_P( char *p)  {
	char b;
	
	while ( (b = pgm_read_byte(p++) ) != 0)
		uart_putc(b);
}

///////////////////////////////////////////////////
// Number of bytes as yet untransmitted
//    don't call this from an ISR
unsigned char uart_tbuflen(void) {

	return(t_in - t_out);
}

///////////////////////////////////////////////////
// unbuffered I/O send byte
void uart_putc(unsigned char c)  {
	
	/* if buffer is full wait, interrupts will empty it */
	while((TBUFSIZE - uart_tbuflen()) <= 2)
		;
	/* Add data to the transmit buffer, enable transmit interrupts */
	tbuf[t_in & TMASK] = (unsigned char)c;
	t_in++;							// increment head index
	UCSR0B |= (1<<UDRIE0);		// Enable UDR empty interrupt
								// the ISR will move from buffer to uart
	if(t_in == t_out) //If transmission buffer is read, clear it.
		t_in = t_out = 0;
}	

///////////////////////////////////////////////////
// buffered I/O send byte
//int uart_fputc(char c, FILE* stream) {
//	
//	uart_putc( (unsigned char)c );
//	return(0);
//}

///////////////////////////////////////////////////
// Number of bytes in receive buffer
// call this to avoid blocking in uart_getc()
unsigned char uart_rbuflen(void) {

	return (unsigned char)(r_in - r_out);
}

///////////////////////////////////////////////////
// Number of un-read bytes in receive buffer
unsigned char uart_unReadBufLen(void) {

	return (r_in);
}

////////////////////////////////////////////////////
//  I/O get char
unsigned char uart_getc(void) {
	unsigned char c;

	while(uart_rbuflen() == 0);  // wait for data. see also uart_rbuflen()
	
	c = rbuf[r_out & RMASK];
	r_out++;
	
	return(c);
}

//////////////////////////////////////////////////////////////
unsigned char uart_clearRxDBuffer(void)
{
	r_in = r_out = 0;
}
//////////////////////////////////////////////////////////////
// buffered I/O get char
//int uart_fgetc(FILE* stream) {
//	
//	return(int)(uart_getc());
//}


//////////////////////////////////////////////////////////////
// Initialization of 1st uart
void uart_init(unsigned int baudrate)
{
	#define BAUD_PRESCALE (((F_CPU/(baudrate*16UL)))-1)
	/* First init for the uart */
	unsigned int calcBaudrate;
	DDRD |= (1<<TxD);
	DDRD &= ~(1<<RxD);
	PORTB |= (1<<TxD) |(1<<RxD);
	
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	r_in = r_out = 0;
	t_in = t_out = 0;
	
}

//========================================================
// software uart transmition routine 9600bps
// Delay time is (1/9600) = 104.16 us.
// Created by Vassilis Serasidis on 28 Jul 2011
//========================================================
//void swuart_puts (unsigned char transmit[SW_TBUFSIZE])
//{
//
				//
		//unsigned char k,i;
		//for(k=0;k<(strlen(transmit));k++)
		//{
			//PORTB &= ~(1<<PB0);
			//_delay_us(104);
//
			//for(i=0;i<8;i++)
			//{				
				//if(bit_is_set(transmit[k],i))
					//PORTB |= (1<<PB0);
				//else
					//PORTB &= ~(1<<PB0);
				//_delay_us(104);
			//}
			//PORTB |= (1<<PB0);
			//_delay_us(300);
		//}
//
		//uart_clearRxDBuffer(); //After a software-transmition call don't forget to clean the Receiver buffer.
//}
