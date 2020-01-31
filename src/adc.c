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
	uint8_t channel_i;

	// Initialize conversion buffer
	for (channel_i = 0; channel_i < 8; channel_i++) {
		if (channel_mask & 0x01)
			conv_buf.buffer[conv_buf.buffer_size++].info = channel_i;
		channel_mask >>= 1;
	}

	// Initialize and enable ADC
	DIDR0 = channel_mask;
	ADMUX = _BV(REFS0) | conv_buf.buffer[conv_buf.write_pos].info;
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
#if defined ADC_START_TC0_COMP
	ADCSRB = _BV(ADTS1) | _BV(ADTS0);
	ADCSRA |= _BV(ADATE);
#elif defined ADC_START_TC0_OVF
	ADCSRB = _BV(ADTS2);
	ADCSRA |= _BV(ADATE);
#elif defined ADC_START_TC1_COMP
	ADCSRB = _BV(ADTS2) | _BV(ADTS0);
	ADCSRA |= _BV(ADATE);
#elif defined ADC_START_TC1_OVF
	ADCSRB = _BV(ADTS2) | _BV(ADTS1);
	ADCSRA |= _BV(ADATE);
#elif defined ADC_START_TC1_CAPT
	ADCSRB = _BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0);
	ADCSRA |= _BV(ADATE);
#else
	ADCSRB = 0x00;
#endif
}

uint8_t adc_poll(uint8_t *channel, uint16_t *data) {
	const uint8_t buf_rpos = conv_buf.read_pos;

	// Check for conversion update flag
	if (conv_buf.buffer[buf_rpos].info & 0x80) {

		// Copy from buffer
		do {
			conv_buf.buffer[buf_rpos].info &= 0x0F;
			*channel = conv_buf.buffer[buf_rpos].info;
			*data = conv_buf.buffer[buf_rpos].data;

		// Retry if new data has been buffered
		} while (conv_buf.buffer[buf_rpos].info & 0x80);

		// Increment buffer read pointer
		if (buf_rpos < conv_buf.buffer_size-1) {
			conv_buf.read_pos = buf_rpos + 1;
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
	const uint8_t buf_wpos = conv_buf.write_pos;

	// Update conversion buffer
	conv_buf.buffer[buf_wpos].info |= 0x80;
	conv_buf.buffer[buf_wpos].data = ADC;

	// Increment buffer write pointer
	if (buf_wpos < conv_buf.buffer_size-1) {
		conv_buf.write_pos = buf_wpos + 1;
	} else {
		conv_buf.write_pos = 0;
	}

	// Select next channel
	ADMUX = (ADMUX & 0xF0) | (conv_buf.buffer[conv_buf.write_pos].info & 0x0F);
}