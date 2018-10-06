#include "AllThings.h"

extern uint8_t cntr_pump_period;
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

bool _ref_b_OilTemperatureOK = false;

void checkTemperatureOil(_str_ADC_value* ADC)
{
  if(!ADC->data_ready){return;}
  
  if(!(ADC->A3 > ADC->A4)){
    _TEN_On(); 
    _ref_b_OilTemperatureOK = false;
  }else{
    _TEN_Off();
    _ref_b_OilTemperatureOK = true;   
  }
}

bool getOilTemperature(){return _ref_b_OilTemperatureOK;}



/// Проверяем уровень масла
_e_period v_Pump_Period = _1s;
void checkFuel_Level(_str_ADC_value* ADC)
{
  if(!ADC->data_ready){return;}
  
  /// 25% ot ADC_value.A5
  uint16_t _25pA5 = ADC->A5/4;
  
  ADC->A5 -=_25pA5;
  
  if(!(ADC->A5 < ADC->A6)){
    _Pump_Off(); 
    return;
  }
  
  if(cntr_pump_period){return;}
  
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
