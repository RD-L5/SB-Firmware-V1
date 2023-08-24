#include <stdio.h>
#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "motor.h"
#include "LTC2449.h"
#include "ds3231.h"

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

//UART 0
#define BUF_SIZE_0 1024
#define ARRAY_SIZE_0 16
// UART RX Variables
static char uart_buf_0[BUF_SIZE_0];
static uint16_t read_index_0 = 0;
static uint16_t write_index_0 = 0;
static uint16_t uart_count_0 = 0;
uint8_t process_buf_0[64];
char charToPrintf[64];
char charToString[64];
char toPrint_0[64];
uint8_t charToPrintf_count = 0;
int8_t setTimeFlag_0 = 0;
int8_t modulusCount_0 = 0;
int8_t dataReady_0 = 0;

//UART 1
#define BUF_SIZE 1024
#define ARRAY_SIZE 16
// UART RX Variables
static char uart_buf[BUF_SIZE];
static uint16_t read_index = 0;
static uint16_t write_index = 0;
static uint16_t uart_count = 0;
static char process_buf[64];
char charToPrintf_1[20];
char charToString_1[64];
char toPrint[64];
uint8_t charToPrintf_count_1 = 0;
int8_t setTimeFlag = 0;
int8_t modulusCount = 0;
int8_t dataReady = 0;

int motor1[6] = {20, 21, 22, 23, 28, 29};
int motor2[6] = {12, 13, 14, 15, 26, 27};

char data_UART0[10][32];
char data_UART1[10][32];
char data_Inject[10][32];
char tempData1[10][32];        
char tempData2[10][32];        

int toInt0[10];
int toInt1[10];

int dataDegree[4];

char dataFromInject[64];
int data_br = 0;
int data_kr = 0;
int data_hl = 0;
int data_tr = 0;
int dataInclinometer = 0;
int thresshold = 3;

//RING BUFFER UART 0
void write_cb_0(char c)
{
    if(c == 0x0D){
        ;
    }else if(c == 0x0A){     
        int token_count = 0;
        int data_count = 0;                      
        sprintf(charToString,"%s", charToPrintf);
        // printf("%s",charToString);
        memset(charToPrintf, '\0', sizeof(charToPrintf));
        charToPrintf_count = 0;
        
        // ========== Split String ================
        char * token = strtok(charToString, ","); // https://www.educative.io/answers/splitting-a-string-using-strtok-in-c
        while( token != NULL ) {
            // if(token_count > 0){
            // }            
            sprintf(data_UART0[token_count], "%s", token);
            // printf( "token %d : %s\n",token_count, token ); //printing each token
            // printf("%s", token);
            token = strtok(NULL, ",");
            token_count++;
        }
        // =================================================

        // ========== convert String to Int || data_UART0[0] is data Inject || data_UART0[>1] is data side guard ================
        // for(int i=0;i<token_count;i++){
        //     toInt0[i] = atoi(data_UART0[i]);
        //     // printf("uart0[%d] = %d\n",i, toInt0[i]);
        // }
        // =================================================

        // ========== copy data to data_br variable ================
        if(token_count > 4){    
            
            sprintf(dataFromInject,"%s", data_UART0[0]);            

            // ========== Split String ================
            char * inject = strtok(dataFromInject, "#"); // https://www.educative.io/answers/splitting-a-string-using-strtok-in-c
            while( inject != NULL ) {
                // if(data_count > 0){
                // }            
                sprintf(data_Inject[data_count], "%s", inject);
                // printf( "inject %d : %s\n",data_count, inject ); //printing each inject
                // printf("%s\n", inject);
                inject = strtok(NULL, "#");
                data_count++;
            }
        // =================================================

        // ========== Getting Degree Value ================
            for(int i=0;i<data_count;i++){
                // printf("%s\n", data_Inject[i]);            
                if(data_Inject[i][3] > 47 && data_Inject[i][3] < 58){
                    // printf("%c", data_Inject[i][2]);
                    // printf("%c", data_Inject[i][3]);
                    sprintf(tempData1[i], "%c", data_Inject[i][2]);
                    sprintf(tempData2[i], "%c", data_Inject[i][3]);
                    strcat(tempData1[i], tempData2[i]);
                    // printf("%s", tempData1[i]);
                }
                else{
                    // printf("%c", data_Inject[i][2]);
                    sprintf(tempData1[i], "%c", data_Inject[i][2]);
                    // printf("%s", tempData1[i]);                
                }
                // printf("\n");
            }
        // =================================================

        // ========== convert String to Int ================
            for(int i=0;i<data_count;i++){
                dataDegree[i] = atoi(tempData1[i]);
                // printf("uart0[%d] = %d\n",i, dataDegree[i]);

                if(dataDegree[0] == 99){
                    data_br = dataInclinometer;
                }
                else{
                    data_br = dataDegree[0];
                }
                // printf("data from inject: %s\n", dataFromInject);
            }
        // =================================================
        }
        // =================================================
        
        // printf("%d\n", dataDegree[0]);

    }else{
        charToPrintf[charToPrintf_count] = c;
        charToPrintf_count++;
    }
    uart_buf_0[write_index_0] = c;
    write_index_0 = (write_index_0 + 1) % BUF_SIZE_0;
    uart_count_0++;
}

