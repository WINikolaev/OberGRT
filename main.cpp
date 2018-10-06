#include "stm8s.h"
#include "AllThings.h"

/// Extern value
/// From ADC
/// Коммент тес
/// как же быть 
 
extern _str_ADC_value ADC_value;
/// From stm8s_it
extern uint8_t cntr_pump_period;
extern uint8_t cntr_start;
extern uint8_t cntr_LED;
extern uint8_t cntr_Check_CRASH_T10;
extern uint8_t CRASH_SYSTEM;

_e_State_machine State_machine = Check_fuel;


// For checking buttom the START
static bool b_START = false;
static bool b_Variable_Written = false;
static bool b_State_system = false;
// For state oil
static bool b_OilTemperatureOK = false;

void _Check_START(void);

/// LED State
void _LED_state(void);


void main()
{  
  /// Переменные 
  _str_ADC_value sADC;
  /// Инициализируем все переферию
  setup_Peripheral();
  /// Вкл прерывания
  enableInterrupts();
  while(1)
  {
    IWDG_ReloadCounter();
    ADC_Updating(&sADC);
    checkFuel_Level(&sADC);
    
    _Check_START();
    _LED_state();
    inputReade();
    switch(State_machine)
    {
      case Check_fuel :            
        checkTemperatureOil(&sADC);
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


_e_Buttom_drz Buttom_drz = First_sample;
void _Check_START(void)
{
  switch (Buttom_drz){
    case First_sample:
      if(get_inputStart()){
        cntr_start = 1;
        Buttom_drz = Second_sample;
      }
      break;
    case Second_sample:
      if(cntr_start == 0){
        Buttom_drz = First_sample;
        if(get_inputStart()){
          b_START = true;
          //if(b_OilTemperatureOK && !b_State_system){
          if(getOilTemperature() && !b_State_system){
            State_machine = START_On_and_Check;
            if(!b_Variable_Written){
              cntr_Check_CRASH_T10 = _10s;
              b_Variable_Written = true;
            }
          }
        }else{
          b_START = false;
          //if(b_OilTemperatureOK){
          if(getOilTemperature()){
            State_machine = START_Off;
          }
        }
      }
      break;
    default:break;
  }
}