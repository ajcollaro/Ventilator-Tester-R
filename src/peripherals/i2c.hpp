#include <avr/io.h>

namespace peripherals {
    
    class i2c {
        private:
            union {
                struct {
                    uint8_t device;
                    uint8_t command;
                    uint8_t byte2;
                    uint8_t byte3;
                };
                uint8_t bytes[3];
            };

            static void tx_stop(void)
            {
                TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO);

                while(TWCR & (1 << TWSTO));
            }

            static void tx_start(void)
            {
                TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);

                while(!(TWCR & (1 << TWINT)));
            }
        
        public:
            void update(uint16_t *);

            void tx_data(void);
            void tx_stop(void);
            void tx_start(void);
            
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