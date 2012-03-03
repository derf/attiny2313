#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h>
#include <stdlib.h>

#define BRIGHTNESS_MAX 40

inline void set_led(char red, char green, char blue)
{
	PORTD = (red << PD4) | (blue << PD5) | (green << PD6);
}

int main (void)
{
	char red, green, blue;
	unsigned int b_red, b_green, b_blue;
	unsigned int n_red, n_green, n_blue;

	unsigned int step = 0;

	unsigned int cnt = 0;
	unsigned char cur = 0;

	unsigned char pwm[16] = {
		0, 1, 1, 1, 1, 2, 2, 3, 4, 6, 8, 11, 15, 22, 30, 40
	};

	unsigned int cnt_max = BRIGHTNESS_MAX;

	DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6);

	b_red = b_green = b_blue = n_red = n_green = n_blue = 20;

	while (1) {

		cnt++;

		if (cnt > cnt_max) {
			step++;
			cnt = 0;

			if (step == 2000) {
				step = 0;
				n_red = pwm[rand() % 16];
				n_green = pwm[rand() % 16];
				n_blue = pwm[rand() % 16];
			}
			if (!(step % 50)) {
				     if (n_red < b_red) b_red--;
				else if (n_red > b_red) b_red++;
				     if (n_green < b_green) b_green--;
				else if (n_green > b_green) b_green++;
				     if (n_blue < b_blue) b_blue--;
				else if (n_blue > b_blue) b_blue++;
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
