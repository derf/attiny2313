#include <avr/io.h>

int main (void) {

	DDRD = (1 << 5);
	PORTD = (1 << PD5);

	while (1) {
		PORTD = 0x00;
		PORTD = (1 << PD5);
	}

	return 0;
}
