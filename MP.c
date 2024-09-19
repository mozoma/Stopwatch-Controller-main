/*
 * AHMED HISHAM FATHY HASSABOU
 * DIPLOMA 70
 * AIN SHAMS MECHATRONICS
 * MINI PROJECT 2
 */
/*
 * Ftimer = 1MHz/1024 = 977Hz
 * Ttimer = 1ms
 * Interrupt Number = 1000/1 = 1000
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define interrupt_no 1000
#define delay 2

unsigned char tick  = 0;
unsigned char sec0  = 0;
unsigned char sec1  = 0;
unsigned char min0  = 0;
unsigned char min1  = 0;
unsigned char hour0 = 0;
unsigned char hour1 = 0;


ISR(TIMER1_COMPA_vect)
{
	tick++;
	if(tick == 10)
	{
		PORTC &= 0xF0;
		sec0 = 0;
		tick = 0;
		sec1++;
		if(sec1==6)
		{
			sec1=0;
			min0++;
		}
		if(min0 == 10)
		{
			sec1=0;
			min0=0;
			min1++;
		}
		if(min1 == 6)
		{
			sec1=0;
			min0=0;
			min1=0;
			hour0++;
		}
		if(hour0 == 10)
		{
			sec1=0;
			min0=0;
			min1=0;
			hour0=0;
			hour1++;
		}
	}
	else
	{
		sec0++;
	}
}

ISR(INT0_vect)                         		// Reset all attributes
{
	sec0=0, sec1=0, min0=0, min1=0, hour0=0, hour1=0, tick =0;
}
ISR(INT1_vect)                          	// stop the clock
{
	TCCR1B &= ~(1 << CS10) & ~(1 << CS11) & ~(1 << CS12);
}
ISR(INT2_vect)                          	// Resume clock
{
	TCCR1B |= (1 << CS10) | (1 << CS12);
}

void Int0config()
{
	DDRD &= ~(1 << PD2);                	// defining as input pin
	PORTD |= (1 << PD2);                	// setting pull up
	MCUCR |= (1 << ISC01);              	// automatic flag
	GICR |= (1 << INT0);                	// enable flag bit
}

void Int1config()							// pull down set externally
{
	DDRD &= ~(1 << PD3);                	// defining as input pin
	MCUCR |= (1 << ISC11) | (1<<ISC10); 	// automatic flag
	GICR |= (1 << INT1);                	// enable flag bit
}

void Int2config()
{
	DDRB &= ~(1 << PB2);                	// defining as input pin
	PORTB |= (1 << PB2);                	// setting pull up
	MCUCSR &= ~(1 << ISC2);             	// automatic flag
	GICR |= (1 << INT2);                	// enable flag bit
}

void Timer1_CTC_Init(void)
{
	TCNT1 = 0;                          	// set Timer1 initial value to zero
	OCR1A = interrupt_no;               	// set compare value to 1000
	TIMSK |= (1<<OCIE1A);               	// Compare A interrupt enable
	TCCR1A = (1<<FOC1A);                	// Force output compare for channel A
	TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS12);// CTC mode and clock select
}

int main(void)
{
	DDRC |= 0x0F;
	PORTC = 0xF0;
	DDRA |= 0x3F;

	SREG|=(1<<7); 							//Enable global interrupt

	Timer1_CTC_Init(); 						//start timer1
	Int0config();
	Int1config();
	Int2config();
	while(1)
	{
		//digit 1 (from right)
		PORTC = (PORTC & 0xF0) | (sec0 & 0x0F);
		_delay_us(delay);
		PORTA |= (1<<PA0);
		_delay_us(delay);
		PORTA &= ~(1<<PA0);

		//digit 2
		PORTC = (PORTC & 0xF0) | (sec1 & 0x0F);
		_delay_us(delay);
		PORTA |= (1<<PA1);
		_delay_us(delay);
		PORTA &= ~(1<<PA1);

		//digit 3
		PORTC = (PORTC & 0xF0) | (min0 & 0x0F);
		_delay_us(delay);
		PORTA |= (1<<PA2);
		_delay_us(delay);
		PORTA &= ~(1<<PA2);

		//digit 4
		PORTC = (PORTC & 0xF0) | (min1 & 0x0F);
		_delay_us(delay);
		PORTA |= (1<<PA3);
		_delay_us(delay);
		PORTA &= ~(1<<PA3);

		//digit 5
		PORTC = (PORTC & 0xF0) | (hour0 & 0x0F);
		_delay_us(delay);
		PORTA |= (1<<PA4);
		_delay_us(delay);
		PORTA &= ~(1<<PA4);

		//digit 6
		PORTC = (PORTC & 0xF0) | (hour1 & 0x0F);
		_delay_us(delay);
		PORTA |= (1<<PA5);
		_delay_us(delay);
		PORTA &= ~(1<<PA5);
	}
}
