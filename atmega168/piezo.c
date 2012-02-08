#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h>
#include <stdlib.h>

#define CNTT_MAX 100
#define CNTT_MIN 2
#define CNTT_INT 2

volatile unsigned int cnt_max = 80;

int main (void)
{
	unsigned int cnt = 0;
	unsigned char cur = 0;

	DDRD = (1 << PD5) | (1 << PD6) | (1 << PD7);

	if (!cnt_max || (cnt_max > CNTT_MAX))
		cnt_max = 80;

	MCUCR = (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00);
	EIMSK = (1 << INT0) | (1 << INT1);
	EICRA = (1 << ISC01) | (1 << ISC11);

	sei();

	while (1) {

		cnt++;

		cur = cnt % cnt_max;

		if (!cur)
			PORTD = 0;

		if (cur == cnt_max/2)
			PORTD = (1 << PD6) | (1 << PD7);

	}

	return 0;
}


ISR(INT0_vect)
{
	if (cnt_max < CNTT_MAX)
		cnt_max += CNTT_INT;
	else
		cnt_max = CNTT_MIN;
}

ISR(INT1_vect)
{
	if (cnt_max > CNTT_MIN)
		cnt_max -= CNTT_INT;
	else
		cnt_max = CNTT_MAX;
}
