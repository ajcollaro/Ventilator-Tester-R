#pragma once

#include <avr/io.h>

namespace devices {

    class sensor {
        private:
            const float SLOPE = 36.463;
            const float INTERCEPT = -18.036;

        public:
            float flow;

            void sample(void)
            {
                this->flow = SLOPE * (((float)ADC / 1023) * 5) + INTERCEPT;
            }
    };

}