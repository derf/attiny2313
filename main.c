#include <avr/io.h>
#include <util/delay.h>

#define BRIGHTNESS_MAX 12

inline void set_led_state(char led1, char led2)
{
	PORTD = (led1 << PD5) | (led2 << PD6);
}

int check_key_edge(int key, int port)
{
	int ret;

	if (PIND & (1 << port)) {
		if (!key)
			ret = 1;
		else
			ret = 2;
	} else
		ret = 0;

	return ret;
}

int main (void) {

	char led1 = 0;
	char led2 = 0;
	char key1 = 0;
	char key2 = 0;
	char key3 = 0;

	int brightness = 0;

	DDRD = (1 << PD5) | (1 << PD6) ;
	PORTD = (1 << PD5) | (1 << PD6);

	while (1) {
		key1 = check_key_edge(key1, PD2);
		key2 = check_key_edge(key2, PD3);
		key3 = check_key_edge(key3, PD4);

		if ((key1 == 1) && (brightness > 0))
			brightness--;
		else if ((key2 == 1) && (brightness < BRIGHTNESS_MAX))
			brightness++;

		if (brightness != BRIGHTNESS_MAX) {
			_delay_us(brightness * 100);
			set_led_state(0, 1);
		}
		if (brightness != 0) {
			_delay_us((BRIGHTNESS_MAX - brightness) * 100);
			set_led_state(1, 0);
		}
	}

	return 0;
}
