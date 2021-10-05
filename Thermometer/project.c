#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "lcd.h"
#include "ds18b20.h"

//ENCODER SETUP

volatile unsigned char new_state, old_state;
volatile unsigned char changed = 0; // Flag for encoder state change
volatile int count = 60;			// Count to display
volatile unsigned char a, b;

unsigned char temp_change = 0;

int msg = 3;
uint16_t temp;

int blink = 0;
int on = 0;
int beep = 0;
int beeped = 0;

// Frequencies for notes from C3 in half-step increments up to C5
int frequency[25] =
	{131, 139, 147, 156, 165, 176, 185, 196, 208, 220, 233, 247,
	 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523};

//early interrupt setup
//clock will be set to ~15.56 kHZ
//...this equates to 15560 ticks per second
#define CL_FREQUENCY (15560)
//counter value keeps track of cycles--this gives us 1 s periods
#define counter_value (CL_FREQUENCY / 2)

//ISR for blinking LED
ISR(TIMER1_COMPA_vect)
{

	//every 0.5 seconds, blink red LED
	if (blink == 1)
	{

		if (on)
		{
			PORTD &= ~(1 << 2);
			on = 0;
		}
		else
		{
			PORTD |= (1 << 2);
			on = 1;
		}
	}
}

//**************

int main(void)
{

	//INTERRUPT SETUP*****

	OCR1A = counter_value;

	//TCCR1A and TCCR1B will be compared
	TCCR1A = 0x00;

	//clears the second timer on compare
	TCCR1B |= (1 << WGM12);

	//divides clock by 1024 (16mHZ -> 16 kHZ) (prevents timer overflow)
	TCCR1B |= (1 << CS10) | (1 << CS12);

	//enable interrupts
	TIMSK1 |= (1 << OCIE1A);
	sei();

	//********

	volatile char temp_threshold = eeprom_read_byte((void *)20); //access and test stored threshold and note values
	if ((temp_threshold < 60) || (temp_threshold > 100))
	{
		temp_threshold = 60;
	}

	char note = eeprom_read_byte((void *)10);
	if ((note < 0) || (note > 24))
	{
		note = 0;
	}

	//ENCODER SETUP
	//a and b represent the two bits changed by the rotary encoder
	//encoder has four positions -- ab: 00, 01, 10, 11
	//positions change differently based on rotation direction
	if ((PINB & (1 << 3)) == 0)
	{
		b = 1;
	}
	else
		b = 0;
	if ((PINB & (1 << 4)) == 0)
	{
		a = 1;
	}
	else
		a = 0;

	if (!b && !a)
		old_state = 0;
	else if (!b && a)
		old_state = 1;
	else if (b && !a)
		old_state = 2;
	else
		old_state = 3;

	new_state = old_state;

	//************

	/***PORT INFO***
	PB3-encoder
	PB4-encoder
	PC1-alert note set
	PC2-temp threshold set
	PC3-1-wire bus to DS18B20
	PC5-buzzer
	PD2-Red LED
	PD3-Green LED
	/****************/

	//port setup
	//enable buttons as inputs/enable pull-up resistors
	DDRC &= ~(1 << 1);
	DDRC &= ~(1 << 2);
	PORTC |= (1 << 1) | (1 << 2);

	//encoder inputs/pull-ups
	DDRB &= ~(1 << 3);
	DDRB &= ~(1 << 4);
	PORTB |= (1 << 3) | (1 << 4);

	//LEDs as outputs
	DDRD |= (1 << 3) | (1 << 2);

	//buzzer as output
	DDRC |= (1 << 5);

	// Initialize LCD
	lcd_init();
	// Write a spash screen to the LCD
	lcd_writecommand(0x01);
	char name[13];
	char proj[14];
	snprintf(name, 13, "Charlie Hill");
	snprintf(proj, 14, "AVR-THERMOMETER");

	lcd_moveto(0, 2);
	lcd_stringout(name);
	lcd_moveto(1, 1);
	lcd_stringout(proj);
	_delay_ms(2000);
	//clear LCD
	lcd_writecommand(0x01);

	unsigned char t[2];
	int encoder_target = 0; //0-initial 1-note 2-temp

	if (ds_init() == 0)
	{ // Initialize the DS18B20
		// If sensor not responding, send warning
		lcd_moveto(1, 0);
		char warn[2];
		snprintf(warn, 2, "%d", 1);
		lcd_stringout(warn);
	}

	ds_convert(); // Start first temperature conversion

	//print initial temp screen
	char top[6];
	snprintf(top, 6, "TEMP:");
	lcd_stringout(top);

	char bottom[14];
	snprintf(bottom, 14, "SET  T:    N:");
	lcd_moveto(1, 0);
	lcd_stringout(bottom);

	//show saved threshold value
	lcd_moveto(1, 7);
	char buf1[4];
	snprintf(buf1, 4, "%d", temp_threshold);
	lcd_stringout(buf1);
	lcd_moveto(1, 17);

	//show saved note value
	lcd_moveto(1, 13);
	snprintf(buf1, 4, "%d", frequency[note]);
	lcd_stringout(buf1);
	lcd_moveto(1, 17);

	//**MAIN LOOP**//

	while (1)
	{
		if (ds_temp(t))
		{ // True if temperature measurement complete

			//convert to Farenheit
			t[1] = (t[1] & 0x7);	   //isolate sig. digits of MSB
			temp = (t[1] << 8) | t[0]; //combine MSB and LSB into 16-bit int
			temp *= 18;				   // celsius * 10 * 9 / 5 converts to F and removes decimal
			int dec = temp % 100;	   //isolate decimal digits
			temp /= 100;			   //move sig. digits back to 10's and 1's place

			//print temp
			char buf[5];
			snprintf(buf, 5, "%d.%d", temp, dec);
			lcd_moveto(0, 5);
			lcd_stringout(buf);
			lcd_moveto(0, 17);

			ds_convert(); // Start next conversion
		}

		int pressed = 0;

		//if button pressed to set temp
		while ((PINC & 1 << 2) == 0)
		{
			pressed = 2;
		}
		//if button pressed to set note
		while ((PINC & 1 << 1) == 0)
		{
			pressed = 1;
		}

		if (pressed == 1)
		{
			char buf[5];
			snprintf(buf, 5, "Note");
			lcd_moveto(1, 0);
			lcd_stringout(buf);
			lcd_moveto(0, 17);

			if (encoder_target == 1)
				encoder_target = 0; //release target on second press
			else
				encoder_target = 1; //set target if first press
		}

		if (pressed == 2)
		{
			char buf[5];
			snprintf(buf, 5, "Temp");
			lcd_moveto(1, 0);
			lcd_stringout(buf);
			lcd_moveto(0, 17);

			if (encoder_target == 2)
			{ //release encoder target if pressed again
				encoder_target = 0;
			}
			else //if only first press or target is 1, set target 2
				encoder_target = 2;
		}

		//handle encoder movement with polling
		//see a and b explaination at "ENCODER SETUP" (line 106)
		if (encoder_target)
		{

			if ((PINB & (1 << 3)) == 0)
			{
				b = 1;
			}
			else
				b = 0;
			if ((PINB & (1 << 4)) == 0)
			{
				a = 1;
			}
			else
				a = 0;

			if (!b && !a)
				new_state = 0;
			else if (!b && a)
				new_state = 1;
			else if (b && !a)
				new_state = 2;
			else
				new_state = 3;

			if (old_state == 0)
			{

				// Handle A and B inputs for state 0
				if (new_state == 1)
				{
					count++;
					note++;
				}
				else if (new_state == 2)
				{
					count--;
					note--;
				}
			}

			else if (old_state == 1)
			{

				// Handle A and B inputs for state 1
				if (new_state == 0)
				{
					count--;
					note--;
				}
				else if (new_state == 3)
				{
					count++;
					note++;
				}
			}

			else if (old_state == 2)
			{

				// Handle A and B inputs for state 2
				if (new_state == 0)
				{
					count++;
					note++;
				}
				else if (new_state == 3)
				{
					count--;
					note--;
				}
			}

			else
			{ // old_state = 3

				// Handle A and B inputs for state 3
				if (new_state == 1)
				{
					count--;
					note--;
				}
				else if (new_state == 2)
				{
					count++;
					note++;
				}
			}

			if (new_state != old_state)
			{
				changed = 1;
				old_state = new_state;
			}

			if (encoder_target == 2)
			{ //keep temp threshold within 60 and 100
				if (count >= 100)
					count = 100;
				if (count <= 60)
					count = 60;
			}

			if (encoder_target == 1)
			{ //keep note within C3 and C5
				if (note < 0)
					note = 0;
				if (note > 24)
					note = 24;
			}

			if (changed && (encoder_target == 2))
			{ //output temp or note results
				changed = 0;
				lcd_moveto(1, 7);
				char buf[5];
				snprintf(buf, 5, "%d ", temp_threshold);
				lcd_stringout(buf);
				lcd_moveto(1, 7);

				temp_threshold = (char)count; //assign count to threshold value
											  //write to eeprom
				char set = temp_threshold;
				eeprom_update_byte((void *)20, set);
			}
			if (changed && (encoder_target == 1))
			{
				changed = 0;
				lcd_moveto(1, 13);
				char buf[6];
				snprintf(buf, 6, "%d  ", frequency[note]);
				lcd_stringout(buf);
				lcd_moveto(0, 17);

				//write note to eeprom
				eeprom_update_byte((void *)10, note);
			}
		}

		//COOL
		if (temp <= temp_threshold)
		{
			blink = 0;
			PORTD |= (1 << 3);
			PORTD &= ~(1 << 2);
			if (msg == 0)
			{
				temp_change = 0;
			}
			else
			{
				temp_change = 1;
				msg = 0;
			}
		}

		//WARM
		if ((temp > temp_threshold) && (temp <= temp_threshold + 2))
		{
			PORTD &= ~(1 << 3);
			blink = 1;

			if (msg == 2) //edge case for going from hot to warm
				PORTD &= ~(1 << 2);

			if (msg == 1)
			{
				temp_change = 0;
			}
			else
			{
				temp_change = 1;
				msg = 1;
			}
		}

		//HOT
		if (temp > temp_threshold + 2)
		{
			blink = 0;
			beep = 1;
			PORTD &= ~(1 << 3);
			PORTD |= (1 << 2);

			if (msg == 2)
			{
				temp_change = 0;
			}
			else
			{
				temp_change = 1;
				msg = 2;
			}
		}

		if (temp_change)
		{
			temp_change = 0;
			char buf[5];
			if (msg == 0)
				snprintf(buf, 5, "COOL");
			if (msg == 1)
				snprintf(buf, 5, "WARM");
			if (msg == 2)
				snprintf(buf, 5, "HOT!");

			lcd_moveto(0, 10);
			lcd_stringout(buf);
			lcd_moveto(0, 17);
		}
	}
}
