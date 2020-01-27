#include <avr/io.h>
#include <avr/interrupt.h>
#include <adc.h>

int main(void) {
	uint16_t adc_data;
	uint8_t adc_channel;
	
	adc_init(0xFF);

	while (1) {

		if (adc_poll(&adc_channel, &adc_data)) {
			// Do something with channel/data.
		}
		
		adc_start();
	}
	return 0;
}