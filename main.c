/*
 * keypad44.c
 *
 * Created: 7/31/2019 12:31:57 PM
 * Author : Navidu
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include "keypad.h"


int main(void)
{
    /* Replace with your application code */
    keypad_init();
    Keypad key;
	DDRD = 0xFF;
    char data[5];
   // DDRD = 0xFF;
    PORTD = 0x00;
    while(1)
    {
	  DDRD = 0xFF;
	  PORTD = 0x00;
      key = waitForKeyPress();
      switch(key)
      {
      case BTN_0:
	   PORTD = 0x00;
	  _delay_ms(1000);
	  PORTD = ~(1<<PIND0);
	  _delay_ms(1000);
	  PORTD = (1<<PIND0);
      break;
	   case BTN_1:
	   _delay_ms(1000);
	   PORTD = ~(1<<PIND1);
	   _delay_ms(1000);
	   PORTD = (1<<PIND1);
	   break;
	    case BTN_2:
	    _delay_ms(1000);
	    PORTD = ~(1<<PIND2);
	    _delay_ms(1000);
	    PORTD = (1<<PIND2);
	    break;
		 case BTN_3:
		 _delay_ms(1000);
		 PORTD = ~(1<<PIND3);
		 _delay_ms(1000);
		 PORTD = (1<<PIND3);
		 break;
		  case BTN_4:
		  _delay_ms(1000);
		  PORTD = ~(1<<PIND4);
		  _delay_ms(1000);
		  PORTD = (1<<PIND4);
		  break;
		   case BTN_5:
		   _delay_ms(1000);
		   PORTD = ~(1<<PIND5);
		   _delay_ms(1000);
		   PORTD = (1<<PIND5);
		   break;
		    case BTN_6:
		    _delay_ms(1000);
		    PORTD = ~(1<<PIND6);
		    _delay_ms(1000);
		    PORTD = (1<<PIND6);
		    break;
			 case BTN_7:
			 _delay_ms(1000);
			 PORTD = ~(1<<PIND7);
			 _delay_ms(1000);
			 PORTD = (1<<PIND7);
			 break;
      }
    }
    
}

