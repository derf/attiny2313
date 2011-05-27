#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#define BRIGHTNESS_MAX 60
#define CNTT_MAX 300
#define CNTT_MIN 30
#define CNTT_INT 25

inline void set_led_states(char led1, char led2)
{
	PORTD = (led1 << PD5) | (led2 << PD6);
}

int check_key_edge(int key, int port)
{
	if (PIND & (1 << port)) {
		if (!key)
			return 1;
		else
			return 2;
	} else
		return 0;
}

int main (void)
{
	char key1 = 0;
	char key2 = 0;
	char key3 = 0;
	char led1 = 0;
	char led2 = 0;

	int brightness = 1;

	int cnt = 0;
	int cnt_max = 100;
	int cur = 0;

	DDRD = (1 << PD5) | (1 << PD6) ;

	while (1) {

		key1 = check_key_edge(key1, PD2);
		key2 = check_key_edge(key2, PD3);
		key3 = check_key_edge(key3, PD4);

		cnt++;

		if ((key1 == 1) && (cnt_max > CNTT_MIN))
			cnt_max -= CNTT_INT;
		else if ((key2 == 1) && (cnt_max < CNTT_MAX))
			cnt_max += CNTT_INT;

		if (cnt > cnt_max) {
			brightness++;
			cnt = 0;

			if (brightness == BRIGHTNESS_MAX)
				brightness = 0;
		}

		cur = cnt % BRIGHTNESS_MAX;

		if (!cur) {
			led1 = 1;
			led2 = 1;
		}

		if (cur == abs(brightness - 30))
			led1 = 0;

		if (cur == abs(abs(brightness - 15) - 30))
			led2 = 0;

		set_led_states(led1, led2);

	}

	return 0;
}
