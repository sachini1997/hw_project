// v1.0
// 4/06 stevech@san.rr.com

//#define UBBRL_VALUE 17	// 11.059200 MHz CPU, 38400 baud serial. See AVRcalc.exe
//#define UBBRL_VALUE 71	// 11.059200 MHz CPU,  9600 baud serial. See AVRcalc.exe
//#define UBBRL_VALUE 3	// 4 MHz CPU,  4800 baud serial. See AVRcalc.exe
//#define UBBRH_VALUE 1	// 4 MHz CPU,  4800 baud serial. See AVRcalc.exe
//#define UBBRL_VALUE 103	// 16 MHz CPU,  9600 baud serial. See AVRcalc.exe
//#define UBBRH_VALUE 0	// 16 MHz CPU,  9600 baud serial. See AVRcalc.exe

/* Buffersizes must be a power of 2 in size, for speed */
#define TBUFSIZE	128 //128 bytes buffer for serial transmission. 
#define RBUFSIZE	128 //128 bytes buffer for serial receiption. 
//#define SW_TBUFSIZE	30

#define RxD PD0 // Serial port Receipt pin.
#define TxD PD1 // Serial port Transmition pin.

#define UDR0 UDR		// ATmega8
#define UCSR0B UCSRB	// ATmega8
#define UDRIE0 UDRIE	// ATmega8
#define RXEN0 RXEN		// ATmega8
#define TXEN0 TXEN		// ATmega8
#define UCSR0C UCSRC	// ATmega8
#define UCSZ00 UCSZ0	// ATmega8
#define UCSZ01 UCSZ1	// ATmega8
#define RXCIE0 RXCIE	// ATmega8
#define UBRR0H UBRRH	// ATmega8
#define UBRR0L UBRRL	// ATmega8




void uart_init(unsigned int baudrate);			// initialize serial port.

void 			uart_putc(unsigned char c);  	// put ASCII or non-ASCII byte, blocks if buffer full
unsigned char 	uart_getc(void);				// get ASCII or non-ASCII byte, 
												//      blocks if none available. See uart_rbuflen()

int 			uart_gets(char *p);	// Get string that ends with a \r .
void 			uart_puts(char *p);				// print string from RAM
void 			uart_puts_P(char *p);			// print string from FLASH MEMORY

unsigned char 	uart_tbuflen(void);				// get number of as yet untransmitted bytes
unsigned char 	uart_rbuflen(void);				// get number of bytes in the receive buffer or zero

// buffered I/O if fdevopen() is to be used. 
// Matches AVR library as of 4/06
// put the next two lines in main()
//    	fdevopen( uart_fputchar, uart_fgetchar );			// establish buffered I/O callbacks
//  	sei();												// Global interrupt enable
//int 			uart_fgetc(FILE *);
//int 			uart_fputc(char, FILE *);
unsigned char   uart_clearRxDBuffer(void);
unsigned char   uart_unReadBufLen(void);

//void swuart_puts (unsigned char transmit[SW_TBUFSIZE]);

//class LiquidCrystal:public Print {
//public:
	//void clear();
//
//private:
	//void send(uint8_t, uint8_t);
//
//};
