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

target_include_directories(test_pwm.elf PRIVATE
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

target_compile_options(test_pwm.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -mmcu=${MCU}
    -Wl,--gc-sections
)

set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    SUFFIX ".elf"
)
