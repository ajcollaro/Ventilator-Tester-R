#pragma once

#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000
#endif

namespace peripherals {

    class adc {
        public:
            adc(void)
            {
                /* Set pin modes and init ADC. Reference voltage is AVCC (5V), 
                 * ADC operating in 10-bit mode with pre-scalar at 128 (125KHz).
                 * Interrupts enabled.
                 */
                ADMUX |= (1 << REFS0);
                ADCSRA |= (1 << ADPS0)|(1 << ADPS1)|(1 << ADPS2)|(1 << ADEN)|(1 << ADIE);
            }
    }

}