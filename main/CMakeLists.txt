project(SMTBED C CXX ASM)

add_executable(main
    main.c
    motor.h
    LTC2449.h
)
target_link_libraries(main 
        pico_stdlib
        hardware_gpio 
        hardware_pwm
        freertos
        hardware_i2c
        hardware_adc
        hardware_spi
     
 )

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)
pico_add_extra_outputs(main)