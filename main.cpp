#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define switchOn            (PINA & (1<<0)) // checking if switch is on 
#define lightOn             (PORTB |= (1<<6)) // light on	
#define lightOff            (PORTB &= ~(1<<6)) // light off

void setup(void);
ISR(INT5_vect); // set up for using interrupt motion sensor 

#define off                 0
#define on                  1
#define motion              2

char state = 0; // initializing the state 

int main(void)
{
	while(1)
	{
		setup();
		
		switch(state)
		{
			case off:
				lightOff;
			
				if(switchOn) // if light switch is on 
					state = on;
			
			break;
			
			case on:
				lightOn;
				
				if(!switchOn) // if light switch is off 
					state = off;
			
			break;
			
			case motion:
				lightOn;
				_delay_ms(10000); // 10 second hold delay on 
				
				state = off;
				
			break;
			
		}
		
	} 
	
}

ISR(INT5_vect)
{
	state = motion; // motion sensor sets the state to motion 
}

void setup(void)
{
	DDRA = 0x00; // switches 
	DDRE = 0x03; // sets PORTE to output
	PORTE = 0x00; // sets multiplexer
	DDRB = 0b01000000; // set bit 6 for light
	EIMSK = 0b00100000; // external interrupt 5
	EICRB = 0b00001100; // external interrupt 5 on rising edge
	sei(); // global enable
}