char read_cb_0(void)
{
    char c = uart_buf_0[read_index_0];
    read_index_0 = (read_index_0 + 1) % BUF_SIZE_0;
    uart_count_0--;
    return c;
}

void UART0_ISR()
{
    uint8_t RX_i = 0;
    while (uart_is_readable(uart0))
    {
        if(setTimeFlag_0 == 0){
            uart_read_blocking(uart0, (uint8_t *) &toPrint_0, 15);
            setTimeFlag_0 = 1;
        }else{            
            if(modulusCount_0 > 3){
                write_cb_0(uart_getc(uart0));        
                modulusCount_0 = 0;
            }
            modulusCount_0++;
        }
    }
}

char *sliceString_0(char *str, int start, int end)
{

    int i;
    int size = (end - start) + 1;
    char *output = (char *)malloc(size * sizeof(char));

    for (i = 0; start <= end; start++, i++)
    {
        output[i] = str[start];
    }

    output[size] = '\0';

    return output;
}

void print_array_0(char *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        // printf("%d ", array[i]);
    }
    // printf("\n");
}

//RING BUFFER UART 1
void write_cb(char c)
{
    if(c == 0x0D){
        ;
    }else if(c == 0x0A){     
        int token_count = 0;           
        sprintf(charToString_1,"%s", charToPrintf_1);
        // printf("%s",charToString_1);
        memset(charToPrintf_1, '\0', sizeof(charToPrintf_1));
        charToPrintf_count_1 = 0;
        
        char * token = strtok(charToString_1, ","); // https://www.educative.io/answers/splitting-a-string-using-strtok-in-c
        while( token != NULL ) {
            // if(token_count > 0){
            // }            
            sprintf(data_UART1[token_count], "%s", token);
            // printf( "token %d : %s\n",token_count, token ); //printing each token
            // printf("%s", token);
            token = strtok(NULL, ",");
            token_count++;
        }
        for(int i=0;i<token_count;i++){
            toInt1[i] = atoi(data_UART1[i]);
            // printf("uart1[%d] = %d\n",i, toInt1[i]);
        }
        dataInclinometer = toInt1[0];
        // printf("======\n");

    }else{
        charToPrintf_1[charToPrintf_count_1] = c;
        charToPrintf_count_1++;
    }

    uart_buf[write_index] = c;    
    write_index = (write_index + 1) % BUF_SIZE;
    uart_count++;    
}

char read_cb(void)
{
    char c = uart_buf[read_index];
    read_index = (read_index + 1) % BUF_SIZE;
    uart_count--;
    return c;
}

void UART1_ISR()
{
    uint8_t RX_i = 0;
    while (uart_is_readable(uart1))
    {
        if(setTimeFlag == 0){
            uart_read_blocking(uart1, (uint8_t *) &toPrint, 15);
            setTimeFlag = 1;
        }else{            
            if(modulusCount > 3){
                write_cb(uart_getc(uart1));        
                modulusCount = 0;
            }
            modulusCount++;
        }
    }
}

char *sliceString(char *str, int start, int end)
{

    int i;
    int size = (end - start) + 1;
    char *output = (char *)malloc(size * sizeof(char));

    for (i = 0; start <= end; start++, i++)
    {
        output[i] = str[start];
    }

    output[size] = '\0';

    return output;
}

