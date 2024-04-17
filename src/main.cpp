#include <avr/io.h>
#include <avr/interrupt.h>
#include "display.hpp"
#include "peripherals/i2c.hpp"
#include "peripherals/misc.hpp"
#include "devices/dac.hpp"
#include "devices/lcd.hpp"
#include "devices/sensor.hpp"
#include <string.h>

ISR(ADC_vect)
{
    // Interrupt, return to main loop.
}

int main(void)
{
    /* Thermal management. */
    DDRA = 0xFF;
    PORTA |= (1 << PORTA7);

    /* Setup peripherals. */
    peripherals::adc adc;
    peripherals::i2c i2c;

    /* Setup devices.. */
    devices::dac dac;
    devices::lcd lcd;
    devices::sensor sensor;

    /* Send calibration test signal. */
    i2c.update_data(0xFFFF);
    i2c.tx_data();
    _delay_ms(10000);

    i2c.update_data(0x0000);
    i2c.tx_data();
    _delay_ms(10000);

    /* Noise reduction mode. */
    SMCR |= (1 << SE);
    sei();

    /* Count cycles. */
    uint8_t cycle = 0;

    while(1)
    {
        cycle++;

        /* Get latest measurement. */
        sensor.sample();
        
        /* Update DAC. */
        dac.set_voltage(&sensor.flow);

        /* Update LCD before overflow. */
        if (cycle == 255) 
        {
            display::report_data(&lcd, &sensor);
            i2c.update_data(dac.bytes);
            i2c.tx_data();
        }

        /* Sleep and begin conversion. */
        asm("sleep \n\t");
    }
}