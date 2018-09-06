#include "stm8s.h"
#include "GPIO.h"
#include "ADC.h"

/// Extern value
/// From ADC
extern _str_ADC_value ADC_value;
/// From stm8s_it
extern uint8_t cntr_pump_period;
extern uint8_t cntr_start;
extern uint8_t cntr_LED;
extern uint8_t cntr_CRASH_T10;
/// This`s for account period (T = 0.5s) so _1s = 2*0.5
typedef enum {_1s = 2, _2s = 4, _10s = 20}_e_period;

typedef enum {Check_fuel, START, TODO, CRASH}_e_State_machine;
_e_State_machine State_machine = Check_fuel;

/// Buttom check drz
typedef enum {First_sample, Second_sample}_e_Buttom_drz;
// For checking buttom the START
static bool b_START = false;
void _Check_START(void);
// For activate the pump
void _CheckFuel(void);
// For activate the TEN
void _Check_temperatureOil(void);

/// LED State
void _LED_state(void);

// For configure the perefery
void _Clock_setup(void);
void _TIM2_Setup(void);
void _IWDG_setup(void);


void main()
{  
  _Clock_setup();
  _GPIO_setup();
  _ADC_setup();
  _TIM2_Setup();
  _IWDG_setup();
  ADC_value.start_stop = false;
  
  enableInterrupts();
  while(1)
  {
    IWDG_ReloadCounter();
    ADC_update();
    
    _CheckFuel();
    _Check_START();
    _LED_state();
    switch(State_machine)
    {
      case Check_fuel :            
        _Check_temperatureOil();
      break;
      
      case START :            
        
      break;
      
      case CRASH :
        
      break;
    
    default:
    break;
    }
   }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  while(1){}
}
#endif
///////////////////////////////////////////////////////////////////////////////

void _LED_state(void)
{
  if(cntr_LED){return;}
  
  if(b_START){
    if(GPIO_ReadInputPin(GPIOB, GPIO_PIN_5)){
      cntr_LED = _1s;
      _LED_On();
    }else{
      cntr_LED = _1s;
      _LED_Off();
    }
  }else{_LED_Off();}
}



void _Check_temperatureOil(void)
{
  if(!ADC_value.data_ready){return;}
  
  if(!(ADC_value.A3 < ADC_value.A4)){
    _TEN_On(); 
  }else{
    _TEN_Off(); 
    if(b_START){
      //cntr_CRASH_T10 = _10s;
      //State_machine = START;
    }    
  }
}

/// For period T = 1s and T = 2s;
_e_period _Pump_Period = _1s;
void _CheckFuel(void)
{    
  if(!ADC_value.data_ready){return;}
  
  if(!(ADC_value.A5 < ADC_value.A6)){
    _Pump_Off(); 
    return;
  }
  
  if(cntr_pump_period){return;}
  
  switch(_Pump_Period)
  {
    case _1s :
      cntr_pump_period = _1s;
      _Pump_On();  
      _Pump_Period = _2s;
    break;
      
    case _2s :            
      cntr_pump_period = _2s;
      _Pump_Off();  
      _Pump_Period = _1s;
    break;
    default:break;
  }

}
_e_Buttom_drz Buttom_drz = First_sample;
void _Check_START(void)
{
  switch (Buttom_drz){
    case First_sample:
      if(_IsThereStart()){
        cntr_start = 1;
        Buttom_drz = Second_sample;
      }
      break;
    case Second_sample:
      if(cntr_start == 0){
        Buttom_drz = First_sample;
        if(_IsThereStart()){
          //_LED_On();
          b_START = true;
        }else{
          //_LED_Off();
          b_START = false;
        }
      }
      break;
    default:break;
  }
}

void _Clock_setup(void)
{
       CLK_DeInit();
       CLK_HSECmd(DISABLE);
       CLK_LSICmd(DISABLE);
       CLK_HSICmd(ENABLE);
       //CLK_CCOCmd(ENABLE);
       //CLK_CCOConfig(CLK_OUTPUT_MASTER);
       while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);
       CLK_ClockSwitchCmd(ENABLE);
       CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);     
       CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, 
                             CLK_SOURCE_HSI,
                             DISABLE,
                             CLK_CURRENTCLOCKSTATE_ENABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);

}


void _IWDG_setup(void)
{
/// IWDG 
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(0xFF);
  IWDG_Enable();
}

void _TIM2_Setup(void)
{ 
  ///TIM2
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_4096, 244);
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  TIM2_Cmd(ENABLE);
}
