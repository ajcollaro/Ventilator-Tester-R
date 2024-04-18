#pragma once

#include <avr/io.h>
#include "devices/lcd.hpp"
#include "devices/sensor.hpp"
#include <stdlib.h>
#include <string.h>

namespace display {

    void report_data(devices::lcd *lcd, devices::sensor *sensor)
    {
        constexpr char UNITS[] = { " L/min (STP)" };

        /* One line of text. */
        char buffer[16];
        char *ptr = &buffer[0];

        lcd->clear();

        /* Convert flow to string and copy to buffer.*/
        itoa((uint16_t)sensor->flow, buffer, 10);
        lcd->write(ptr);

        /* Copy units to buffer. */
        memcpy(buffer, UNITS, sizeof(UNITS));
        lcd->write(ptr);
    }

}