#include "hardware/gpio.h"





void motor1_CC(uint PWM_A1_pin, uint ENA_A1_pin, uint PWM_B1_pin, uint ENA_B1_pin, uint PWM_C1_pin, uint ENA_C1_pin){
        gpio_put(PWM_A1_pin, false);
        gpio_put(ENA_A1_pin, false);
        gpio_put(PWM_B1_pin, true);
        gpio_put(ENA_B1_pin, false);
        gpio_put(PWM_C1_pin, false);
        gpio_put(ENA_C1_pin, true);
      
        
}

void motor1_CCW(uint PWM_A1_pin, uint ENA_A1_pin, uint PWM_B1_pin, uint ENA_B1_pin, uint PWM_C1_pin, uint ENA_C1_pin){
        gpio_put(PWM_A1_pin, false);
        gpio_put(ENA_A1_pin, false);
        gpio_put(PWM_B1_pin, false);
        gpio_put(ENA_B1_pin, true);
        gpio_put(PWM_C1_pin, true);
        gpio_put(ENA_C1_pin, false);
        
}

void motor2_CC(uint PWM_A1_pin, uint ENA_A1_pin, uint PWM_B1_pin,uint ENA_B1_pin,uint PWM_C1_pin,uint ENA_C1_pin){
        gpio_put(PWM_A1_pin, false);
        gpio_put(ENA_A1_pin, true);
        gpio_put(PWM_B1_pin, true);
        gpio_put(ENA_B1_pin, false);
        gpio_put(PWM_C1_pin, true);
        gpio_put(ENA_C1_pin, true);
     
}

void motor2_CCW(uint PWM_A1_pin,uint ENA_A1_pin,uint PWM_B1_pin,uint ENA_B1_pin,uint PWM_C1_pin,uint ENA_C1_pin){
        gpio_put(PWM_A1_pin, true);
        gpio_put(ENA_A1_pin, false);
        gpio_put(PWM_B1_pin, false);
        gpio_put(ENA_B1_pin, true);
        gpio_put(PWM_C1_pin, false);
        gpio_put(ENA_C1_pin, false);
       
}

void motor1_OFF(uint PWM_A1_pin, uint ENA_A1_pin, uint PWM_B1_pin, uint ENA_B1_pin, uint PWM_C1_pin, uint ENA_C1_pin){
        gpio_put(PWM_A1_pin, false);
        gpio_put(ENA_A1_pin, false);
        gpio_put(PWM_B1_pin, false);
        gpio_put(ENA_B1_pin, false);
        gpio_put(PWM_C1_pin, false);
        gpio_put(ENA_C1_pin, false);
       

}

void motor1_B_CC(uint PWM_A2_pin,uint ENA_A2_pin,uint PWM_B2_pin,uint ENA_B2_pin,uint PWM_C2_pin,uint ENA_C2_pin){
        gpio_put(PWM_A2_pin, false);
        gpio_put(ENA_A2_pin, false);
        gpio_put(PWM_B2_pin, true);
        gpio_put(ENA_B2_pin, false);
        gpio_put(PWM_C2_pin, false);
        gpio_put(ENA_C2_pin, true);
        
        
}

void motor1_B_CCW(uint PWM_A2_pin,uint ENA_A2_pin,uint PWM_B2_pin,uint ENA_B2_pin,uint PWM_C2_pin,uint ENA_C2_pin){
        gpio_put(PWM_A2_pin, false);
        gpio_put(ENA_A2_pin, false);
        gpio_put(PWM_B2_pin, false);
        gpio_put(ENA_B2_pin, true);
        gpio_put(PWM_C2_pin, true);
        gpio_put(ENA_C2_pin, false);
       
}

void motor2_B_CC(uint PWM_A2_pin,uint ENA_A2_pin,uint PWM_B2_pin,uint ENA_B2_pin,uint PWM_C2_pin,uint ENA_C2_pin){
        gpio_put(PWM_A2_pin, false);
        gpio_put(ENA_A2_pin, true);
        gpio_put(PWM_B2_pin, true);
        gpio_put(ENA_B2_pin, false);
        gpio_put(PWM_C2_pin, true);
        gpio_put(ENA_C2_pin, true);
        
}

void motor2_B_CCW(uint PWM_A2_pin,uint ENA_A2_pin,uint PWM_B2_pin,uint ENA_B2_pin,uint PWM_C2_pin,uint ENA_C2_pin){
        gpio_put(PWM_A2_pin, true);
        gpio_put(ENA_A2_pin, false);
        gpio_put(PWM_B2_pin, false);
        gpio_put(ENA_B2_pin, true);
        gpio_put(PWM_C2_pin, false);
        gpio_put(ENA_C2_pin, false);
           
}

void motor2_OFF(uint PWM_A2_pin, uint ENA_A2_pin, uint PWM_B2_pin, uint ENA_B2_pin, uint PWM_C2_pin, uint ENA_C2_pin){
        gpio_put(PWM_A2_pin, false);
        gpio_put(ENA_A2_pin, false);
        gpio_put(PWM_B2_pin, false);
        gpio_put(ENA_B2_pin, false);
        gpio_put(PWM_C2_pin, false);
        gpio_put(ENA_C2_pin, false);
       

}
