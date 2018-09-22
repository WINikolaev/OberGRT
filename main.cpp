#include "stm8s.h"
#include "AllThings.h"

/// Extern value
/// From ADC
extern _str_ADC_value ADC_value;
/// From stm8s_it
extern uint8_t cntr_pump_period;
extern uint8_t cntr_start;
extern uint8_t cntr_LED;
extern uint8_t cntr_Check_CRASH_T10;
extern uint8_t CRASH_SYSTEM;
/// This`s for account period (T = 0.5s) so _1s = 2*0.5
typedef enum {_1s = 2, _2s = 4, _10s = 20}_e_period;

typedef enum {Check_fuel, START_Off, START_On_and_Check, START_OK, CRASH}_e_State_machine;
_e_State_machine State_machine = Check_fuel;

/// Buttom check drz
typedef enum {First_sample, Second_sample}_e_Buttom_drz;
// For checking buttom the START
static bool b_START = false;
static bool b_Variable_Written = false;
static bool b_State_system = false;
// For state oil
static bool b_OilTemperatureOK = false;

void _Check_START(void);
// For activate the pump
void _CheckFuel(void);
// For activate the TEN
void _Check_temperatureOil(void);

/// LED State
void _LED_state(void);


void main()
{  
  setup_Peripheral();

  enableInterrupts();
  while(1)
  {
    IWDG_ReloadCounter();
    ADC_Updating();
    
    _CheckFuel();
    _Check_START();
    _LED_state();
    inputReade();
    switch(State_machine)
    {
      case Check_fuel :            
        _Check_temperatureOil();
      break;
      case START_Off :            
        _LED_Off();
        _AIR_Off();
        _FIRE_Off();
        _TEN_Off();
         b_State_system = false;
         cntr_Check_CRASH_T10 = _10s;
      break;
      
      case START_On_and_Check :            
          _AIR_On();
          _FIRE_On();
          if(get_inputLight()){
            State_machine = START_OK;
            b_State_system = true;
          }else{
            if(!cntr_Check_CRASH_T10){State_machine = CRASH;}
          }
      break;
      
      case START_OK :
        _AIR_On();
        _FIRE_Off();
        _LED_On();
        if(!get_inputLight()){
          State_machine = START_On_and_Check;
          cntr_Check_CRASH_T10 = _10s;
          b_State_system = false;
        }   
      break;
      
      case CRASH :
        _LED_Off();
        _Pump_Off();
        _AIR_Off();
        _FIRE_Off();
        _TEN_Off();
        
        TIM2_DeInit();
        TIM2_TimeBaseInit(TIM2_PRESCALER_4096, 100);
        TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
         TIM2_Cmd(ENABLE);
        
        CRASH_SYSTEM = 1;
        while(1){
          IWDG_ReloadCounter();
          if(!CRASH_SYSTEM){
            if(GPIO_ReadInputPin(GPIOB, GPIO_PIN_5)){
              CRASH_SYSTEM = 1;
              _LED_On();
            }else{
              CRASH_SYSTEM = 1;
              _LED_Off();
            }
          }
        }
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
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void _LED_state(void)
{
  if(cntr_LED){return;}
  
  if(b_START && !b_State_system){
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
  
  if(!(ADC_value.A3 > ADC_value.A4)){
    _TEN_On(); 
    b_OilTemperatureOK = false;
  }else{
    _TEN_Off();
    b_OilTemperatureOK = true;   
  }
}

/// For period T = 1s and T = 2s;
_e_period _Pump_Period = _1s;
void _CheckFuel(void)
{    
  if(!ADC_value.data_ready){return;}
  
  /// 25% ot ADC_value.A5
  uint16_t _25pA5 = ADC_value.A5/4;
  ADC_value.A5 -=_25pA5;
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
      //if(_IsThereStart()){
      if(get_inputStart()){
        cntr_start = 1;
        Buttom_drz = Second_sample;
        //if(b_OilTemperatureOK){State_machine = START_On_and_Check;}
      }
      break;
    case Second_sample:
      if(cntr_start == 0){
        Buttom_drz = First_sample;
        //if(_IsThereStart()){
        if(get_inputStart()){
          //_LED_On();
          b_START = true;
          if(b_OilTemperatureOK && !b_State_system){
            State_machine = START_On_and_Check;
            if(!b_Variable_Written){
              cntr_Check_CRASH_T10 = _10s;
              b_Variable_Written = true;
            }
            
          }
        }else{
          //_LED_Off();
          b_START = false;
          if(b_OilTemperatureOK){
            State_machine = START_Off;
          }
        }
      }
      break;
    default:break;
  }
}