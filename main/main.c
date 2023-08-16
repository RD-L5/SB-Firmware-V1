#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "motor.h"
#include "LTC2449.h"


const uint FAULT_a_pin = 25;
const uint PWM_A1_pin = 20;
const uint ENA_A1_pin = 23;
const uint PWM_B1_pin = 29;
const uint ENA_B1_pin = 22;
const uint PWM_C1_pin = 28;
const uint ENA_C1_pin = 21;

const uint FAULT_b_pin = 10;
const uint PWM_A2_pin = 15;
const uint ENA_A2_pin = 12;
const uint PWM_B2_pin = 26;
const uint ENA_B2_pin = 13;
const uint PWM_C2_pin = 27;
const uint ENA_C2_pin = 14;

const uint NSLEEP = 24;

// SPI Pin
#define MISO    16
#define CS      17
#define SCLK    18
#define MOSI    19
#define SPI_PORT spi0



int motor1[6] = {20, 21, 22, 23, 28, 29};
int motor2[6] = {12, 13, 14, 15, 26, 27};

int8_t LTC2445_EOC_timeout(uint16_t miso_timeout){
    uint16_t timer_count = 0;            
    gpio_put(CS, 0);                     
    while (1){
        if (gpio_get(MISO) == 0){
            break;        
        }
        if (timer_count++ > miso_timeout) {
            gpio_put(CS, 1);               
            return 1;
        } else {
            sleep_ms(1);
        }
    }
    gpio_put(CS, 1);                     
    return 0;
}

uint32_t LTC2445_read(uint16_t adc_command){
    uint8_t data[4];
    uint8_t command[4];

    command[0] = (adc_command >> 8) & 0xFF;;
    command[1] = adc_command & 0xFF;
    command[2] = 0;
    command[3] = 0;

    gpio_put(CS, 0);
    spi_write_read_blocking(SPI_PORT, command, data, 4);
    gpio_put(CS, 1);

    return (((uint32_t)data[0] & 0xFF) << 24) | (((uint32_t)data[1] & 0xFF) << 16) | (((uint32_t)data[2] & 0xFF) << 8) | ((uint32_t)data[3] & 0xFF);
}

void led_1_task()
{   
    const uint LED_PIN = 7;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (1) {
        gpio_put(LED_PIN, 1);
        vTaskDelay(10);
        gpio_put(LED_PIN, 0);
        vTaskDelay(10);
    }
}

void ltc2449_task()
{   
    spi_init(SPI_PORT, 500 * 1000);
    gpio_set_function(MISO, GPIO_FUNC_SPI);
    gpio_set_function(SCLK, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);

    gpio_init(CS);
    gpio_set_dir(CS, GPIO_OUT);
    gpio_put(CS, 1);

    while (1) {
        uint16_t miso_timeout = 1000;
        uint16_t adc_command = LTC2449_P6_N7 | LTC2449_OSR_32768;                
        if(LTC2445_EOC_timeout(miso_timeout)){
            printf("Timeout");
        } else{
            // printf("CHANNEL 1 :%x\n", LTC2445_read(adc_command));
        }

        uint16_t adc_command6 = LTC2449_P0_N1 | LTC2449_OSR_32768;        
        if(LTC2445_EOC_timeout(miso_timeout)){
            printf("Timeout");
        } else{
            // printf("CHANNEL 6 :%x\n", LTC2445_read(adc_command6));
        }

        // printf("=======================\n");
        
        vTaskDelay(50);
    }
}

