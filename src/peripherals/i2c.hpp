#pragma once

#include <avr/io.h>

namespace peripherals {
    
    class i2c {
        private:
            union {
                struct {
                    uint8_t device;
                    uint8_t command;
                    uint8_t data_big;
                    uint8_t data_little;
                };
                uint8_t data[4];
            };

            void tx_start(void)
            {
                TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);

                while(!(TWCR & (1 << TWINT)));
            }

            void tx_stop(void)
            {
                TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO);

                while(TWCR & (1 << TWSTO));
            }
        
        public:
            /* Data to send. */
            void update_data(uint16_t data)
            {
                this->data_big = data;
                this->data_little = data << 8;
            }

            void tx_data(void)
            {
                this->tx_start();

                for(uint8_t i = 0; i < sizeof(this->data); i++)
                {
                    TWDR = this->data[i];
                    TWCR = (1 << TWINT)|(1 << TWEN);

                    while(!(TWCR & (1 << TWINT)));
                };

                this->tx_stop();
            }
            
            i2c(void)
            {
                /* I2C using two-wire interface. */
                TWBR = 0x0C;

                /* Setup data to be transmitted. */
                this->device = 0x63 << 1;
                this->command = 0x40;
            }
    };

}