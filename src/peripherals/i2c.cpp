#include <avr/io.h>
#include "i2c.hpp"

namespace peripherals {

    void i2c::update(uint16_t *data)
    {
        (uint16_t)this->bytes[2];
    }

    void i2c::tx_data(void)
    {
        for(uint8_t i = 0; i <= sizeof(this->bytes); i++)
        {
            TWDR = this->bytes[i];
            TWCR = (1 << TWINT)|(1 << TWEN);

            while(!(TWCR & (1 << TWINT)));
        };
    }

    static void i2c::tx_stop(void)
    {
        TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO);

        while(TWCR & (1 << TWSTO));
    }

    static void i2c::tx_start(void)
    {
        TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);

        while(!(TWCR & (1 << TWINT)));
    }
}