/*
 * COMPE_375_Lab_5_ Anishvar_Singaram.c
 *
 * Created: 10/14/2023 10:18:06 AM
 * Author : Anishvar Singaram
 */ 

// libraries used
#include <avr/io.h>

// child function definitions
void Port_Init();
void Run_Timer0(uint8_t bVal, uint8_t keyCol);

// main function
int main(void)
{
	
	// OCRB TON frequency values				 
	uint8_t TON[4][4] = {
							{35, 33, 31, 29},
							{27, 26, 24, 23},
							{21, 20, 19, 18},
							{17, 16, 15, 14},
						};

	Port_Init(); // initialize GPIO ports 
	
	while(1)
	{
		
		// loop to check if 4x4 keypad pressed
		for (uint8_t keyRow = 0; keyRow < 4; keyRow++)
		{	
			PORTD &= ~(1<<keyRow); // set a row to input sequentially	
			for (uint8_t keyCol = 0; keyCol < 4; keyCol++)
			{
				// if col is 0 (input) in input row, button is pressed
				if ( !(PINB & (1<<keyCol)) ) 
				{
					Run_Timer0(TON[keyRow][keyCol], keyCol); // run timer0 with 50% duty cycle
				}
			}
			PORTD |= (1 << keyRow); // reset current input row to output
		}
		
	}
	
}

// initialize GPIO ports
void Port_Init() 
{
	
	// 4x4 keypad I/O initializations
	DDRD |= ( (1<<DDD0) | (1<<DDD1) | (1<<DDD2) | (1<<DDD3) ); // set PD0-3 as output (row)
	DDRB &= ( ~(1<<DDB0) & ~(1<<DDB1) & ~(1<<DDB2) & ~(1<<DDB3) ); // set PB0-3 as input (col)
	PORTB |= ( (1<<PORTB0) | (1<<PORTB1) | (1<<PORTB2) | (1<<PORTB3) ); // enable pull-up option (1)
	PORTD |= ( (1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7) ); // make all rows logic high (1)
	
	// fast PWM I/O initializations
	DDRD |= (1<<DDD6); // set PD6 as Fast PWM output (OC0A)
	
}

// run timer0 as long as button is pressed
void Run_Timer0(uint8_t bVal, uint8_t keyCol) 
{
	
	// Timer0 initializations
	TCCR0A |= ( (1<<WGM01) | (1<<WGM00) ); // enable Fast PWM (TOP = OCRA) mode
	TCCR0B |= (1<<WGM02); // enable Fast PWM (TOP = OCRA) mode
	TCCR0A |= (1<<COM0A0); // turn OC0A pin on
	OCR0B = bVal; // TON
	OCR0A = 2*bVal; // TOP value (2*TON = 50% duty cycle)
	TCCR0B |= ( (1<<CS02) | (1<<CS00) ); // clk (PS = 1024)
	
	while ( !(PINB & (1<<keyCol)) ); // run timer0 while button pressed
	TCCR0A &= ~(1<<COM0A0); // turn OC0A pin off
	
}

