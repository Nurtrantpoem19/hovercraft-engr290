#include "../include/imu.h"
#include "../include/twi_290.h"
#include <avr/io.h>

extern volatile uint8_t TWI_status, TWI_byte;

// 0 = +/-250, 1 = +/-500, 2 = +/-1000, 3 = +/-2000
uint8_t gyro_set_FS(uint8_t fs_sel) {
    if (fs_sel > 3) return 2; // invalid input
    TWI_status = Write_Reg(MPU_ADDR, GYRO_CONFIG, fs_sel << FS_SEL);
    if (TWI_status) return 1; // TWI error
    return 0;
}

// 0 = +/-2g, 1 = +/-4g, 2 = +/-8g, 3 = +/-16g
uint8_t accel_set_FS(uint8_t afs_sel) {
    if (afs_sel > 3) return 2; // invalid input
    TWI_status = Write_Reg(MPU_ADDR, ACCEL_CONFIG, afs_sel << AFS_SEL);
    if (TWI_status) return 1; // TWI error
    return 0;
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
    return 0;
}