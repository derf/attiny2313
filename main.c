#include <avr/io.h>

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

	DDRD = (1 << PD5) | (1 << PD6) ;
	PORTD = (1 << PD5) | (1 << PD6);

	while (1) {
		key1 = check_key_edge(key1, PD2);
		key2 = check_key_edge(key2, PD3);
		key3 = check_key_edge(key3, PD4);

		if (key1 == 1)
			led1 = !led1;

		if (key2 == 1)
			led2 = !led2;

		set_led_state(led1, led2);
	}

	return 0;
}
