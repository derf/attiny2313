#include <avr/io.h>
#include <stdlib.h>

#define LS_0000 0x00
#define LS_0001 0x01
#define LS_0010 0x04
#define LS_0011 0x05
#define LS_0100 0x10
#define LS_0101 0x11
#define LS_0110 0x14
#define LS_0111 0x15
#define LS_1000 0x40
#define LS_1001 0x41
#define LS_1010 0x44
#define LS_1011 0x45
#define LS_1100 0x50
#define LS_1101 0x51
#define LS_1110 0x54
#define LS_1111 0x55

#define ANIM_MAX 8
#define CNT_MAX 5000

int main (void)
{
	uint16_t cnt = 0;

	DDRB = (1 << PB0) | (1 << PB2) | (1 << PB4) | (1 << PB6);

	uint8_t animation[] = {
		LS_0001,
		LS_0011,
		LS_0110,
		LS_1100,
		LS_1000,
		LS_0000,
		LS_0000,
		LS_0000
	};

	while(1) {
		if (++cnt == CNT_MAX)
			cnt = 0;

		if ((cnt % (CNT_MAX / ANIM_MAX)) == 0)
			PORTB = animation[ cnt / (CNT_MAX / ANIM_MAX) ];

	}

	return 0;
}
