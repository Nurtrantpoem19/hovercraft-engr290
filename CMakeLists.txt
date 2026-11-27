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

target_include_directories(test_pwm.elf PRIVATE
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

target_include_directories(test_steering.elf PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(test_lift.elf PRIVATE
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
target_compile_options(test_pwm.elf PRIVATE
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
    test_thrust.elf
    test_sensor.elf
    test_ir.elf
    test_steering.elf
    test_lift.elf
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

set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    SUFFIX ".elf"
)
