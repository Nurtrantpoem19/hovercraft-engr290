#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include <math.h>
// MPU6050 Default Address
#define MPU_ADDR 0x68
//---------------------------------------------------------------
// Registers
#define SMPRT_DIV 0x19 // Sample Rate Divider
#define CONFIG 0x1A // Configuration
#define GYRO_CONFIG 0x1B // Gyroscope Configuration
#define ACCEL_CONFIG 0x1C // Acceleromter Configuration
#define ACCEL_XOUT_H 0x3B // Accelerometer Measurement - X, High
#define ACCEL_XOUT_L 0x3C // Accelerometer Measurement - X, Low
#define ACCEL_YOUT_H 0x3D // Accelerometer Measurement - Y, High
#define ACCEL_YOUT_L 0x3E // Accelerometer Measurement - Y, Low
#define ACCEL_ZOUT_H 0x3F // Accelerometer Measurement - Z, High
#define ACCEL_ZOUT_L 0x40 // Accelerometer Measurement - Z, Low
#define GYRO_XOUT_H 0x43 // Gyroscope Measurement - X, High
#define GYRO_XOUT_L 0x44 // Gyroscope Measurement - X, Low
#define GYRO_YOUT_H 0x45 // Gyroscope Measurement - Y, High
#define GYRO_YOUT_L 0x46 // Gyroscope Measurement - Y, Low
#define GYRO_ZOUT_H 0x47 // Gyroscope Measurement - Z, High
#define GYRO_ZOUT_L 0x48 // Gyroscope Measurement - Z, Low
#define PWR_MGMT_1 0x6B // Power Management 1
#define WHO_AM_I 0x75 // Who Am I (Should return 0x68)

//---------------------------------------------------------------
// Register Bits
// ===GYRO_CONFIG===
#define FS_SEL 3 // [1:0]
// ===ACCEL_CONFIG===
#define AFS_SEL 3 // [1:0]
// ===PWR_MGMT_1===
#define CLKSEL 0 // 0 = Internal 8MHz oscillator, 1 = PLL with X axis gyroscope reference
#define SLEEP 6 // 0 = awake, 1 = sleep

//---------------------------------------------------------------
// Constants
#define DT 0.02f // 20ms step
#define PI 3.14159265f
#define RAD_TO_DEG (180.f / PI)
#define DEG_TO_RAD (PI / 180.f)
#define GRAVITY 9.80665f

//---------------------------------------------------------------
// Structs
typedef struct { // x y z
    int16_t x;
    int16_t y;
    int16_t z;
} vector3_int16_t;
typedef struct { // x y z (float)
    float x;
    float y;
    float z;
} vector3_t;
typedef struct { // yaw pitch roll
    float yaw;
    float pitch;
    float roll;
} rotation_t;
//---------------------------------------------------------------
// Global variables
extern vector3_t accel; // acceleration in g
extern vector3_t gyro; // angular velocity in deg/s
extern vector3_t acceleration; // acceleration in m/s^2
extern vector3_t velocity; // velocity in m/s
extern vector3_t position; // position in m
extern rotation_t rotation; // rotation in deg
//---------------------------------------------------------------
// Prototypes
uint8_t imu_init(void);
uint8_t imu_read(void);
uint8_t gyro_set_FS(uint8_t fs_sel);
uint8_t accel_set_FS(uint8_t afs_sel);
void convert_accel_raw(void);
void convert_gyro_raw(void);
void imu_calibrate(void);
void rotation_update(void);
void position_update(void);
#endif // IMU_H
//================================================================
//================================================================

#ifndef SERVO_H
#define SERVO_H

void set_servo_angle(float yaw);

#endif // SERVO_H
//================================================================
//================================================================

#ifndef TWI_290_H
#define TWI_290_H

#include <util/twi.h>

extern volatile uint8_t TWI_status, TWI_byte;

uint8_t TWI_start(uint8_t twi_addr, uint8_t read_write);

void TWI_stop(void);

uint8_t TWI_write(uint8_t tx_data); //write byte to the started device

uint8_t TWI_ack_read(void); // continuous read

uint8_t TWI_nack_read(void); //read and stop condition

uint8_t Read_Reg(uint8_t TWI_addr, uint8_t reg_addr);

uint8_t Read_Reg_N(uint8_t TWI_addr, uint8_t reg_addr, uint8_t bytes, uint8_t* data);

uint8_t Write_Reg(uint8_t TWI_addr, uint8_t reg_addr, uint8_t value);

#endif // TWI_290_H
//================================================================
//================================================================

#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <stdint.h>

void uart_init(uint32_t f_cpu, uint32_t baud);
void uart_transmit_char(char c);
void uart_transmit_string(const char *str);
void uart_transmit_uint16(uint16_t num);
void uart_transmit_float(float num);

void transmit_accel(float accel_x, float accel_y, float accel_z);
void transmit_gyro(float gyro_x, float gyro_y, float gyro_z);
void transmit_position(float pos_x, float pos_y, float pos_z);
void transmit_rotation(float yaw, float pitch, float roll);

// Assignment 1
// void transmit_IR();
// void transmit_US();
#endif // UART_DRIVER_Hinclude <avr/io.h>
//================================================================
//================================================================
