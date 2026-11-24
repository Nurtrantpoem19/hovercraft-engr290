#include <stdint.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "common.h"
#include <avr/interrupt.h>

#define PWM_TOP 2500

// Main Timer
volatile uint8_t timer1_flag = 0;

void timer1_init(void) {
    TCCR1A = (1 << COM1A1);
    TCCR1B = (1 << WGM13) | (1 << CS11) | (1 << CS10); // phase correct PWM, prescaler = 64
    ICR1 = PWM_TOP; // 50hz PWM
    OCR1A = 187; // middle point (0 degrees)
    TIMSK1 |= (1 << ICIE1); // enable interrupt
}

ISR(__vector_default) { // capture it all
}


ISR(TIMER1_CAPT_vect) {
    timer1_flag = 1; // starts IMU sampling
}

void update_led() {
    if (rotation.yaw > 85.f || rotation.yaw < -85.f) { // yaw outside range [-85, 85]
        PORTB |= (1 << PB5); // turn on LED
    } else {
        PORTB &= ~(1 << PB5); // turn off LED
    }
}

void gpio_init() {
  cli();
  DDRB= (1 << PB5) | (1 << PB1); // LED and PWM output 
  PORTB=((1<<PB4) | (1<<PB0)); // set High

  PORTC=((1<<PC5)|(1<<PC4)); // for TWI pins
  DIDR0=((1<<PC3)|(1<<PC2)|(1<<PC1)|(1<<PC0)); // disable digital inputs on analog pins.

  DDRD=(1<<PD7)|(1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD1); //Set PD1 (TX) and PD7-PD4 (power control) as outputs.
  PORTD=((1<<PD1)|(1<<PD2)|(1<<PD3)); // Set PD1(TX) to HIGH (idle) and enable pull-ups on PD3 and PD2.
  sei(); 
}

int main(void) {
    //=================Assignment 1======================
    // Initialize hardware peripherals
    // uart_init(F_CPU, 9600);
    // Assignment 1 stuff
    // uart_transmit_string("System Initialized. Reading IR Sensor...\r\n");
    // adc_init();
    // sensorsUS_init();
    // uint16_t ir_counter = 0;
    //===================================================

    uart_init(F_CPU, 115200);
    uint16_t imu_counter = 0;
    uint16_t printx_counter = 0;
    imu_init();

    // full scale range selection, uncomment the one to test (default is 250deg/s & 2g)
    // gyro_set_FS(0);
    // gyro_set_FS(1);
    // gyro_set_FS(2);
    // gyro_set_FS(3);
    // accel_set_FS(0);
    // accel_set_FS(1);
    // accel_set_FS(2);
    // accel_set_FS(3);


    imu_calibrate();

    _delay_ms(1000);

    timer1_init();
    gpio_init();

    sei();

    while (1) {
        //=================Assignment 1======================
        // transmit_IR();
        // ir_counter++;
        // if (ir_counter >= 10)
        // {
        //     transmit_US();
        //     ir_counter = 0; 
        // }
        // _delay_ms(200);
        //===================================================

        

        if (timer1_flag) {
            timer1_flag = 0; // reset the flag
            printx_counter += 20; // + 20ms elapsed
            imu_counter += 20; // + 20ms elapsed
            // imu
            imu_read();
            convert_accel_raw();
            convert_gyro_raw();
            rotation_update();
            position_update();
            
            // servo
            set_servo_angle(rotation.yaw);

            // LED
            update_led();

            if (imu_counter >= 1500) { // 1500ms passed
                imu_counter = 0; // reset counter
                transmit_accel(accel.x, accel.y, accel.z);
                transmit_rotation(rotation.yaw, rotation.pitch, rotation.roll);

            } 

            if (printx_counter >= 1000) { // 1000ms passed
                printx_counter = 0; // reset counter
                uart_transmit_string("Position: ");
                uart_transmit_float(position.x);
                uart_transmit_string("\r\n");
            }
        }

        
    }

    return 0;
}
