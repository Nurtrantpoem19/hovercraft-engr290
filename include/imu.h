#ifndef IMU_H
#define IMU_H

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
#define WHO_AM_I 0x75 // Who Am I

// Register Bits
// GYRO_CONFIG
#define FS_SEL 3 // Full Scale Range: 0 = +/-250, 1 = +/-500, 2 = +/-1000, 3 = +/-2000
// ACCELL_CONFIG
#define AFS_SEL 3 // Full Scale Range: 0 = +/-2g, 1 = +/-4g, 2 = +/-8g, 3 = +/-16g
// PWR_MGMT_1
#define CLKSEL 0 // 1 = gyroX ref, 2 = gyroY ref, 3 = gyroZ ref
#define SLEEP 6 // 0 = awake, 1 = sleep


#endif