void print_array(char *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        // printf("%d ", array[i]);
    }
    // printf("\n");
}



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
        vTaskDelay(20);
        gpio_put(LED_PIN, 0);
        vTaskDelay(20);
    }
}

void ltc2449_task()
{   
    float adc_voltage;
    float lsb = 1000;
    int32_t offset_code = 536906755;
    spi_init(SPI_PORT, 500 * 1000);
    gpio_set_function(MISO, GPIO_FUNC_SPI);
    gpio_set_function(SCLK, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);

    gpio_init(CS);
    gpio_set_dir(CS, GPIO_OUT);
    gpio_put(CS, 1);
    
    while (1) {
        uint16_t miso_timeout = 1000;
        //lOAD CELL CHANNEL 1
        // uint16_t adc_command = LTC2449_P0_N1 | LTC2449_OSR_32768; 
        int32_t adc_command = LTC2449_P0_N1 | LTC2449_OSR_32768;
        int32_t adc_command_value;
        
        if(LTC2445_EOC_timeout(miso_timeout)){
            printf("Timeout");
        } else{
            printf("CHANNEL 1 :%d\n", LTC2445_read(adc_command));
            adc_command_value = LTC2445_read(adc_command);
        }

         //lOAD CELL CHANNEL 2
        // uint16_t adc_command1 = LTC2449_P2_N3  | LTC2449_OSR_32768;   
        //int32_t adc_command1 = LTC2449_P2_N3 | LTC2449_OSR_32768;     
        // if(LTC2445_EOC_timeout(miso_timeout)){
        //     printf("Timeout");
        // } else{
        //     printf("CHANNEL 2 :%d\n", LTC2445_read(adc_command1));
        // }

        //  //lOAD CELL CHANNEL 3
        // uint16_t adc_command2 = LTC2449_P4_N5  | LTC2449_OSR_32768;    
        // int32_t adc_command2 = LTC2449_P4_N5  | LTC2449_OSR_32768;    
        // if(LTC2445_EOC_timeout(miso_timeout)){
        //     printf("Timeout");
        // } else{
        //     printf("CHANNEL 3 :%d\n", LTC2445_read(adc_command2));
        // }
        
        //  //lOAD CELL CHANNEL 4
        // uint16_t adc_command3 = LTC2449_P6_N7 | LTC2449_OSR_32768; 
        // int32_t adc_command3 = LTC2449_P4_N5  | LTC2449_OSR_32768;               
        // if(LTC2445_EOC_timeout(miso_timeout)){
        //     printf("Timeout");
        // } else{
        //     printf("CHANNEL 4 :%d\n", LTC2445_read(adc_command3));
        // }

        

        // printf("=======================\n");
        
        vTaskDelay(50);
    }
}

