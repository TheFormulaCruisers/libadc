#ifndef _ADC_H
#define _ADC_H

/**
 * @brief Initialize ADC and conversion buffer.
 * @param channel_mask Bit mask indicating the ADC channels to use.
 * @return void
 *
 * A buffer is used to store the conversion results from each channel that has
 * been enabled by channel_mask, allowing retrieval of the results at any
 * suitable moment. The buffer always holds the latest conversion results.
 *
 * In order to start conversion on a timer/counter action, define one of the 
 * following macros:
 *	- ADC_START_TC0_COMP
 * 	- ADC_START_TC0_OVF
 * 	- ADC_START_TC1_COMP
 * 	- ADC_START_TC1_OVF
 * 	- ADC_START_TC1_CAPT
 */
void adc_init(uint8_t channel_mask);

/**
 * @brief Poll for new conversion result and retrieve if available.
 * @param channel Pointer to where the ADC channel number is copied.
 * @param data Pointer to where the conversion result is copied.
 * @return (1) new conversion available, (0) otherwise.
 */
uint8_t adc_poll(uint8_t *channel, uint16_t *data);

/**
 * @brief Start a new conversion.
 * @param void
 * @return void
 */
void adc_start(void);

#endif