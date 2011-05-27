#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h>
#include <stdlib.h>

#define BRIGHTNESS_MAX 40
#define CNTT_MAX (BRIGHTNESS_MAX * 10)
#define CNTT_MIN BRIGHTNESS_MAX
#define CNTT_INT BRIGHTNESS_MAX

volatile unsigned char f_led1[BRIGHTNESS_MAX];
volatile unsigned char f_led2[BRIGHTNESS_MAX];

unsigned int ee_cnt_max EEMEM;
volatile unsigned int cnt_max;

unsigned char ee_opmode EEMEM;
unsigned char opmode;
enum {
	MODE_FADE_INV, MODE_FADE_OUT,  MODE_FADE_SAME, MODE_FADE_IN,
	MODE_BLINK_INV, MODE_BLINK_OUT, MODE_BLINK_SAME, MODE_BLINK_IN,
	MODE_UFADE, MODE_BFADE, MODE_DFADE,
	MODE_END
};

void set_opmode(void);
void set_led_blink(char *led, char offset);
void set_led_fade(char *led, char offset);
void set_led_ufade(char *led, char offset);
void set_led_dfaed(char *led, char offset);

void set_opmode(void)
{
	switch (opmode) {
		case MODE_END:
			opmode = MODE_FADE_INV;
			/* fall-through */
		case MODE_FADE_INV:
			set_led_fade(f_led1, 0);
			set_led_fade(f_led2, BRIGHTNESS_MAX / 2);
			break;
		case MODE_FADE_OUT:
			set_led_fade(f_led1, 0);
			set_led_fade(f_led2, (BRIGHTNESS_MAX / 2)
					+ (BRIGHTNESS_MAX / 4));
			break;
		case MODE_FADE_SAME:
			set_led_fade(f_led1, 0);
			set_led_fade(f_led2, 0);
			break;
		case MODE_FADE_IN:
			set_led_fade(f_led1, 0);
			set_led_fade(f_led2, BRIGHTNESS_MAX / 4);
			break;
		case MODE_BLINK_INV:
			set_led_blink(f_led1, 0);
			set_led_blink(f_led2, BRIGHTNESS_MAX / 2);
			break;
		case MODE_BLINK_OUT:
			set_led_blink(f_led1, 0);
			set_led_blink(f_led2, (BRIGHTNESS_MAX / 2)
					+ (BRIGHTNESS_MAX / 4));
			break;
		case MODE_BLINK_SAME:
			set_led_blink(f_led1, 0);
			set_led_blink(f_led2, 0);
			break;
		case MODE_BLINK_IN:
			set_led_blink(f_led1, 0);
			set_led_blink(f_led2, BRIGHTNESS_MAX / 4);
			break;
		case MODE_UFADE:
			set_led_ufade(f_led1, 0);
			set_led_ufade(f_led2, 0);
			break;
		case MODE_BFADE:
			set_led_ufade(f_led1, 0);
			set_led_dfade(f_led2, 0);
			break;
		case MODE_DFADE:
			set_led_dfade(f_led1, 0);
			set_led_dfade(f_led2, 0);
			break;
	}
}

inline void set_led_states(char led1, char led2)
{
	PORTD = (led1 << PD5) | (led2 << PD6);
}

void set_led_blink(char *led, char offset)
{
	for (unsigned char i = 0; i < BRIGHTNESS_MAX; i++) {
		if (((i + offset) % BRIGHTNESS_MAX) < (BRIGHTNESS_MAX / 2))
			led[i] = BRIGHTNESS_MAX;
		else
			led[i] = 0;
	}
}

void set_led_fade(char *led, char offset)
{
	for (unsigned char i = 0; i < BRIGHTNESS_MAX; i++)
		led[i] = abs(abs(i - offset) - (BRIGHTNESS_MAX / 2));
}

void set_led_ufade(char *led, char offset)
{
	for (unsigned char i = 0; i < BRIGHTNESS_MAX; i++)
		led[(i + offset) % BRIGHTNESS_MAX] = i * i / BRIGHTNESS_MAX;
}

void set_led_dfade(char *led, char offset)
{
	for (unsigned char i = 0; i < BRIGHTNESS_MAX; i++)
		led[BRIGHTNESS_MAX - i - 1] = i * i / BRIGHTNESS_MAX;
}

int main (void)
{
	char led1 = 0;
	char led2 = 0;

	unsigned char brightness = 1;

	unsigned int cnt = 0;
	unsigned char cur = 0;

	DDRD = (1 << PD5) | (1 << PD6);

	opmode = eeprom_read_byte(&ee_opmode);
	if (opmode == 0xFF)
		opmode = MODE_FADE_INV;

	cnt_max = eeprom_read_word(&ee_cnt_max);
	if (!cnt_max || (cnt_max > CNTT_MAX))
		cnt_max = BRIGHTNESS_MAX * 2;

	set_opmode();

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
	opmode++;

	set_opmode();

	eeprom_write_byte(&ee_opmode, opmode);
}

ISR(INT1_vect)
{
	if (cnt_max > CNTT_MIN)
		cnt_max -= CNTT_INT;
	else
		cnt_max = CNTT_MAX;

	eeprom_write_word(&ee_cnt_max, cnt_max);
}
