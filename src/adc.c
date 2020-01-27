#include <avr/io.h>
#include <avr/interrupt.h>
#include <adc.h>

// ---------------------------------------------------------------------- Types

typedef struct {
	/* Info Register
	 * Bit 7: Conversion updated.
	 * Bit 6-4: Unused.
	 * Bit 3-0: Channel number.
	 */
	uint8_t info;
	uint16_t data;
} conv_t;

typedef struct {
	uint8_t write_pos;
	uint8_t read_pos;
	uint8_t buffer_size;
	conv_t buffer[8];
} conv_buffer_t;

// --------------------------------------------------------------------- Memory

static volatile conv_buffer_t conv_buf = {0};

// --------------------------------------------------------- External Functions

void adc_init(uint8_t channel_mask) {
	uint8_t channel_i, channel_mask_i;

	// Config conversion buffer
	channel_mask_i = 0x01;
	for (channel_i = 0; channel_i < 8; channel_i++) {
		if (channel_mask_i & channel_mask)
			conv_buf.buffer[conv_buf.buffer_size++].info = channel_i;
		channel_mask <<= 1;
	}

	// Config and start ADC
	DIDR0 = channel_mask;
	ADMUX = conv_buf.buffer[conv_buf.write_pos].info & 0x0F;
	ADCSRB = 0x00;
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

uint8_t adc_poll(uint8_t *channel, uint16_t *data) {

	// Check for new conversion flag
	if (conv_buf.buffer[conv_buf.read_pos].info & 0x80) {

		// Copy from buffer
		do {
			conv_buf.buffer[conv_buf.read_pos].info &= 0x0F;
			*channel = conv_buf.buffer[conv_buf.read_pos].info;
			*data = conv_buf.buffer[conv_buf.read_pos].data;

		// Retry if new data has been buffered
		} while (conv_buf.buffer[conv_buf.read_pos].info & 0x80);

		// Increment buffer read pointer
		if (conv_buf.read_pos < conv_buf.buffer_size-1) {
			conv_buf.read_pos++;
		} else {
			conv_buf.read_pos = 0;
		}

		return 1;
	}
	return 0;
}

void adc_start(void) {

	ADCSRA |= _BV(ADSC);
}

ISR(ADC_vect) {

	// Update conversion buffer
	conv_buf.buffer[conv_buf.write_pos].info |= 0x80;
	conv_buf.buffer[conv_buf.write_pos].data = ADC;

	// Increment buffer write pointer
	if (conv_buf.write_pos < conv_buf.buffer_size-1) {
		conv_buf.write_pos++;
	} else {
		conv_buf.write_pos = 0;
	}
	
	// Select next channel
	ADMUX = (ADMUX & 0xF0) | (conv_buf.buffer[conv_buf.write_pos].info & 0x0F);
}