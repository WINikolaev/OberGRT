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




_e_count_input cnt_input;
u_input_read input_start;
u_input_read input_light;
uint8_t result_start = 0x00;
uint8_t result_light = 0x00;
void inputReade(void)
{
  switch(cnt_input){
    case _1:
      input_start.b.b0  = GPIO_ReadInputPin(GPIOB, GPIO_PIN_4)? 1 : 0;
      input_light.b.b0  = GPIO_ReadInputPin(GPIOA, GPIO_PIN_3)? 1 : 0;
      cnt_input = _2;
    break;
  
    case _2:
      input_start.b.b1  = GPIO_ReadInputPin(GPIOB, GPIO_PIN_4)? 1 : 0;
      input_light.b.b1  = GPIO_ReadInputPin(GPIOA, GPIO_PIN_3)? 1 : 0;
      cnt_input = _3;
    break;
  
    case _3:
      input_start.b.b2  = GPIO_ReadInputPin(GPIOB, GPIO_PIN_4)? 1 : 0;
      input_light.b.b2  = GPIO_ReadInputPin(GPIOA, GPIO_PIN_3)? 1 : 0;
      cnt_input = _4;
    break;
  
    case _4:
      input_start.b.b3  = GPIO_ReadInputPin(GPIOB, GPIO_PIN_4)? 1 : 0;
      input_light.b.b3  = GPIO_ReadInputPin(GPIOA, GPIO_PIN_3)? 1 : 0;
      cnt_input = _5;
    break;
  
    case _5:
      input_start.b.b4  = GPIO_ReadInputPin(GPIOB, GPIO_PIN_4)? 1 : 0;
      input_light.b.b4  = GPIO_ReadInputPin(GPIOA, GPIO_PIN_3)? 1 : 0;
      cnt_input = _6;
    break;
    
    case _6:
       input_start.b.b5  = GPIO_ReadInputPin(GPIOB, GPIO_PIN_4)? 1 : 0;
       input_light.b.b5  = GPIO_ReadInputPin(GPIOA, GPIO_PIN_3)? 1 : 0;
       cnt_input = _7;
    break;
    
    case _7:
       input_start.b.b6  = GPIO_ReadInputPin(GPIOB, GPIO_PIN_4)? 1 : 0;
       input_light.b.b6  = GPIO_ReadInputPin(GPIOA, GPIO_PIN_3)? 1 : 0;
       cnt_input = _8;
    break;
    
    case _8:
       input_start.b.b7  = GPIO_ReadInputPin(GPIOB, GPIO_PIN_4)? 1 : 0;
       input_light.b.b7  = GPIO_ReadInputPin(GPIOA, GPIO_PIN_3)? 1 : 0;
       
       result_start = input_start.b.b0&input_start.b.b2&input_start.b.b3&input_start.b.b4&input_start.b.b5&input_start.b.b6&input_start.b.b7;
       result_light = input_light.b.b0&input_light.b.b2&input_light.b.b3&input_light.b.b4&input_light.b.b5&input_light.b.b6&input_light.b.b7;
       
       cnt_input = _1;
    break;
    
    default:
    break;
  }
}