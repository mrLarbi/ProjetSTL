#include "PWMoutput.h"

void reset_pwm(PWMout_t pwm);
void reset_handler(PWMout_t pwm);

void initalise_HAL() {
	HAL_Init();
}

void Error_Handler(void);

GPIO_TypeDef* PWM_OutputPort (char type) {
	switch(type) {
		case 'A': 
			return GPIOA;
		case 'B':
			return GPIOB;
		case 'C': 
			return GPIOC;
		case 'D':
			return GPIOD;
		case 'E':
			return GPIOE;		
		case 'H': 
			return GPIOH;
		default:
			return NULL;		
	}
}


void PWM_initaliseOutputPort(GPIO_TypeDef* GPIOx) {		
	    if(GPIOx==GPIOA){
 			__GPIOA_CLK_ENABLE();	
			return;
		}
		else if(GPIOx==GPIOB){
 			__GPIOB_CLK_ENABLE();	
			return;
		} else if(GPIOx==GPIOC){
 			__GPIOC_CLK_ENABLE();	
			return;
		} else if(GPIOx==GPIOD){
 			__GPIOD_CLK_ENABLE();	
			return;
		}else if(GPIOx==GPIOE){
 			__GPIOE_CLK_ENABLE();	
			return;
		} else {
			__GPIOH_CLK_ENABLE();
		}  
}

void PWM_DeInitOutputPort(GPIO_TypeDef* GPIOx) {		
	    if(GPIOx==GPIOA){
 			__GPIOA_CLK_DISABLE();	
			return;
		}
		else if(GPIOx==GPIOB){
 			__GPIOB_CLK_DISABLE();	
			return;
		} else if(GPIOx==GPIOC){
 			__GPIOC_CLK_DISABLE();	
			return;
		} else if(GPIOx==GPIOD){
 			__GPIOD_CLK_DISABLE();	
			return;
		}else if(GPIOx==GPIOE){
 			__GPIOE_CLK_DISABLE();	
			return;
		} else {
			__GPIOH_CLK_DISABLE();
		}
}

uint16_t PWM_selectPin(uint32_t PIN) {
	if(IS_GPIO_PIN(PIN)) {
		return (uint16_t)PIN;
	} else {
		return GPIO_PIN_All;
	}
}
void PWM_InitHandler(PWMout_t pwm) {
    pwm->TimHandle = (TIM_HandleTypeDef*)malloc(sizeof(TIM_HandleTypeDef));
    pwm->TimHandle->Instance = TIM2;
    pwm->TimHandle->Init.Period = 0;
    pwm->TimHandle->Init.Prescaler = 0;
    pwm->TimHandle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    pwm->TimHandle->Init.RepetitionCounter = 0;
    pwm->TimHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
    reset_handler(pwm);
}

void PWM_InitGPIO(PWMout_t pwm,uint32_t pin) {
	pwm->GPIO_InitStructure = malloc(sizeof(GPIO_InitTypeDef));
    pwm->GPIO_InitStructure->Pin = pin ;
    pwm->GPIO_InitStructure->Mode = GPIO_MODE_AF_PP;    
    pwm->GPIO_InitStructure->Pull = GPIO_PULLUP;
    pwm->GPIO_InitStructure->Speed = GPIO_SPEED_HIGH;
    pwm->GPIO_InitStructure->Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, pwm->GPIO_InitStructure);
}

void PWM_Config(PWMout_t pwm) { 
	pwm->PWMConfig = (TIM_OC_InitTypeDef*)malloc(sizeof(TIM_OC_InitTypeDef));
    pwm->PWMConfig->Pulse = 0;
    pwm->PWMConfig->OCMode = TIM_OCMODE_PWM1;
    pwm->PWMConfig->OCPolarity = TIM_OCPOLARITY_HIGH;
    pwm->PWMConfig->OCNIdleState = TIM_OUTPUTSTATE_ENABLE;
}

