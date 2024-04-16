#include "peripherals/i2c.hpp"
#include "peripherals/mcu.hpp"
#include "devices/sensor.hpp"

int main(void)
{
    /* Setup MCU peripherals*/
    peripherals::adc adc;
    peripherals::i2c i2c;

    /* Setup sensor. */
    devices::sensor sensor;
}