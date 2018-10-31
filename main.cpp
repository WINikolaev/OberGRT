#include "stm8s.h"
#include "AllThings.h"

#define ONE_MINUTE 1
/// Extern value
/// From ADC 
extern _str_ADC_value ADC_value;
/// From stm8s_it
extern uint16_t cntr_Wait_Oil;
extern uint8_t cntr_pump_period;
extern uint8_t cntr_start;
extern uint8_t cntr_LED;
extern uint8_t cntr_Check_CRASH_T10;
extern uint8_t CRASH_SYSTEM;

_e_State_machine State_machine = WAIT;


// For checking buttom the START
static bool b_START = false;
static bool b_Variable_Written = false;
static bool b_State_system = false;
// For state oil



bool _Check_START(void);

/// LED State
void _LED_state(void);


void main()
{  
  /// Готовность определяется температурой масла, если греть ненадо то в принципе готовы
  bool b_Ready = false;
  /// Берем отсчет для контроля огня в горелке
  _e_Buttom_drz eCheckFire = setup;
  /// Переменные 
  _str_ADC_value sADC;
  /// Инициализируем все переферию
  setup_Peripheral();
  /// Вкл прерывания
  enableInterrupts();
  while(1)
  {
    IWDG_ReloadCounter();
    /// Чекаем состояния входных сигналов
    inputReade();
    /// Проверяем уровень масла
    checkFuel_Level();
    // Чекаем наши АЦПшки
    ADC_Updating(&sADC);
    /// Проверяем температуру масла
    b_Ready = checkTemperatureOil(&sADC);
  
    
    
    _LED_state();
    switch(State_machine)
    {
      case WAIT :{
        if(!b_Ready){break;}
        All_OFF();
        if(_Check_START()){
          State_machine = START_WORK;
          eCheckFire = setup;
        }  
      break;
      }
      
      case START_WORK :{
        if(!_Check_START()){
          State_machine = WAIT;
          break;
        }
        /// TODO - сдлеай отдельную функцию....если конечно не будет лень
        switch (eCheckFire){
          case First_sample:{
            if(CRASH_SYSTEM){break;}
            /// Если дождались 4 сек выключаем искру и N-раз вызывает цикл проверки фотодатчика делаем проверку фотодатчика
            _FIRE_Off();
            for(char i = 0x00; i<25; i++){inputReade();};
            if(get_inputLight()){
              State_machine = WORKING;
              eCheckFire = setup;
              break;
            }
            eCheckFire = Second_sample;
            _FIRE_On();
            CRASH_SYSTEM = 8;
          break;
          }
          
          case Second_sample:{
            if(CRASH_SYSTEM){break;}
            _FIRE_Off();
            for(char i = 0x00; i<25; i++){inputReade();};
            if(get_inputLight()){
              State_machine = WORKING;
              eCheckFire = setup;
              break;
            }
            eCheckFire = setup;
            State_machine = CRASH;
          break;
          }
          case setup:{
            _AIR_On();
            _FIRE_On();
            /// Ждем первые 4 секунды
            CRASH_SYSTEM = 8;
            eCheckFire = First_sample;
          break;
          }
          
          default:{
            eCheckFire = setup;
          break;
          }
        }
        
      break;
      }
      
      case WORKING :{
        if(!_Check_START()){State_machine = WAIT;}
        if(!get_inputLight()){State_machine = START_WORK;}
        _FIRE_Off();
      break;
      }
      
      case CRASH :{
        TIM2_DeInit();
        TIM2_TimeBaseInit(TIM2_PRESCALER_4096, 50);
        TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
        TIM2_Cmd(ENABLE);
        while(1){
          IWDG_ReloadCounter();
          _AIR_Off();
          _FIRE_Off();
          _TEN_Off();
          _Pump_Off();
          _LED_state();
        }
      break;
      }
      
      default:{
        
      break;
      }
    }
    /*
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
*/
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
  
  switch(State_machine){
    case WAIT:{
      cntr_LED = 2;
      if(_LED_State()){_LED_On();}else{_LED_Off();}
    break;
    }
  
    case START_WORK:{
      cntr_LED = 2;
      if(_LED_State()){_LED_On();}else{_LED_Off();}
    break;
    }
  
    case WORKING:{
      _LED_On();
    break;
    }
  
    case CRASH:{
      cntr_LED = 1;
      if(_LED_State()){_LED_On();}else{_LED_Off();}
    break;
    }
  
    default:break;
  }
}


_e_Buttom_drz Buttom_drz = First_sample;
bool State_Start = false;
bool _Check_START(void)
{
  switch (Buttom_drz){
    case First_sample:{
      if(get_inputStart()){
        cntr_start = 1;
        Buttom_drz = Second_sample;
      }
    break;
    }
    case Second_sample:{
      if(cntr_start == 0){
        Buttom_drz = First_sample;
        if(get_inputStart()){State_Start =  true;}else{State_Start = false;}
      }
      break;
    }
    default:
      Buttom_drz = First_sample;
      State_Start = false;
      break;
  }
  return State_Start;
}