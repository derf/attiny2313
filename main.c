#include <avr/io.h>
#include <util/delay.h>

#define BRIGHTNESS_MAX 30
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

	int brightness = 1;
	int direction = 1;

	int cnt = 0;
	int cnt_max = 100;
	int cur = 0;

	DDRD = (1 << PD5) | (1 << PD6) ;

	while (1) {

		key1 = check_key_edge(key1, PD3);
		key2 = check_key_edge(key2, PD4);

		cnt++;

		if ((key1 == 1) && (cnt_max > CNTT_MIN))
			cnt_max -= CNTT_INT;
		else if ((key2 == 1) && (cnt_max < CNTT_MAX))
			cnt_max += CNTT_INT;

		if (cnt > cnt_max) {
			brightness += direction;

			if (brightness == BRIGHTNESS_MAX)
				direction = -1;
			else if (brightness == 0)
				direction = 1;

			cnt = 0;
		}

		cur = cnt % BRIGHTNESS_MAX;

		if (!cur)
			set_led_states(1, 0);

		if (cur == brightness)
			set_led_states(0, 1);

	}

	return 0;
}
