#define F_CPU 8000000UL     // define cpu frquency
#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>
#include <stdio.h>

///////////////////////LCD display/////////////////
#include "display/i2c_lcd.h"
#include "common/i2cmaster.h"

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)     // convert to baudrate dec value

#define STRING_SIZE 16

void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);         // Turn on transmission and reception by setting RX Tx bits
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);    // Use 8-bit character sizes
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value
	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value
}

void UART_TxChar(char c)
{
	while (! (UCSRA & (1<<UDRE))); // Wait for empty transmit buffer
	UDR = c;
}

void UART_sendString(char *str)
{
	unsigned char s=0;
	
	while (str[s]!=0) // string till null
	{
		UART_TxChar(str[s]);    // send s to UART_TxChar(s) function
		s++;
	}
}
unsigned char UART_RxChar()									/* Data receiving function */
{
	while (!(UCSRA & (1 << RXC)));					/* Wait until new data receive */
	return UDR;									/* Get and return received data */
}


char* receiveStringFromNode(){
	static char str[STRING_SIZE];
	char num;
	while (UART_RxChar() != 255);
	for (char i = 0; i < STRING_SIZE; i++)
	{
		if(num = UART_RxChar())
			str[i] = num;
		if(num == '\0')
			break;
	}
	return str;
}

unsigned char getCharFromNode()	{
	return UART_RxChar();
}

void sendStringToNode(char * str){
	_delay_ms(50);
	USART_SendString(str);      // send string
	_delay_ms(1200);
}

int main(void)
{
	lcd_init(LCD_BACKLIGHT_ON);
	lcd_clear();
	lcd_goto_xy(0, 0);
	
	UART_init(9600); // initialise UART communication
	
	
	lcd_puts_at("Pass", 0, 0);
	
	_delay_ms(3000);
	
	lcd_clear();
	
	lcd_goto_xy(0,0);
	



	int x1 = 0;
	while(1)
	{
		char str[6];
		sprintf(str, "PND: %d", x1++);
		
		lcd_puts_at(str, 0, 0);	
		
		
		_delay_ms(50);			
		sendStringToNode(str);
		_delay_ms(1200);
		
		//int num = 0;
		//char snum[6];
		//
		//char x[10];
		//char ch;
		////
		////_delay_ms(50);
		////UART_sendString("Hello");
		////_delay_ms(1200);
		//
		//char* str = receiveStringFromNode();
		//lcd_puts_at(str, 0, 0);
		//
//
		//itoa(x1, snum, 10);
		//lcd_puts_at(snum, 0, 1);
		//x1++;
		//
		//if(num = USART_RxChar()){
			////_delay_ms(1000);
			//itoa(num, snum, 10);
			//lcd_puts_at(snum, 0, 0);
			//itoa(x1, snum, 10);
			//lcd_puts_at(snum, 0, 1);
			//x1++;
		//}
		
		//if(bit_is_clear(PINA,1)) //if push button is not pressed
		//{
			//PORTA |= (1<<0); // turn on led
			//_delay_ms(20);
		//}
		//else
		//{                          // if pressed then transmit "SEND" command over serial
			//PORTA &= ~(1<<0);    //turn off led
			//_delay_ms(50);
			//USART_SendString("SEND");      // send string
			//_delay_ms(1200);
		//}
	}
}


