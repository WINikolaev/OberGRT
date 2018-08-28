#include "GPIO.h"


void _GPIO_setup(void)
{
  // OUTPUT
  
  // Init Led
  GPIO_Init(GPIOB, GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
  
  // Init Pump
  GPIO_Init(GPIOC, GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_WriteLow(GPIOC, GPIO_PIN_5);
  
  // Init AIR
  GPIO_Init(GPIOC, GPIO_PIN_7,GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_WriteLow(GPIOC, GPIO_PIN_7);
  
  // Init TEN
  GPIO_Init(GPIOC, GPIO_PIN_4,GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_WriteLow(GPIOC, GPIO_PIN_4);
  
  // Init Fire
  GPIO_Init(GPIOC, GPIO_PIN_6,GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_WriteLow(GPIOC, GPIO_PIN_6);
  
  
  // INPUT
  // There is light
  GPIO_Init(GPIOA, GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);
  // There is START
  GPIO_Init(GPIOB, GPIO_PIN_4,GPIO_MODE_IN_PU_NO_IT);
}

// INPUT
BitStatus _IsThereLight(void){ return GPIO_ReadInputPin(GPIOA, GPIO_PIN_3);}
BitStatus _IsThereStart(void){ return GPIO_ReadInputPin(GPIOB, GPIO_PIN_4);}

// OUTPUT

void _FIRE_On(void){GPIO_WriteHigh(GPIOC, GPIO_PIN_6);}
void _FIRE_Off(void){GPIO_WriteLow(GPIOC, GPIO_PIN_6);}

void _TEN_On(void){GPIO_WriteHigh(GPIOC, GPIO_PIN_4);}
void _TEN_Off(void){GPIO_WriteLow(GPIOC, GPIO_PIN_4);}

void _AIR_On(void){GPIO_WriteHigh(GPIOC, GPIO_PIN_7);}
void _AIR_Off(void){GPIO_WriteLow(GPIOC, GPIO_PIN_7);}

void _Pump_On(void){GPIO_WriteHigh(GPIOC, GPIO_PIN_5);}
void _Pump_Off(void){GPIO_WriteLow(GPIOC, GPIO_PIN_5);}


void _LED_On(void){GPIO_WriteLow(GPIOB, GPIO_PIN_5);}
void _LED_Off(void){GPIO_WriteHigh(GPIOB, GPIO_PIN_5);}