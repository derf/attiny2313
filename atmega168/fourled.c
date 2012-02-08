#include <avr/io.h>
#include <math.h>
#include <stdlib.h>

#define MAX_BRIGHTNESS 10

int main (void)
{
	uint16_t cnt = 0;
	uint16_t cnt_max = 200;
	uint8_t led[6] = { 0,  0,  0,  0,  0,  0};

	uint8_t step = 0;
	uint8_t x = 0;
	uint8_t x_max = 19;

	uint8_t f_led[4][x_max];

	uint8_t i;

	DDRB = (1 << PB0) | (1 << PB2) | (1 << PB4) | (1 << PB6);
	DDRD = (1 << PD5) | (1 << PD6);

	for (i = 0; i < x_max; i++) {
		f_led[0][i] = floor( 5 * ( cos( (float) (i +  0) / 3 ) + 1 ) );
		f_led[1][i] = floor( 5 * ( cos( (float) (i +  3) / 3 ) + 1 ) );
		f_led[2][i] = floor( 5 * ( cos( (float) (i +  6) / 3 ) + 1 ) );
		f_led[3][i] = floor( 5 * ( cos( (float) (i +  9) / 3 ) + 1 ) );
	}

	while (1) {
		if (++cnt == cnt_max) {
			cnt = 0;

			if (++x == x_max)
				x = 0;

			for (i = 0; i < 4; i++)
				led[i] = f_led[i][x];
		}

		step = cnt % MAX_BRIGHTNESS;

		if (step == 0) {
			PORTB = (1 << PB0) | (1 << PB2) | (1 << PB4) | (1 << PB6);
			PORTD = (1 << PD5) | (1 << PD6);
		}

		if (step == led[0])
			PORTB &= ~(1 << PB0);

		if (step == led[1])
			PORTB &= ~(1 << PB2);

		if (step == led[2])
			PORTB &= ~(1 << PB4);

		if (step == led[3])
			PORTB &= ~(1 << PB6);

		if (step == led[4])
			PORTD &= ~(1 << PD5);

		if (step == led[5])
			PORTD &= ~(1 << PD6);

	}

	return 0;
}
