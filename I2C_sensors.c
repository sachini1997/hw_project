/*
 * Demonstration for using I2C devices/sensors such as compass, eeprom, accelerometer etc.
 *
 * Target: Any AVR with hardware I2C (TWI). TWI : Two Wire Interface.
 *
 * Created: on 29 September 2013
 * Author: Vassilis Serasidis
 *
 * Home: http://www.serasidis.gr
 * e-mail: avrsite@yahoo.gr, info@serasidis.gr 
 *
 * Devices was used:
 * - GY26:     I2C digital compass.
 * - LIS302DL: 3-axes I2C/SPI accelerometer.
 * - PCF8574T: I2C-to-parallel converter. Usually used to drive dot matrix LCDs such as 16x2 or 20x4 characters.
 * - DS1307:   I2C Real-Time-Clock. 
 * - 24C32:    I2C 32kbit eeprom (4 kByte x 8 bit = 32 kBits). 
 *
 *
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "i2cmaster.h"
#include "uart.h"
#include "GY26.h"
#include "LIS302DL.h"
#include "DS1307.h"
#include "i2c_lcd.h"
#include "24cXX.h"

#define LED1 PB0


void placeEmptyCharacters (char charsNumber[8], unsigned char emptySpacesNumber);

//--------------------------------------------------------------------

//--------------------------------------------------------------------
int main(void)
{
	char data[16], time[21];
	unsigned int compass;
	int axe_value;
	unsigned char length, i;
	
	DDRB |= (1<<LED1);
	
	uart_init(9600);				// Initialize serial port to 9600 bits per second (9600 bps).
	gy26_init();					// Initialize GY-26 digital compass.
	lcd_init(LCD_BACKLIGHT_ON);		// Initialize I2C LCD.
	LIS302DL_init();				// initialize I2C LIS302DL XYZ accelerometer.
	//sei();						// Enable interrupts.
	
	//-------------------------------------------------------------------
	// Sets the DS1307 date/time(Real Time Clock chip).
	// To set the Time/date comment-out the commands bellow and change to values to these you want.
	// When you do that,re-compile the source code and program the AVR and keep pressed the reset button until the minutes in your clock 
	// of your house are the same with the minutes you programmed the DS1307.
	// Then, comment-in the commands bellow, compile the source code and re-program the AVR
	// with the new code.
	// Do that in case your DS1307 has not the correct date/time.
	// If your DS1307 is already programmed with the correct time then leave the commands bellow in comment.
	//
	// The commands below program the DS1307:
	// 1 (Monday) 30/09/2013  20:25:00
	//-------------------------------------------------------------------
	//DS1307Write(0x00,0x00); //seconds (0-59). Now is 0x00 = 0.
	//DS1307Write(0x01,0x25); //Minutes (0-59). Now is 0x25 = 25 on LCD.
	//DS1307Write(0x02,0x20); //Hours (0-23). Now is 0x20 = 20 on LCD.
	//DS1307Write(0x03,0x01); //Day (1-7). First day is Sunday. Now is 0x01 = 1 on LCD.
	//DS1307Write(0x04,0x30); //Date (1-31). Now is 0x30 = 30 on LCD.
	//DS1307Write(0x05,0x09); //Month (1-12). Now is 0x09 = 9 on LCD.
	//DS1307Write(0x06,0x13); //Year (00-99). Now is 0x13 = 13 on LCD.
	//DS1307Write(0x07,0b00000000); 
	
	lcd_goto_xy(0,2);
	lcd_puts_P(PSTR("X:"));
	lcd_goto_xy(6,2);
	lcd_puts_P(PSTR("Y:"));
	lcd_goto_xy(12,2);
	lcd_puts_P(PSTR("Z:"));
	
	for(i=0;i<5;i++)
		eep24cXX_write_byte(i,40 + i); //Writes 5 continuous bytes to I2C eeprom (24c32).
			
	lcd_goto_xy(0,3); //Go to 4th line on 20x4 LCD.
	for(i=0;i<5;i++)  //Read 4 continuous bytes from 24C32 eeprom starting from address 0x00.
	{
		itoa(eep24cXX_read_byte(i),data,10); //Read a byte from I2C eeprom and convert the value in to a string (ASCII value).
		lcd_puts(data);	// Print the ASCII byte value on LCD.
		lcd_putc(' ');  // Print a 'space' character on LCD.	
	}		
		
    while(1)
    {	
		//-------------------------------------------------------------------
		// Read DS1307 RTC and print date and time on LCD.			
		//-------------------------------------------------------------------	
		DS1307ReadASCII(time); //Read date and time from DS1307 I2C clock.			
		lcd_goto_xy(0,0); // x = horizontal, y = vertical (line).
		lcd_puts(time);
		
		//-------------------------------------------------------------------
		// Read compass and print degrees value on LCD.	
		//-------------------------------------------------------------------
		compass = gy26Read();
		itoa(compass, data, 10);
		length = strlen(data);			
		data[length + 1] = 0x00;
		data[length] = data[length - 1];
		data[length - 1] = '.';
		if(length == 1)
		{
			data[3] = 0;
			data[2] = data[1];
			data[1] = '.';
			data[0] = '0';
		}
		lcd_goto_xy(0,1);// x = horizontal, y = vertical (line).
		lcd_puts(data);
		placeEmptyCharacters(data,5);
		
		//-------------------------------------------------------------------
		// Read accelerometer and print X,Y,Z axis value on LCD.	
		//-------------------------------------------------------------------
		axe_value = LIS302DL_read(outX); //Read X axe value.
		itoa(axe_value, data, 10);		//Convert the X value integer into string.
		lcd_goto_xy(2,2);				//Go to character 2 and line 2 on LCD.
		lcd_puts(data);					//Print X value number on LCD.
		placeEmptyCharacters(data,4);	//Print some number of space characters
										//Example: the "-32" needs 3 characters. So, one more space character will be print on LCD (total 4 characters).  
			
		axe_value = LIS302DL_read(outY); //Read Y axe value.
		itoa(axe_value, data, 10);
		lcd_goto_xy(8,2);
		lcd_puts(data);
		placeEmptyCharacters(data,4);
			
		axe_value = LIS302DL_read(outZ); //Read Z axe value.
		itoa(axe_value , data, 10);
		lcd_goto_xy(14,2);
		lcd_puts(data);
		placeEmptyCharacters(data,4);
			
		_delay_ms(200);	
    }
}

//==================================================================================
// Prints space characters on LCD starting from the last used LCD address.
// This function is used to erase previous printed data on LCD from specific location.
//==================================================================================
void placeEmptyCharacters (char charsNumber[8], unsigned char emptySpacesNumber)
{
	unsigned char length, i;
	length = strlen(charsNumber);
	for(i=0;i<(emptySpacesNumber - length);i++)
		lcd_putc(' ');	
}	