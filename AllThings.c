#include "AllThings.h"

extern uint8_t cntr_pump_period;
extern uint16_t cntr_Wait_Oil;
extern uint8_t cntr_start;
extern uint8_t cntr_LED;
extern uint8_t cntr_Check_CRASH_T10;
extern uint8_t CRASH_SYSTEM;



void setup_Peripheral(void)
{
  setup_CLK();
  setup_GPIO();
  setup_ADC();
  setup_TIM2();
  setup_IWDG();
}

bool b_Ready = false;

bool checkTemperatureOil(_str_ADC_value* ADC)
{
  if(!ADC->data_ready){return b_Ready;}
  
  if(!(ADC->A3 > ADC->A4)){
    _TEN_On(); 
  }else{
    _TEN_Off();
    b_Ready = true;   
  }
  return b_Ready;
}



/// Проверяем уровень масла
_e_period v_Pump_Period = _1s;
static bool b_Wait2Minute = false;
static bool isFirstLevel = false;
bool checkFuel_Level(void)
{
  /// Если масло есть то обновляем переменные и выходим
  if(!get_inputOilLevel()){
    _Pump_Off(); 
    isFirstLevel = true;
    cntr_Wait_Oil = 0;
    b_Wait2Minute = false;
    cntr_pump_period = _1s;
    return isFirstLevel;
  }
  /// иначе чекаем флаг, если не выставляли ожидание на 1-2 минуты, то ставим и ждем пока натикает 1-2 минуты
  if(!b_Wait2Minute){
    cntr_Wait_Oil = 10;
    b_Wait2Minute = true;
  }else{
    /// и крутимся здесь необходимое нам премя
    if(cntr_Wait_Oil){return isFirstLevel;}
  }
  /// После чего идет вторая временка, которая отыгрывает период в 1 сек - работает насос, 2 сек - не работает
  if(cntr_pump_period){return isFirstLevel;}
  
  switch(v_Pump_Period)
  {
    case _1s :
      cntr_pump_period = _1s;
      _Pump_On();  
      v_Pump_Period = _2s;
    break;
      
    case _2s :            
      cntr_pump_period = _2s;
      _Pump_Off();  
      v_Pump_Period = _1s;
    break;
    default:break;
  }
  return isFirstLevel;
}


void setup_CLK(void)
{
       CLK_DeInit();
       CLK_HSECmd(DISABLE);
       CLK_LSICmd(DISABLE);
       CLK_HSICmd(ENABLE);
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

void setup_IWDG(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(0xFF);
  IWDG_Enable();
}

void setup_TIM2(void)
{ 
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_4096, 244);
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  TIM2_Cmd(ENABLE);
}

void All_OFF()
{
  //_LED_Off();
  _AIR_Off();
  _FIRE_Off();
  if(!b_Ready){_TEN_Off();}
  //b_State_system = false;
  cntr_Check_CRASH_T10 = _10s;
}