void motor_task()
{   
    while (1) {
        // printf("data inject = %d\n", data_br);
        // printf("data inclinometer = %d\n", dataInclinometer);
        if(dataInclinometer < data_br - thresshold){
            motor1_CCW(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        }else if(dataInclinometer > data_br + thresshold){
            motor1_CC(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        }else{
            motor1_OFF(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
            // printf("SUDUT SAMA\n");
        }
        vTaskDelay(10);

        // motor1_CC(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        // vTaskDelay(200);
        // motor1_OFF(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        // vTaskDelay(100);
        // motor1_CCW(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        // vTaskDelay(200);
        // motor1_OFF(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        // vTaskDelay(100);
        // motor2_CC(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        // vTaskDelay(200);
        // motor1_OFF(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        // vTaskDelay(100);
        // motor2_CCW(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        // vTaskDelay(200);
        // motor1_OFF(PWM_A1_pin,ENA_A1_pin,PWM_B1_pin,ENA_B1_pin,PWM_C1_pin,ENA_C1_pin);
        // vTaskDelay(100);
        
        // motor1_B_CC(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin,ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        // vTaskDelay(200);
        // motor2_OFF(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin, ENA_B2_pin, PWM_C2_pin,ENA_C2_pin);
        // vTaskDelay(100);
        // motor1_B_CCW(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin,ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        // vTaskDelay(200);
        // motor2_OFF(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin, ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        // vTaskDelay(100);
        // motor2_B_CC(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin,ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        // vTaskDelay(200);
        // motor2_OFF(PWM_A2_pin, ENA_A2_pin,PWM_B2_pin,ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        // vTaskDelay(100);
        // motor2_B_CCW(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin,ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        // vTaskDelay(200);
        // motor2_OFF(PWM_A2_pin,ENA_A2_pin,PWM_B2_pin, ENA_B2_pin,PWM_C2_pin,ENA_C2_pin);
        // vTaskDelay(100);
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
    // pwm_set_enabled(slice, true);
    // vTaskDelay(50);
    while (1) {
         pwm_set_enabled(slice, true);
         vTaskDelay(50);
         pwm_set_enabled(slice, false);
         vTaskDelay(50);
         pwm_set_enabled(slice, true);
         vTaskDelay(50);
         pwm_set_enabled(slice, false);
         vTaskDelay(50);
    }
}

void uart0_task()
    {
        // UART0 RX IRQ
        uart_init(uart0, 115200);                        // Initialize UART0
        gpio_set_function(0, GPIO_FUNC_UART);           // Pin Functions
        gpio_set_function(1, GPIO_FUNC_UART);           // Pin Functions
        uart_set_hw_flow(uart0, false, false);           // Default UART
        uart_set_format(uart0, 8, 1, UART_PARITY_NONE);  // Default UART
        uart_set_fifo_enabled(uart0, false);             // Turn off FIFO's - we want to do this character by character   
        uart_set_irq_enables(uart0, true, false);
        irq_set_exclusive_handler(UART0_IRQ, UART0_ISR); // And set up and enable the interrupt handlers
        irq_set_enabled(UART0_IRQ, true);                // Enable Interrupt
        uart_set_irq_enables(uart0, true, false);        // Now enable the UART to send interrupts - RX only
        
        while (1) {
            // printf("%s", charToString );            
            
            // char * token = strtok(charToString, ","); // https://www.educative.io/answers/splitting-a-string-using-strtok-in-c
            // while( token != NULL ) {
            //     // printf( "%s\n", token ); //printing each token
            //     // printf("%s", token[0]);
            //     token = strtok(NULL, ",");               
            // }
        
            // char rx_0;
            // if( uart_count_0 > 0){
            //     rx_0 = read_cb_0();

            //     if(rx_0 == 0x0A){
            //         printf("\n");
            //     }else{
            //         printf("%c", rx_0);
            //     }
            // }
        vTaskDelay(10);
        }
}


void uart1_task()
    {  
        static int chars_rxed = 0;
        uart_init(uart1, 115200);                        // Initialize UART0
        gpio_set_function(4, GPIO_FUNC_UART);           // Pin Functions
        gpio_set_function(5, GPIO_FUNC_UART);           // Pin Functions
        uart_set_hw_flow(uart1, false, false);           // Default UART
        uart_set_format(uart1, 8, 1, UART_PARITY_NONE);  // Default UART
        uart_set_fifo_enabled(uart1, false);             // Turn off FIFO's - we want to do this character by character   
        uart_set_irq_enables(uart1, true, false);
        irq_set_exclusive_handler(UART1_IRQ, UART1_ISR); // And set up and enable the interrupt handlers
        irq_set_enabled(UART1_IRQ, true);                // Enable Interrupt
        uart_set_irq_enables(uart1, true, false);        // Now enable the UART to send interrupts - RX only
    
        while (1) {
            // printf("%s\n", charToString_1);
            // char * token = strtok(charToString_1, ",");
            // while( token != NULL ) {
            //   //  printf( " %s\n", token ); //printing each token
            //     token = strtok(NULL, ",");
        // }
            // char rx;
            // if( uart_count > 0){
            //     rx = read_cb();
            //     if(rx == 0x0A){
            //         printf("\n");
            //     }else{
            //         printf("%c", rx);
            //     }
            // }
            vTaskDelay(2);
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
    xTaskCreate(uart0_task,"Uart0",256, NULL, 1, NULL);
    xTaskCreate(uart1_task,"Uart",256, NULL, 1, NULL);

    vTaskStartScheduler();
    
    while(1){};
    
}
