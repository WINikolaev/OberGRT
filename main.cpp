#include "stm8s.h"
#include "AllThings.h"


/// From stm8s_it
extern uint16_t cntr_Wait_Oil;
extern uint8_t cntr_pump_period;
extern uint8_t cntr_start;
extern uint8_t cntr_LED;
extern uint8_t cntr_Check_CRASH_T10;
extern uint8_t CRASH_SYSTEM;
extern uint8_t cntr_Wait_Presetting;

e_State_machine machine_State = WAIT;
void led_Function(void);

void main()
{     
  /// Готовность определяется температурой масла, если греть ненадо то в принципе готовы
  bool isPresetting = false;
  /// Берем отсчет для контроля огня в горелке
  _e_Buttom_drz eCheckFire = setup;
  setup_Peripheral();
  
  /// Вкл прерывания
  enableInterrupts();
  /// Ждем для того чтобы считались все IO
  cntr_Wait_Presetting = 2;
  while(cntr_Wait_Presetting){
    IWDG_ReloadCounter();
    read_Input_IO();
    adc_Updating();
  }
  while(1)
  {
    IWDG_ReloadCounter();
    /// Опрашиваем сосятония на входе, только при выключенном осциляторе.
    if(!_IsThereFire()){
      read_Input_IO();
      adc_Updating();
    } 
    
    if(checkFuel_Level()){
      isPresetting = checkTemperatureOil();
    }
    
    led_Function();    
    switch(machine_State){
      case(WAIT):{
        _AIR_Off();
        _FIRE_Off();
        _LED_Off();
        if(isStart() && isPresetting){
          machine_State = START_WORK;
          eCheckFire = setup;
        }
      break;
      }
      case(START_WORK):{
        if(!isStart()){
          machine_State = WAIT;
          break;
        }
        
        switch (eCheckFire){
          case First_sample:{
            if(CRASH_SYSTEM){break;}
            /// Если дождались 4 сек выключаем искру и N-раз вызывает цикл проверки фотодатчика делаем проверку фотодатчика
            _FIRE_Off();
            for(char i = 0x00; i<25; i++){read_Input_IO();};
            if(get_inputLight()){
              machine_State = WORKING;
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
            for(char i = 0x00; i<25; i++){read_Input_IO();};
            if(get_inputLight()){
              machine_State = WORKING;
              eCheckFire = setup;
              break;
            }
            eCheckFire = setup;
            machine_State = CRASH;
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
      case(WORKING):{
        if(!isStart()){machine_State = WAIT;}
        if(!get_inputLight()){machine_State = START_WORK;}
        _FIRE_Off();
      break;
      }
      case(CRASH):{
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
          led_Function();
        }
      break;
      }
      default:{
      
      break;
      }
    }
  }
}

void led_Function()
{
    if(cntr_LED){return;}
    switch(machine_State){
      case(WAIT):{
        _LED_Off();
      break;
      }
      case(START_WORK):{
        cntr_LED = 2;
        if(_LED_State()){_LED_On();}else{_LED_Off();}
      break;
      }
      case(WORKING):{
        _LED_On();
      break;
      }
      case(CRASH):{
        cntr_LED = 1;
        if(_LED_State()){_LED_On();}else{_LED_Off();}
      break;
      }
      default:{    
      break;
      }
    }
}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  while(1){
  
  
  }
}
#endif

