#include <avr/io.h>
#include <stdlib.h>

#define MAX_BRIGHTNESS 50

int main (void)
{
	uint8_t cnt = 0;
	uint8_t cnt_max = 250;
	uint8_t led[6] = {50, 40, 30, 20,  0,  0};
	uint8_t inc[6] = {49, 49, 49, 49, 49, 47};

	uint8_t step = 0;

	DDRB = (1 << PB0) | (1 << PB2) | (1 << PB4) | (1 << PB6);
	DDRD = (1 << PD5) | (1 << PD6);

	while(1) {
		if (++cnt == cnt_max) {
			cnt = 0;

			for (int i = 0; i < 6; i++)
				led[i] = (led[i] + inc[i]) % MAX_BRIGHTNESS;
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
