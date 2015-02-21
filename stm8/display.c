#include "display.h"
#include "stm8s.h"

uint8_t display_idx;
uint8_t display_data[4];

static const display_number[10] = {
	0xFC, // '0'
	0x60, // '1'
	0xDA, // '2'
	0xF2, // '3'
	0x66, // '4'
	0xB6, // '5'
	0xBE, // '6'
	0xE0, // '7'
	0xFE, // '8'
	0xF6, // '9'
};

void display_init(void)
{
	// We assume that the pins were already configured
}

#define SET_DATA(bit) do { if (bit) { PD_ODR |= (1<<4); } else { PD_ODR &= ~(1<<4); }} while (0)
#define PULSE_CLOCK() do { PA_ODR |= (1<<1); PA_ODR &= ~(1<<1); } while (0)
#define SAVE_DATA() do { PA_ODR &= ~(1<<2); PA_ODR |= (1<<2); } while (0)

void display_word(uint16_t word)
{
	uint8_t i;

	for (i = 0; i < 16; i++) {
		uint8_t bit = word & 1;
		word >>= 1;
		SET_DATA(bit);
		PULSE_CLOCK();
	}
	SAVE_DATA();
}

void display_refresh(void)
{
	int i = display_idx++;

	uint8_t bit = 8+(i*2);
	uint16_t digit = 0xFF00 ^ (3<<bit);
	display_word(digit | display_data[i]);

	if (display_idx == 4)
		display_idx = 0;
}

uint8_t display_char(uint8_t ch, uint8_t dot)
{
	if (dot)
		dot = 1;
	if (ch >= '0' && ch <= '9')
		return display_number[ch-'0'] | dot;
	return dot;
}

void display_show(uint8_t ch1, uint8_t dot1, uint8_t ch2, uint8_t dot2, uint8_t ch3, uint8_t dot3, uint8_t ch4, uint8_t dot4)
{
	display_data[3] = display_char(ch1, dot1);
	display_data[2] = display_char(ch2, dot2);
	display_data[1] = display_char(ch3, dot3);
	display_data[0] = display_char(ch4, dot4);
}
