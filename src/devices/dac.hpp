#pragma once

#include <avr/io.h>

namespace devices {

    class dac {
        private:
            const uint8_t OFFSET = 20;

        public:
            union {
                struct {
                    uint8_t byte_big;
                    uint8_t byte_little;
                };
                uint16_t bytes;
            };

            void set_voltage(float *data)
            {
                this->bytes = (uint16_t)data + OFFSET;
            }
    };

}