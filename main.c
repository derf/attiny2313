#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>

#define BRIGHTNESS_MAX 40
#define CNTT_MAX (BRIGHTNESS_MAX * 10)
#define CNTT_MIN BRIGHTNESS_MAX
#define CNTT_INT BRIGHTNESS_MAX

volatile char f_led1[BRIGHTNESS_MAX];
volatile char f_led2[BRIGHTNESS_MAX];

volatile int cnt_max = BRIGHTNESS_MAX * 2;
volatile char offset_2 = 10;

inline void set_led_states(char led1, char led2)
{
	PORTD = (led1 << PD5) | (led2 << PD6);
}

void set_led_normal(char *led)
{
	for (int i = 0; i < BRIGHTNESS_MAX; i++)
		led[i] = abs(i - (BRIGHTNESS_MAX / 2));
}

void set_led_offset(char *led, char offset)
{
	for (int i = 0; i < BRIGHTNESS_MAX; i++)
		led[i] = abs(abs(i - offset) - (BRIGHTNESS_MAX / 2));
}

int main (void)
{
	char led1 = 0;
	char led2 = 0;

	int brightness = 1;

	int cnt = 0;
	char cur = 0;

	DDRD = (1 << PD5) | (1 << PD6);

	set_led_normal(f_led1);
	set_led_offset(f_led2, offset_2);

	MCUCR = (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00);
	GIMSK = (1 << INT0) | (1 << INT1);

	sei();

	while (1) {

		cnt++;

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

		if (cur == f_led1[brightness])
			led1 = 0;

		if (cur == f_led2[brightness])
			led2 = 0;

		set_led_states(led1, led2);

	}

	return 0;
}

ISR(INT0_vect)
{
	offset_2 = (offset_2 + 10) % BRIGHTNESS_MAX;
	set_led_offset(f_led2, offset_2);
}

ISR(INT1_vect)
{
	if (cnt_max > CNTT_MIN)
		cnt_max -= CNTT_INT;
	else
		cnt_max = CNTT_MAX;
}
