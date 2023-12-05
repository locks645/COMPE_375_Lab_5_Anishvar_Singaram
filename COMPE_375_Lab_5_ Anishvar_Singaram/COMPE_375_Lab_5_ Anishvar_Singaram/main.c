/*
 * COMPE_375_Lab_5_ Anishvar_Singaram.c
 *
 * Created: 10/14/2023 10:18:06 AM
 * Author : Anishvar Singaram
 */ 

// libraries used
#include <avr/io.h>

// child function definitions
void GPIO_init();
void timer0_init(uint8_t b_val, uint8_t key_col);

// main function
int main(void)
{
	
	// OCR0B values for corresponding 4x4 keypad buttons				 
	uint8_t t_on[4][4] = {
							{35, 33, 31, 29},
							{27, 26, 24, 23},
							{21, 20, 19, 18},
							{17, 16, 15, 14},
						 };

	GPIO_init(); // initialize GPIO pins 
	
	// infinite while-loop to prevent program termination  
	while(1)
	{
		// nested for-loops to continuously check if a 4x4 keypad button is pressed
		for (uint8_t key_row = 0; key_row < 4; key_row++)
		{	
			PORTD &= ~(1<<key_row); // set current row to input	
			for (uint8_t key_col = 0; key_col < 4; key_col++)
			{
				// check if current col = 0 (input)
				if ( (PINB & (1<<key_col)) == 0 ) 
				{
					timer0_init(t_on[key_row][key_col], key_col); // initialize timer0
				}
			}
			PORTD |= (1<<key_row); // reset current row to output
		}	
	}
	
}

// GPIO pin initialization function
void GPIO_init() 
{
	// 4x4 keypad I/O initializations
	DDRD |= ( (1<<DDD0) | (1<<DDD1) | (1<<DDD2) | (1<<DDD3) ); // set D0-D3 pins (row) as output
	PORTD |= ( (1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7) ); // initialize rows to 1
	DDRB &= ( ~(1<<DDB0) & ~(1<<DDB1) & ~(1<<DDB2) & ~(1<<DDB3) ); // set B0-B3 pins (col) as input
	PORTB |= ( (1<<PORTB0) | (1<<PORTB1) | (1<<PORTB2) | (1<<PORTB3) ); // enable pull-up resistor(s)
	
	// Fast PWM I/O initializations
	DDRD |= (1<<DDD6); // set PD6 as Fast PWM output (OC0A)	
}

// timer0 initialization function
void timer0_init(uint8_t b_val, uint8_t key_col) 
{	
	// timer0 initializations
	TCCR0A |= ( (1<<WGM01) | (1<<WGM00) ); // enable Fast PWM (TOP = OCRA) mode
	TCCR0B |= (1<<WGM02); // enable Fast PWM (TOP = OCRA) mode
	OCR0B = b_val; // t_on value
	OCR0A = 2*b_val; // TOP value (2*t_on = 50% duty cycle)
	TCCR0B |= ( (1<<CS02) | (1<<CS00) ); // CLK with PS = 1024
	
	// Fast PWM output pin logic
	TCCR0A |= (1<<COM0A0); // turn OC0A pin on
	while ( (PINB & (1<<key_col)) == 0 ); // run timer0 while button is pressed
	TCCR0A &= ~(1<<COM0A0); // turn OC0A pin off	
}

