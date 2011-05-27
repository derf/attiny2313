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

volatile unsigned int cnt_max = BRIGHTNESS_MAX * 2;

unsigned char ee_opmode EEMEM;
unsigned char opmode;
enum {
	MODE_FADE_INV, MODE_FADE_OUT,  MODE_FADE_SAME, MODE_FADE_IN,
	MODE_BLINK_INV, MODE_BLINK_OUT, MODE_BLINK_SAME, MODE_BLINK_IN,
	MODE_UFADE,
	MODE_END
};

void set_opmode(void);
void set_led_blink(char *led, char offset);
void set_led_fade(char *led, char offset);
void set_led_ufade(char *led, char offset);

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
	/* 
	 * Should be:
	 * led[i] = BRIGHTNESS_MAX * ( ( pow(b, (i / BRIGHTNESS_MAX)) - 1) / (b - 1) );
	 */

	/* hardcoded and no offset support due to RAM limitations */

	led[0] = 0;
	led[1] = 0;
	led[2] = 1;
	led[3] = 1;
	led[4] = 1;
	led[5] = 2;
	led[6] = 2;
	led[7] = 3;
	led[8] = 3;
	led[9] = 4;
	led[10] = 5;
	led[11] = 6;
	led[12] = 7;
	led[13] = 8;
	led[14] = 9;
	led[15] = 10;
	led[16] = 12;
	led[17] = 14;
	led[18] = 15;
	led[19] = 18;
	led[20] = 20;
	led[21] = 18;
	led[22] = 15;
	led[23] = 14;
	led[24] = 12;
	led[25] = 10;
	led[26] = 9;
	led[27] = 8;
	led[28] = 7;
	led[29] = 6;
	led[30] = 5;
	led[31] = 4;
	led[32] = 3;
	led[33] = 3;
	led[34] = 2;
	led[35] = 2;
	led[36] = 1;
	led[37] = 1;
	led[38] = 1;
	led[39] = 0;
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
}
