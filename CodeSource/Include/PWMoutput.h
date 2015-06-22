#ifndef PWM_OUT_H
#define PWM_OUT_H

#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "system_stm32f4xx.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"

typedef struct st_PWMout{

	TIM_OC_InitTypeDef *PWMConfig;
	TIM_HandleTypeDef  *TimHandle;	
	GPIO_InitTypeDef   *GPIO_InitStructure;
	//GPIO_TypeDef	   *GPIO_Type; 		
}*PWMout_t;



/*void Init_System(void);*/
/*void SystemClock_Config(void);*/

void initalise_HAL();

PWMout_t create_PWMout(char type, uint32_t pin);
void destroy_PWMout(char type, PWMout_t pwmOut);

void set_prescaler (PWMout_t my_pwm ,int value);  /*** Can be used to set the prescaler */

/** Modifying the pulse with **/
void pulse_width_s (PWMout_t my_pwm , int seconds);  
void pulse_width_ms (PWMout_t my_pwm , int milliseconds);
void pulse_width_us (PWMout_t my_pwm , int microseconds);

/** The next three functions is for setting the period.We can do this by 
    either passing ,seconds, microseconds or milliseconds as the second parameter. */

void period_s (PWMout_t my_pwm , int seconds);
void period_ms(PWMout_t my_pwm , int milliseconds);
void period_us (PWMout_t my_pwm , int microseconds);

void write (PWMout_t my_pwm ,  double duty);  // Write duty cycle
double read(PWMout_t my_pwm);   // Read signal's  duty cycle 
#endif

