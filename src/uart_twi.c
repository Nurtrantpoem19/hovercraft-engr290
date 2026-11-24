#include "common.h"

//===============================================================
//===============================================================
// begin uart

void uart_init(uint32_t f_cpu, uint32_t baud) {
    // Enable 2x speed mode
    UCSR0A |= (1 << U2X0);

    uint16_t ubrr = (uint16_t)((f_cpu / (8UL * baud)) - 1);
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr & 0xFF);

    UCSR0B = (1 << TXEN0);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit_char(char c) {
    while (!(UCSR0A & (1 << UDRE0)));

    UDR0 = c;
}

void uart_transmit_string(const char *str) {
    while (*str) {
        uart_transmit_char(*str++);
    }
}

void uart_transmit_uint16(uint16_t num) {
    char buffer[6];
    int i = 0;

    if (num == 0) {
        uart_transmit_char('0');
        return;
    }

    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    while (i > 0) {
        uart_transmit_char(buffer[--i]);
    }
}

void uart_transmit_float(float num) {
    char buffer[6];
    uint8_t i = 0;

    if (num < 0.f) {
        uart_transmit_char('-');
        num = -num;
    }

    uint16_t num_int = (uint16_t) num; // get the integer
    uint8_t num_decimal = (uint8_t) ((num - (float) num_int) * 100.f); // get the first 2 decmials
    // Integer part
    if (num_int == 0) {
        uart_transmit_char('0');
    }
    else {
        while (num_int > 0) {
            buffer[i++] = (num_int % 10) + '0';
            num_int /= 10;
        }
        while (i > 0) {
            uart_transmit_char(buffer[--i]); // print integer part
        }
    }
    
    

    uart_transmit_char('.'); // print decmial point

    uart_transmit_char((num_decimal / 10) + '0');
    uart_transmit_char((num_decimal % 10) + '0');
}

// void transmit_IR()
// {
//         uint16_t raw_val = sensorsIR_read();
//         uint16_t mv_val  = sensorsIR_raw_to_mv(raw_val);

//         uart_transmit_string("IR Raw: ");
//         uart_transmit_uint16(raw_val);
//         uart_transmit_string(" | Voltage: ");
//         uart_transmit_uint16(mv_val);
//         uart_transmit_string(" mV\r\n");


// }

// void transmit_US()
// {
//     uint16_t distance = sensorsUS_read();
    
//     uart_transmit_string("US Dist: ");
//     uart_transmit_uint16(distance);
//     uart_transmit_string(" cm\r\n");




// }

void transmit_accel(float accel_x, float accel_y, float accel_z) {
    uart_transmit_string("accel x: ");
    uart_transmit_float(accel_x);
    uart_transmit_string("g\r\n");

    uart_transmit_string("accel y: ");
    uart_transmit_float(accel_y);
    uart_transmit_string("g\r\n");

    uart_transmit_string("accel z: ");
    uart_transmit_float(accel_z);
    uart_transmit_string("g\r\n");
}

void transmit_gyro(float gyro_x, float gyro_y, float gyro_z) {
    uart_transmit_string("gyro x: ");
    uart_transmit_float(gyro_x);
    uart_transmit_string("deg/s\r\n");

    uart_transmit_string("gyro y: ");
    uart_transmit_float(gyro_y);
    uart_transmit_string("deg/s\r\n");

    uart_transmit_string("gyro z: ");
    uart_transmit_float(gyro_z);
    uart_transmit_string("deg/s\r\n");
}

void transmit_position(float pos_x, float pos_y, float pos_z) {
    uart_transmit_string("pos x: ");
    uart_transmit_float(pos_x);
    uart_transmit_string("m\r\n");

    uart_transmit_string("pos y: ");
    uart_transmit_float(pos_y);
    uart_transmit_string("m\r\n");

    uart_transmit_string("pos z: ");
    uart_transmit_float(pos_z);
    uart_transmit_string("m\r\n");
}

void transmit_rotation(float yaw, float pitch, float roll) {
    uart_transmit_string("yaw: ");
    uart_transmit_float(yaw);
    uart_transmit_string("deg\r\n");

    uart_transmit_string("pitch: ");
    uart_transmit_float(pitch);
    uart_transmit_string("deg\r\n");

    uart_transmit_string("roll: ");
    uart_transmit_float(roll);
    uart_transmit_string("deg\r\n");
}
// end uart
//===============================================================
//===============================================================
// begin twi
#include <util/twi.h>

volatile struct {
	uint8_t TX_new_data:1;
	uint8_t TX_finished:1;
	uint8_t TX_buffer1_empty:1;
	uint8_t TX_buffer2_empty:1;
	uint8_t RX_flag:3;
	uint8_t TWI_ACK:1;
} flags;

