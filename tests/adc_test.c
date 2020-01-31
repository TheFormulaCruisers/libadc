#include <avr/io.h>
#include <avr/interrupt.h>
#include <adc.h>

int main(void) {
	uint32_t timeri;
	uint16_t adc_data;
	uint8_t adc_channel;

	DDRC = 0xFF;
	PORTC = 0x00;
	TCCR0A = _BV(CS02) | _BV(CS00);
	TIMSK0 = _BV(TOIE0);
	adc_init(0x01);
	sei();

	while (1) {
		while (adc_poll(&adc_channel, &adc_data)) {
			switch (adc_channel) {
				case 0:
					PORTC = (uint8_t)(adc_data >> 2);
					break;
				case 1:
					PORTC = 0x00;
					break;
				case 2:
					PORTC = 0x00;
					break;
				case 3:
					PORTC = 0x00;
					break;
				case 4:
					PORTC = 0x00;
					break;
				case 5:
					PORTC = 0x00;
					break;
				case 6:
					PORTC = 0x00;
					break;
				case 7:
					PORTC = 0x00;
					break;
			}
		}
	}
	return 0;
}