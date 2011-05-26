#include <avr/io.h>
#include <util/delay.h>

#define BRIGHTNESS_MAX 30

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
	int cur = 0;

	DDRD = (1 << PD5) | (1 << PD6) ;

	while (1) {

		cnt++;

		if (cnt > 100) {
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
