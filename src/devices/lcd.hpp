#pragma once

#include <avr/io.h>
#include <util/delay.h>

namespace devices {

    class lcd {
        private:
            /* Mapping physical connections to LCD. */
            #define LCD_CONTROL_PORT    PORTL
            #define LCD_RS_PIN          PORTL2
            #define LCD_EN_PIN          PORTL4
            #define LCD_DATA_PORT1      PORTD
            #define LCD_DATA_PORT2      PORTC
            #define LCD_DATA_PIN1       PORTD7
            #define LCD_DATA_PIN2       PORTC1
            #define LCD_DATA_PIN3       PORTC3
            #define LCD_DATA_PIN4       PORTC5

            void tx_bits(uint8_t byte)
            {
                /* Test most significant 4-bits and set each pin. */
                (byte & 0x10) ? (LCD_DATA_PORT1 |=  (1 << LCD_DATA_PIN1)) : (LCD_DATA_PORT1 &= ~(1 << LCD_DATA_PIN1));
                (byte & 0x20) ? (LCD_DATA_PORT2 |=  (1 << LCD_DATA_PIN2)) : (LCD_DATA_PORT2 &= ~(1 << LCD_DATA_PIN2));
                (byte & 0x40) ? (LCD_DATA_PORT2 |=  (1 << LCD_DATA_PIN3)) : (LCD_DATA_PORT2 &= ~(1 << LCD_DATA_PIN3));
                (byte & 0x80) ? (LCD_DATA_PORT2 |=  (1 << LCD_DATA_PIN4)) : (LCD_DATA_PORT2 &= ~(1 << LCD_DATA_PIN4));
            }

            void tx_data(char byte)
            {
                /* Send data to LCD. */
                this->tx_bits(byte);
                
                LCD_CONTROL_PORT |=  (1 << LCD_RS_PIN)|(1 << LCD_EN_PIN);
                _delay_us(1);
                LCD_CONTROL_PORT &= ~(1 << LCD_EN_PIN);
                _delay_us(2);

                byte <<= 4;

                this->tx_bits(byte);

                LCD_CONTROL_PORT |=  (1 << LCD_EN_PIN);
                _delay_us(1);
                LCD_CONTROL_PORT &= ~(1 << LCD_EN_PIN);
                _delay_us(50);
            }

            void tx_cmd(uint8_t byte)
            {
                /* Send command to LCD. */
                this->tx_bits(byte);

                LCD_CONTROL_PORT &= ~(1 << LCD_RS_PIN);
                LCD_CONTROL_PORT |=  (1 << LCD_EN_PIN);  
                _delay_us(1);
                LCD_CONTROL_PORT &= ~(1 << LCD_EN_PIN);
                _delay_ms(1);

                byte <<= 4;

                this->tx_bits(byte);
                
                LCD_CONTROL_PORT |=  (1 << LCD_EN_PIN);
                _delay_us(1);
                LCD_CONTROL_PORT &= ~(1 << LCD_EN_PIN);
                _delay_ms(2);
            }
        
        public:
            lcd(void)
            {
                /* Set data direction. */
                DDRC = 0xFF;
                DDRD = 0xFF;
                DDRL = 0xFF;

                _delay_ms(15);
                
                /* 4-bit mode, 1-line. */
                tx_cmd(0x02);
                tx_cmd(0x0C);
                tx_cmd(0x01);
            }

            void clear(void)
            {
                this->tx_cmd(0x01);
            }

            void write(char *byte)
            {
                while((*byte)!='\0')
                {
                    this->tx_data(*byte);
                    byte++;
                }
            }
    };

}