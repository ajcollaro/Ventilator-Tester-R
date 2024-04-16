#include <avr/io.h>
#include "devices/sensor.hpp"
#include "peripherals/i2c.hpp"

namespace devices {

    class dac {
        private:
            const uint8_t OFFSET = 20;

        public:
            union {
                struct {
                    uint8_t byte_h;
                    uint8_t byte_l;
                };
                uint16_t bytes;
            };

            void set(float *data, peripherals::i2c *i2c)
            {
                this->bytes = (uint16_t)data + OFFSET;
                
                i2c->update(&this->bytes);
                // 
            }
    };

}