PWMout_t create_PWMout(char type, uint32_t pin) {
	PWMout_t pwm = (PWMout_t)malloc(sizeof(struct st_PWMout));
	//pwm->GPIO_Type = *PWM_OutputPort(type);
	GPIO_TypeDef* GPIO_Type = PWM_OutputPort(type);
	PWM_initaliseOutputPort(GPIO_Type);
	
	pwm->GPIO_InitStructure = malloc(sizeof(GPIO_InitTypeDef));
    pwm->GPIO_InitStructure->Pin = PWM_selectPin(pin);
    pwm->GPIO_InitStructure->Mode = GPIO_MODE_AF_PP;    
    pwm->GPIO_InitStructure->Pull = GPIO_PULLUP;
    pwm->GPIO_InitStructure->Speed = GPIO_SPEED_HIGH;
    pwm->GPIO_InitStructure->Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIO_Type, pwm->GPIO_InitStructure);
    
	__TIM2_CLK_ENABLE();
    
    pwm->TimHandle = (TIM_HandleTypeDef*)malloc(sizeof(TIM_HandleTypeDef));
    
    pwm->TimHandle->Instance = TIM2;
    pwm->TimHandle->Init.Period = 0;
    pwm->TimHandle->Init.Prescaler = 0;
    pwm->TimHandle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    pwm->TimHandle->Init.RepetitionCounter = 0;
    pwm->TimHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
    
    reset_handler(pwm);
    
    pwm->PWMConfig = (TIM_OC_InitTypeDef*)malloc(sizeof(TIM_OC_InitTypeDef));
    pwm->PWMConfig->Pulse = 0;
    pwm->PWMConfig->OCMode = TIM_OCMODE_PWM1;
    pwm->PWMConfig->OCPolarity = TIM_OCPOLARITY_HIGH;
    pwm->PWMConfig->OCNIdleState = TIM_OUTPUTSTATE_ENABLE;
	
    //reset_pwm(pwm);
    return pwm;
}

	
void destroy_PWMout(char type,PWMout_t pwmOut){
	HAL_GPIO_DeInit(LED2_GPIO_PORT, pwmOut->GPIO_InitStructure->Pin);
	HAL_TIM_PWM_DeInit(pwmOut->TimHandle);
	//GPIO_TypeDef* GPIO_Type = PWM_OutputPort(type);
	//PWM_DeInitOutputPort(GPIO_Type);
	free(pwmOut);
	}
	
void set_prescaler (PWMout_t pwm ,int value){
	pwm->TimHandle->Init.Prescaler = value;
	reset_handler(pwm);
	}	
	
void period_s (PWMout_t pwm , int seconds){
	pwm->TimHandle->Init.Period = seconds;
	reset_handler(pwm);
}
	
// duty in terms of percentage
void write (PWMout_t pwm ,  double duty) {
	if(pwm->TimHandle->Init.Period != 0) {
		pwm->PWMConfig->Pulse = (int)(duty*pwm->TimHandle->Init.Period/100);
		reset_pwm(pwm);
		}
		
	}
		
void reset_pwm(PWMout_t pwm) {
    if(HAL_TIM_PWM_ConfigChannel(pwm->TimHandle,pwm->PWMConfig,TIM_CHANNEL_1) != HAL_OK)
    {
      Error_Handler();
		
		}
     
    if(HAL_TIM_PWM_Start(pwm->TimHandle, TIM_CHANNEL_1) != HAL_OK)
    {
      Error_Handler();
		
		}
}	
	
void reset_handler(PWMout_t pwm) {
	if(HAL_TIM_PWM_Init(pwm->TimHandle) != HAL_OK)
    {
      Error_Handler();
		}
}		
	
double read(PWMout_t pwm){
	if(pwm->TimHandle->Init.Period != 0) {
		return pwm->PWMConfig->Pulse/pwm->TimHandle->Init.Period;
		}
	return -1.0;	
	}    

void pulse_width_s (PWMout_t pwm , int seconds){
	pwm->PWMConfig->Pulse= seconds;
	reset_pwm(pwm);
	}

void Error_Handler(void)
{
  while(1)
  {
  }
}
		