volatile uint8_t TWI_status, TWI_byte;

//=============================== TWI functions ======================

uint8_t TWI_start(uint8_t twi_addr, uint8_t read_write) {

	TWCR=((1<<TWINT)|(1<<TWSTA)|(1<<TWEN)); //send START condition
	while(!(TWCR&(1<<TWINT))); //wait until transmission completed
	if (((TWSR&0xF8)!=TW_START)&&((TWSR&0xF8)!=TW_REP_START)) return 1; //something went wrong

	twi_addr=((twi_addr<<1)|(read_write&1)); //setting r/w bit
	TWDR=twi_addr; //send device address
	TWCR=((1<<TWINT)|(1<<TWEN)); //reset the flag

	while(!(TWCR&(1<<TWINT))); // wail until transmission completed and ACK/NACK has been received
	if (((TWSR&0xF8)!=TW_MT_SLA_ACK)&&((TWSR&0xF8)!=TW_MR_SLA_ACK)) return 2;
	return 0;
}

void TWI_stop(void) {
	TWCR=((1<<TWINT)|(1<<TWEN)|(1<<TWSTO)); // send stop condition
	while(TWCR&(1<<TWSTO)); // wait until stop condition is executed and bus released
}

uint8_t TWI_write(uint8_t tx_data) //write byte to the started device
{	
	TWDR=tx_data;
	TWCR=((1<<TWINT)|(1<<TWEN));
	while(!(TWCR & (1<<TWINT))); // wait until transmission completed
	if((TWSR&0xF8)!=TW_MT_DATA_ACK) return 1; //check value of TWI Status Register. Mask prescaler bits. Write failed
	return 0;
}

uint8_t TWI_ack_read(void) // continuous read
{
	TWCR=((1<<TWINT)|(1<<TWEN)|(1<<TWEA)); // Start read cycle
	while(!(TWCR&(1<<TWINT)));
	flags.TWI_ACK=1;    
	if((TWSR&0xF8)!=TW_MR_DATA_ACK) flags.TWI_ACK=0; //check value of TWI Status Register. Mask prescaler bits. Read failed
    return TWDR;
}


uint8_t TWI_nack_read(void) //read and stop condition
{
	TWCR=((1<<TWINT)|(1<<TWEN));
	while(!(TWCR&(1<<TWINT)));
	flags.TWI_ACK=1;    
	if((TWSR&0xF8)!=TW_MR_DATA_NACK) return flags.TWI_ACK=0; //check value of TWI Status Register. Mask prescaler bits. Read failed
    return TWDR;
}


uint8_t Read_Reg(uint8_t TWI_addr, uint8_t reg_addr){

	TWI_status=TWI_start (TWI_addr, TW_WRITE);
	if (TWI_status) return 1;

	TWI_status=TWI_write (reg_addr); //  register #
	if (TWI_status) return 2;

	TWI_status=TWI_start (TWI_addr, TW_READ);
	if (TWI_status) return 3;

	TWI_byte=TWI_nack_read();
	TWI_stop();
	if (!flags.TWI_ACK) return 4;
	return 0;
}


uint8_t Read_Reg_N(uint8_t TWI_addr, uint8_t reg_addr, uint8_t bytes, uint8_t* data){ 
	
	uint8_t *p_data=(uint8_t*)data;

	TWI_status=TWI_start (TWI_addr, TW_WRITE);
	if (TWI_status) return 1;

	TWI_status=TWI_write (reg_addr); //  register #
	if (TWI_status) return 2;

	TWI_status=TWI_start (TWI_addr, TW_READ);
	if (TWI_status) return 3;
//	p_data=(uint8_t*)data;	
	for (uint8_t i=0; i<bytes-1; i++) {
		*p_data=TWI_ack_read();
		if (!flags.TWI_ACK) return 5;
		p_data++;	
	}
	*p_data=TWI_nack_read();
	TWI_stop();
	if (!flags.TWI_ACK) return 4;
	return 0;
}


uint8_t Write_Reg(uint8_t TWI_addr, uint8_t reg_addr, uint8_t value){

	TWI_status=TWI_start (TWI_addr, TW_WRITE);
	if (TWI_status) return 1;

	TWI_status=TWI_write (reg_addr); //  register #
	if (TWI_status) return 2;

	TWI_status=TWI_write (value); // write the value
	if (TWI_status) return 3;

	TWI_stop();
	if (!flags.TWI_ACK) return 4;

	return 0;
}

//=============================== TWI functions end ===================
//=====================================================================