void motor_task()
{   
  

    while (1) {
        motor1_CC(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        vTaskDelay(200);
        motor1_OFF(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        vTaskDelay(100);
        motor1_CCW(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        vTaskDelay(200);
        motor1_OFF(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        vTaskDelay(100);
        motor2_CC(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        vTaskDelay(200);
        motor1_OFF(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        vTaskDelay(100);
        motor2_CCW(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        vTaskDelay(200);
        motor1_OFF(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        vTaskDelay(100);
        
        motor1_B_CC(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin,ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        vTaskDelay(200);
        motor2_OFF(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin, ENA_B2_pin, PWM_C2_pin,ENA_C2_pin);
        vTaskDelay(100);
        motor1_B_CCW(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin,ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        vTaskDelay(200);
        motor2_OFF(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin, ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        vTaskDelay(100);
        motor2_B_CC(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin,ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        vTaskDelay(200);
        motor2_OFF(PWM_A2_pin, ENA_A2_pin,PWM_B2_pin,ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        vTaskDelay(100);
        motor2_B_CCW(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin,ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        vTaskDelay(200);
        motor2_OFF(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin, ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        vTaskDelay(100);
    }
 }
void piezo_task()
{   
   
   // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(3, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(3);
    pwm_set_wrap(slice, 125);  // 100 cycles (1 cycle = 1/100 seconds)
    pwm_set_chan_level(slice, PWM_CHAN_A, 60);  // 50% duty cycle (half of the period)
    pwm_set_clkdiv(slice, 25000 ); //30000
    pwm_set_enabled(slice, true);
    vTaskDelay(50);
    
    
    while (1) {
         pwm_set_enabled(slice, false);
         vTaskDelay(50);
    }
}

void uart0_task()
    {   

        // static int chars_rxed = 0;
        // uint8_t rxChar[16];
        // UART0 RX IRQ
        uart_init(uart0, 115200);                        // Initialize UART0
        gpio_set_function(0, GPIO_FUNC_UART);           // Pin Functions
        gpio_set_function(1, GPIO_FUNC_UART);           // Pin Functions
        uart_set_hw_flow(uart0, false, false);           // Default UART
        uart_set_format(uart0, 8, 1, UART_PARITY_NONE);  // Default UART
        uart_set_fifo_enabled(uart0, false);             // Turn off FIFO's - we want to do this character by character
        irq_set_enabled(UART0_IRQ, true);                // Enable Interrupt
        uart_set_irq_enables(uart0, true, false);        // Now enable the UART to send interrupts - RX only
    
        while (1) {

            while (uart_is_readable(uart0)) {
                // // uint8_t rxChar = uart_getc(UART_ID);
                // uart_read_blocking(uart0, rxChar, 11);
                // printf("uart 0: %s\n", rxChar);
                // Can we send it back?
            if (uart_is_writable(uart0)) {
                // Change it slightly first!
            
                // uart_putc(UART_ID, rxChar);
            }
            // chars_rxed++;
        }
        vTaskDelay(100);
    }
}
            
void uart1_task()
    {   
        // UART Defines
        // #define UART1_TX 4
        // #define UART1_RX 5

        #define UART_ID uart1
        static int chars_rxed = 0;
        uint8_t rxChar[16];
        // UART0 RX IRQ
        uart_init(uart1, 115200);                        // Initialize UART0
        gpio_set_function(4, GPIO_FUNC_UART);           // Pin Functions
        gpio_set_function(5, GPIO_FUNC_UART);           // Pin Functions
        uart_set_hw_flow(uart1, false, false);           // Default UART
        uart_set_format(uart1, 8, 1, UART_PARITY_NONE);  // Default UART
        uart_set_fifo_enabled(uart1, false);             // Turn off FIFO's - we want to do this character by character
        //irq_set_enabled(UART1_IRQ, true);                // Enable Interrupt
        uart_set_irq_enables(uart1, true, false);
        while (1) {

            while (uart_is_readable(uart1)) {

                uint8_t rxChar = uart_getc(UART_ID);
                //uart_read_blocking(uart1, rxChar, 11);
                printf("%c", rxChar);
                vTaskDelay(100);
            // if (rxChar == 0x0A) {
            //         printf("\n");

            //     }
            }
                // Can we send it back?
        //     if (uart_is_writable(uart1)) {
        //         // Change it slightly first!
            
        //         // uart_putc(UART_ID, rxChar);
        //     }
        //     //chars_rxed++;
        // }
        // vTaskDelay(100);
    }
 }
int main()
{
    stdio_init_all();
    
    gpio_init(NSLEEP);
    gpio_set_dir(NSLEEP, GPIO_OUT);
    gpio_put(NSLEEP, true);
    
    gpio_init(FAULT_a_pin);
    gpio_set_dir(FAULT_a_pin, GPIO_OUT);
    gpio_put(FAULT_a_pin, true);

    gpio_init(FAULT_b_pin);
    gpio_set_dir(FAULT_b_pin, GPIO_OUT);
    gpio_put(FAULT_b_pin, true);
    for (uint8_t i = 0; i <6 ; ++i){
        gpio_init(motor1[i]);
        gpio_init(motor2[i]);
    }

    for (uint8_t i = 0; i <6 ; ++i){
        gpio_set_dir(motor1[i], GPIO_OUT);
        gpio_set_dir(motor2[i], GPIO_OUT);
    }
    xTaskCreate(led_1_task, "LED_Task_1", 256, NULL, 1, NULL);
    xTaskCreate(ltc2449_task, "LTC2449", 256, NULL, 1, NULL);
    xTaskCreate(motor_task, "motor_task", 256, NULL, 1, NULL);
    xTaskCreate(piezo_task, "piezo", 256, NULL, 1, NULL);
    //xTaskCreate(uart0_task,"Uart0",256, NULL, 1, NULL);
    // xTaskCreate(uart1_task,"Uart",256, NULL, 1, NULL);

    vTaskStartScheduler();
    
    while(1){};
    
}