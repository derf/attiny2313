#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h>
#include <stdlib.h>

#define BRIGHTNESS_MAX 80

inline void set_led(char red, char green, char blue)
{
	PORTD = (red << PD4) | (blue << PD5) | (green << PD6);
}

int main (void)
{
	char red, green, blue;
	char b_red, b_green, b_blue;
	char n_red, n_green, n_blue;

	unsigned int step = 0;

	unsigned int cnt = 0;
	unsigned char cur = 0;

	unsigned char pwm[16] = {
		0, 1, 1, 1, 1, 3, 4, 5, 8, 11, 15, 21, 30, 43, 60, 80
	};

	unsigned int cnt_max = BRIGHTNESS_MAX;

	DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6);

	while (1) {

		cnt++;

		if (cnt > cnt_max) {
			step++;
			cnt = 0;

			if (step == 300) {
				step = 0;
				n_red = pwm[rand() % 16];
				n_green = pwm[rand() % 16];
				n_blue = pwm[rand() % 16];
			}
			if (!(step % 12)) {
				b_red = ((unsigned int)b_red * 19 + n_red) / 20;
				b_green = ((unsigned int)b_green * 19 + n_green) / 20;
				b_blue = ((unsigned int)b_blue * 19 + n_blue) / 20;
			}
		}

		cur = cnt % BRIGHTNESS_MAX;

		if (!cur) {
			red = green = blue = 1;
		}

		if (cur == b_red)
			red = 0;
		if (cur == b_green)
			green = 0;
		if (cur == b_blue)
			blue = 0;

		set_led(red, green, blue);

	}

	return 0;
}
