#include <avr/io.h>
#include <avr/interrupt.h>
#include "display.hpp"
#include "peripherals/i2c.hpp"
#include "peripherals/misc.hpp"
#include "devices/lcd.hpp"
#include "devices/sensor.hpp"
#include <string.h>

ISR(ADC_vect)
{
    // Interrupt, return to main loop.
}

/* Calibration runner. */
template<class T> void calibrate(T send, peripherals::i2c *i2c)
{
    i2c->update_data(send);
    i2c->tx_data();
    _delay_ms(10000);
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
    devices::lcd lcd;
    devices::sensor sensor;

    /* Notify user of cal test signal. */
    constexpr char CAL_NOTIFY[] = { "Cal test signal" };
    char buffer[16];
    char *ptr = &buffer[0];

    memcpy(buffer, CAL_NOTIFY, sizeof(CAL_NOTIFY));
    lcd.write(ptr);

    /* Send calibration test signals. */
    calibrate(0xFFFF, &i2c);
    calibrate(0x0000, &i2c);

    /* Offset to avoid EPAP underflow on DAC. */
    const uint16_t OFFSET = 20;

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
        i2c.update_data(sensor.flow + OFFSET);
        i2c.tx_data();

        /* Update LCD before overflow. */
        if (cycle == 255) 
            display::report_data(&lcd, &sensor);

        /* Sleep and begin conversion. */
        asm("sleep \n\t");
    }
}