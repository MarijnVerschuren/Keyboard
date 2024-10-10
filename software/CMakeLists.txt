cmake_minimum_required(VERSION 3.0)

# project name
project(STM32F412 C ASM)

# toolchain
set(CMAKE_TOOLCHAIN_FILE "cmake/arm-none-eabi.cmake")

# select linker script
set(LINKER_SCRIPT "lnk/STM32F412.ld")

# define CPU OPTIONS
set(CPU_OPTIONS -mthumb -mcpu=cortex-m4)

# optimizations (-O0 -O1 -O2 -O3 -Os -Ofast -Og -flto)
set(CMAKE_C_FLAGS_DEBUG "-O0 -g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-Ofast -flto")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -flto")

add_compile_options(
        ${CPU_OPTIONS}
        $<$<COMPILE_LANGUAGE:C>:-std=c11>
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
        $<$<COMPILE_LANGUAGE:CXX>:-fms-extensions>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
        $<$<COMPILE_LANGUAGE:CXX>:-Wold-style-cast>
        $<$<COMPILE_LANGUAGE:CXX>:-Wsuggest-override>
        -fstrict-volatile-bitfields
        -ffunction-sections

#        -Werror
#        -Wall
#        -Wextra
#        -Wcast-align
#        -Wconversion
#        -Wsign-conversion
#        -Wshadow
#        -Wlogical-op
#        -Wsuggest-final-types
#        -Wsuggest-final-methods
#        -pedantic
)

include_directories(
        "inc/"
        "test/"
        "lib/"
)

link_directories(
        ${CMAKE_SOURCE_DIR}
)

# list of modules to build final firmware
add_executable(${PROJECT_NAME}
        src/vector.S
        src/startup.S
        src/sys.S
        src/NVIC.S
        src/GPIO.S
        src/EXTI.S
        src/USART.S

        src/main.c
        src/sys.c
        src/USART.c
        src/RTC.c
        src/I2C.c
        src/tim.c

        test/test.S

        lib/AS5600/AS5600.c
        src/ADC.c
        src/SPI.c
)

target_link_libraries(${PROJECT_NAME}
        ${CPU_OPTIONS}
        -T${LINKER_SCRIPT}
        -nostartfiles
        m
        # -nostdlib
)

set_property(TARGET ${PROJECT_NAME} PROPERTY LINK_DEPENDS ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT})

include("cmake/flash.cmake")
