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
 *-----------------------Pulse Sensor---------------------------
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "i2cmaster.h"
#include "string.h"
#include "i2c_lcd.h"

char disp[16]="0000000000000001";
char result[8] = "00000001";






void ADC_Init(){
	DDRA=0x0;			/* Make ADC port as input */
	ADCSRA = 0x87;			/* Enable ADC, fr/128  */
	ADMUX = 0x40;			/* Vref: Avcc, ADC channel: 0 */
}



int ADC_Read(char channel){
	int Ain,AinLow;
	
	ADMUX=ADMUX|(channel & 0x0f);	/* Set input channel to read */

	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	_delay_us(10);
	AinLow = (int)ADCL;		/* Read lower byte*/
	Ain = (int)ADCH*256;		/* Read higher 2 bits and 
					Multiply with weight */
	Ain = Ain + AinLow;				
	return(Ain);			/* Return digital value*/
}


/*************** threshold ***************/
double m=0,c=0; // gradient and slope

double sumit(int data_points[],int length){
	
	double sum=0;
	for(int i=1;i<=length;i++)sum+=data_points[i];

	return sum;
}

double xysum(int data_points[],int length){
	
	double sum=0;
	for(int i=1;i<=length;i++)sum+=(i)*data_points[i];

	return sum;
}

void regression(int data_points[],double n){
	double squarex= (n)*(n+1)*(2*n+1)/6.0;
	double xbar = (n+1)/2.0;
	double ybar = sumit(data_points,n)/n;

	m=(xysum(data_points,n)- n*xbar*ybar)/( squarex - n*xbar*xbar );
	c=ybar-m*xbar;
}
/*************** end threshold ***************/




void main(void)
{	while(1){
	DDRD = 0xFF;  // #
	DDRC = 0xFF;  //for lcd
	DDRA = 0x00;  //Analog input
	
	ADC_Init();
	_delay_ms(1000);
	lcd_init(LCD_BACKLIGHT_ON);


	lcd_goto_xy(0,1);
	lcd_puts("Starting Pulse ~)");

	lcd_goto_xy(0,2);
	lcd_puts("Sensor *_*");
	

	_delay_ms(1000);
	lcd_clear();
	lcd_goto_xy(0,1);
	lcd_puts("Place your");
	
	lcd_goto_xy(0,2);
	lcd_puts("fingertip");
	_delay_ms(2000);
	
	lcd_clear();
	lcd_goto_xy(0,1);
	lcd_puts("Keep placing");
	lcd_goto_xy(0,2);
	lcd_puts("your fingertip");
	
	
	_delay_ms(3000);
	int i;
	int thresh=300;
	int count=0;
	int count2=0; //modified peak counting algo
	
	
	/*  timing data */
	double sampling_rate = 0.1000 ;	   	// actually this is the _delay_ms val
	int time_limit = 10 ;  				 //in seconds
	int se=time_limit/(sampling_rate*2);
	int data_points[se+1];

	data_points[0]=0;
	/* /timing data */
	
	/* for debugging purposes -> h and l records the peaks */
	int h=0;
	int l=1023;
	
	char val[6]; //temporary variable for itoa

	
	for(i=0;i<time_limit/(sampling_rate*2);i++){
		data_points[i+1]=ADC_Read(0);
		
		
		itoa(data_points[i+1],val,10);
		//lcd_disp(val,1,0,"");
		lcd_clear();
		lcd_goto_xy(0,1);
		lcd_puts("Measuring>>>");
		
		int k=16*(data_points[i+1]-200)/375;
		
		
		int h=0;
		for(h=0;h<k;h++){
		}
		itoa(k,val,10);

		_delay_ms(sampling_rate*1000);
		
	}
	
	regression(data_points,se);
	
	for(i=0;i<time_limit/(sampling_rate*2);i++){
		thresh=(i+2)*m+c;

		int a=data_points[i+1];//current value
		char temp[11]="";
		char ccount[3];

		if(a>h)h=a; //max peak
		if(a<l)l=a; //min peak
	
		if(a>thresh){
			count+=1; //peak counting]
			if(a>data_points[i] && a>data_points[i+2]){
				count2+=1;
			}
		
		}	
	}
	
	//displaying the heart rate

	lcd_clear();
	lcd_goto_xy(0,1);
	lcd_puts("Heart Rate:- ");
	
	
	itoa(count2*(60/time_limit),val,10);

	strcat(val," bpm");
	
	lcd_goto_xy(4,2);
	lcd_puts(val);
	_delay_ms(10000);
	lcd_clear();
	
	





}

	
}
		
		
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

    
	
	



