cmake_minimum_required(VERSION 3.16)


set(MCU atmega328p)
set(F_CPU 16000000UL)

project(hovercraft_firmware C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

enable_testing()

file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)


add_executable(${PROJECT_NAME}.elf
    ${SOURCES}
)

add_executable(test_pwm.elf
    tests/test_pwm.c
    src/servo.c
)
add_executable(test_servo2.elf
    tests/test_servo2.c
    src/servo.c
    src/servo2.c
)
add_executable(test_gyro_controller.elf
    tests/test_gyro_controller.c
    src/gyro_controller.c
    src/imu.c
    src/TWI_290.c
    src/servo.c
    src/uart.c
)
add_executable(test_thrust.elf
    tests/test_thrust.c
    src/fan_thrust.c
)
add_executable(test_sensor.elf
    tests/test_sensor.c
    src/uart.c
    src/adc.c
    src/sensors.c
)
add_executable(test_ir.elf
    tests/test_ir.c
    src/uart.c
    src/adc.c
    src/sensors.c
)
add_executable(test_steer_withoutthrust.elf
    tests/test_steer_withoutthrust.c
    src/servo.c
    src/adc.c
    src/sensors.c
    src/uart.c
    src/TWI_290.c
    src/imu.c
)
add_executable(test_steering.elf
    tests/test_steering.c
    src/steering.c
    src/servo.c
    src/fan_thrust.c
    src/adc.c
    src/sensors.c
    src/uart.c
)
add_executable(test_lift.elf
    tests/test_lift.c
    src/fan_lift.c
)
add_executable(test_us_with_fans.elf
    tests/test_us_with_fans.c
    src/uart.c
    src/sensors.c
    src/adc.c
    src/fan_lift.c
    src/fan_thrust.c
)
add_executable(test_us2.elf
    tests/test_us2.c
    src/uart.c
    src/sensors.c
    src/adc.c
)

target_include_directories(test_pwm.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(test_servo2.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(test_gyro_controller.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(test_thrust.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(test_sensor.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(test_ir.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(test_steer_withoutthrust.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(test_steering.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(test_lift.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(test_us_with_fans.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(test_us2.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)


target_include_directories(${PROJECT_NAME}.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)

target_compile_options(test_thrust.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)
target_compile_options(test_sensor.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)
target_compile_options(test_ir.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)
target_compile_options(test_steer_withoutthrust.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)
target_compile_options(test_steering.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)
target_compile_options(test_lift.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)
target_compile_options(test_us_with_fans.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)
target_compile_options(test_us2.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)
target_compile_options(test_pwm.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)

target_compile_options(test_servo2.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)

target_compile_options(test_gyro_controller.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)

foreach(test_target
    test_pwm.elf
    test_servo2.elf
    test_gyro_controller.elf
    test_thrust.elf
    test_sensor.elf
    test_ir.elf
    test_steering.elf
    test_lift.elf
    test_us_with_fans.elf
    test_us2.elf
)
    target_link_options(${test_target} PRIVATE
        -mmcu=${MCU}
        -Wl,--gc-sections
    )
endforeach()

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -mmcu=${MCU}
    -Wl,--gc-sections
)

target_link_options(test_steer_withoutthrust.elf PRIVATE
    -mmcu=${MCU}
    -Wl,--gc-sections
)
target_link_libraries(test_steer_withoutthrust.elf PRIVATE m)
target_link_libraries(test_gyro_controller.elf PRIVATE m)

set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    SUFFIX ".elf"
)
