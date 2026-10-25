#include "../include/imu.h"
#include "../include/TWI_290.h"
#include <avr/io.h>
#include <util/delay.h>

extern volatile uint8_t TWI_status, TWI_byte;
uint16_t accel_sens; // LSB/g
float gyro_sens = 131.0f; // LSB/deg/s
//---------------------------------------------------------------
// Global variables
vector3_t accel;
vector3_t gyro;
vector3_t position;
rotation_t rotation; // yaw pitch roll
//---------------------------------------------------------------
// Static variables
static vector3_int16_t accel_raw;
static vector3_int16_t gyro_raw;

static vector3_int16_t accel_offset;
static vector3_int16_t gyro_offset;

// 0 = +/-250, 1 = +/-500, 2 = +/-1000, 3 = +/-2000
uint8_t gyro_set_FS(uint8_t fs_sel) {
    if (fs_sel > 3) return 2; // invalid input
    TWI_status = Write_Reg(MPU_ADDR, GYRO_CONFIG, fs_sel << FS_SEL);
    if (TWI_status) return 1; // TWI error
    switch (fs_sel) {
        case 0:
            gyro_sens = 131.f;
            break;
        case 1:
            gyro_sens = 65.5f;
            break;
        case 2:
            gyro_sens = 32.8f;
            break;
        case 3:
            gyro_sens = 16.4f;
            break;
        
    }
    return 0;
}

// 0 = +/-2g, 1 = +/-4g, 2 = +/-8g, 3 = +/-16g
uint8_t accel_set_FS(uint8_t afs_sel) {
    if (afs_sel > 3) return 2; // invalid input
    TWI_status = Write_Reg(MPU_ADDR, ACCEL_CONFIG, afs_sel << AFS_SEL);
    if (TWI_status) return 1; // TWI error
    switch (afs_sel) {
        case 0:
            accel_sens = 16384;
            break;
        case 1:
            accel_sens = 8192;
            break;
        case 2:
            accel_sens = 4096;
            break;
        case 3:
            accel_sens = 2048;
            break;
    }
    return 0;
}

void imu_calibrate(void) {
    int32_t sum_ax = 0;
    int32_t sum_ay = 0;
    int32_t sum_az = 0;
    int32_t sum_gx = 0;
    int32_t sum_gy = 0;
    int32_t sum_gz = 0;
    
    for (int i = 0; i < 1000; i++) {
        imu_read();
        if (i >= 100) { // discard the first 100 readings
            sum_ax += accel_raw.x;
            sum_ay += accel_raw.y;
            sum_az += accel_raw.z;
            sum_gx += gyro_raw.x;
            sum_gy += gyro_raw.y;
            sum_gz += gyro_raw.z;
        }
        
        _delay_ms(1);
    }
    // divide by 900 to get average offset value
    accel_offset.x = (int16_t) (sum_ax / 900);
    accel_offset.y = (int16_t) (sum_ay / 900);
    accel_offset.z = (int16_t) ((sum_az / 900) - accel_sens);
    gyro_offset.x = (int16_t) (sum_gx / 900);
    gyro_offset.y = (int16_t) (sum_gy / 900);
    gyro_offset.z = (int16_t) (sum_gz / 900);
}

uint8_t imu_init() {
    // Ensure WHO_AM_I is correct
    TWI_status = Read_Reg(MPU_ADDR, WHO_AM_I);
    if (TWI_status) return 1; // TWI error
    if (TWI_byte != MPU_ADDR) return 2; // Incorrect address read
    
    // Configuration
    uint8_t status;
    TWI_status = Write_Reg(MPU_ADDR, PWR_MGMT_1, (0 << SLEEP) | (1 << CLKSEL)); // Wake and select PLL gyro X-axis clock
    if (TWI_status) return 1; // TWI error
    status = gyro_set_FS(0); // Set full scale range to +/-250
    if (status) return status;
    status = accel_set_FS(0); // Set full scale range to +/-2g
    if (status) return status;

    imu_calibrate();

    return 0;
}

// gets the raw data for gyro and accel
uint8_t imu_read() {
    // Read data
    uint8_t data[14];
    TWI_status = Read_Reg_N(MPU_ADDR, ACCEL_XOUT_H, 14, data);
    if (TWI_status) return 1; // TWI error

    // Accel bytes 0-5
    accel_raw.x = (int16_t) ((data[0] << 8) | data[1]);
    accel_raw.y = (int16_t) ((data[2] << 8) | data[3]);
    accel_raw.z = (int16_t) ((data[4] << 8) | data[5]);
    // Temp bytes 6-7 not needed
    // Gyroscope (bytes 8 - 13)
    gyro_raw.x  = (int16_t)  ((data[8]  << 8) | data[9]);
    gyro_raw.y  = (int16_t)  ((data[10] << 8) | data[11]);
    gyro_raw.z  = (int16_t)  ((data[12] << 8) | data[13]);
    return 0;
}

// accel = accel_raw / accel_sens
void convert_accel_raw() {
    accel.x = (float) (accel_raw.x - accel_offset.x) / accel_sens;
    accel.y = (float) (accel_raw.y - accel_offset.y) / accel_sens;
    accel.z = (float) (accel_raw.z - accel_offset.z) / accel_sens;
}

// gyro = gyro_raw / gyro_sens
void convert_gyro_raw() {
    gyro.x = (float) (gyro_raw.x - gyro_offset.x) / gyro_sens;
    gyro.y = (float) (gyro_raw.y - gyro_offset.y) / gyro_sens;
    gyro.z = (float) (gyro_raw.z - gyro_offset.z) / gyro_sens;
}

// gyro_yaw, gyro_pitch, gyro_roll
void rotation_update() {

}