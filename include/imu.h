#ifndef IMU_H
#define IMU_H

#include <stdint.h>
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
// Data storage from the IMU
typedef struct { // Raw acceleration values
    int16_t x;
    int16_t y;
    int16_t z;
} accel_raw_t;
typedef struct { // Raw gyroscope values
    int16_t x;
    int16_t y;
    int16_t z;
} gyro_raw_t;
typedef struct { // converted accel values (g)
    int16_t x;
    int16_t y;
    int16_t z;
} accel_t;
typedef struct { // converted gyro values (deg/s)
    int16_t x;
    int16_t y;
    int16_t z;
} gyro_t;

//---------------------------------------------------------------
// Prototypes
uint8_t imu_init(void);
uint8_t imu_read(accel_raw_t *accel, gyro_raw_t *gyro);
uint8_t gyro_set_FS(uint8_t fs_sel);
uint8_t accel_set_FS(uint8_t afs_sel);
void convert_accel_raw(accel_raw_t *accel_raw, accel_t *accel);
void convert_gyro_raw(gyro_raw_t *gyro_raw, gyro_t *gyro);

#endif