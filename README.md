# libadc

This library interfaces with the analog-to-digital converter built into a range of Microchip's 8-bit AVR microcontrollers. It supports multi-channel signal conversion and timer/counter and manual triggering. A buffer to store newly acquired conversions is implemented, eliminating the need for immediate intervention by the user once a conversion has been finished. Instead, conversions may be retrieved from the buffer whenever convenient.

## Operation

Built into the hardware of the majority of Microchip's AVR microcontrollers is an analog-to-digital converter. They support sampling one channel (an ADC input pin) at a time. Depending on its resolution and accuracy, a conversion might take a few to a few hundred microseconds. The ADC hardware may generate an interrupt when a conversion has finished.

Libadc implements a buffer between the ADC conversion register and its interface. It is used to store the most recent measurement for each configured channel. When a conversion finishes, the ADC Interrupt Service Routine copies the result into its respective buffer location and selects the next configured channel to be sampled - all configured channels are sampled subsequently. Conversion can be started on a timer/counter compare, overflow or capture, or manually. If used, the corresponding timer/counter flag is cleared by libadc during the ADC ISR. The conversion buffer acts as a circular buffer except in that unread values are overwritten with new conversion results. There's space to store one result per configured channel. Results are received from the buffer subsequently.

## Usage

Functions are defined in the header files located in _inc_. Each function is supplemented with a documentational block comment that describes its usage. A block comment may contain the following tags:

  * @brief
  * @param
  * @return
  * @note
  * @bug

## Macros

One of the following macros may be specified when compiling the source:

  * ADC_START_TC0_COMP
  * ADC_START_TC0_OVF
  * ADC_START_TC1_COMP
  * ADC_START_TC1_OVF
  * ADC_START_TC1_CAPT

## Build

The _tools_ folder contains an ATMEL Studio project that may be used to build the test program located in _tests_. The project is setup to link to the original files in _inc_ and _src_, thus avoiding unnecessary copies. Its build-in compiler includes _inc_ as well (_Properties > Toolchain > Directories_). Macros are defined at _Properties > Toolchain > Symbols_.

A makefile is provided, but is not setup to generate a programmable image. By using _make_, one may easily check whether the program compiles correctly and without warnings using a terminal or command-prompt.

## Devices

Device | Status
--- | ---
AT90CAN128 | Supported
ATMEGA32M1 | Untested
