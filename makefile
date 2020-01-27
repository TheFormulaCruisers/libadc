#
# Parameters
#===========

CXX         = avr-gcc
CXXFLAGS    = -Wall
LIBS        = -Iinc
DEVICE		= at90can128
FCPU		= 16000000

#
# Make
#===========

O = adc_test.o adc.o
BIN = adc_test

adc_build: $(O)
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -o bin/$(BIN) $(O)

adc_test.o: tests/adc_test.c
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -c tests/adc_test.c $(LIBS)

adc.o: src/adc.c
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -c src/adc.c $(LIBS)

clean:
	rm *.o