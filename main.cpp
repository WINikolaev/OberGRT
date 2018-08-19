#include "stm8s.h"

void _ADC_setup(void);
void _Clock_setup(void);
void _WWDG_setup(void);

uint16_t A3 = 0x0000;
uint16_t A4 = 0x0000;
uint16_t A5 = 0x0000;
uint16_t A6 = 0x0000; 

void main()
{
  
  _Clock_setup();
  _ADC_setup();
  
  GPIO_Init(GPIOB, GPIO_PIN_5,GPIO_MODE_OUT_OD_HIZ_SLOW);
  GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
  /// IWDG 
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(0xFF);
  IWDG_Enable();
  
  bool ADC_StartConversion = false;
  
  while(1)
  {
    IWDG_ReloadCounter();
    if(!ADC_StartConversion)
    {
      ADC1_ScanModeCmd(ENABLE);
      ADC1_StartConversion();
      ADC_StartConversion = true;
    }
    
    if(ADC1_GetFlagStatus(ADC1_FLAG_EOC))
    {
      ADC1_ClearFlag(ADC1_FLAG_EOC);
      ADC_StartConversion = false;
      
      A6 = ADC1_GetBufferValue(6);
      A5 = ADC1_GetBufferValue(5);
      A4 = ADC1_GetBufferValue(4);
      A3 = ADC1_GetBufferValue(3);
      
      if(A4 > 500){
        GPIO_WriteLow(GPIOB, GPIO_PIN_5);
        
      }else{
        GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
      }
    }
    
   }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  while(1){}
}
#endif

void _Clock_setup(void)
{
       CLK_DeInit();
       CLK_HSECmd(DISABLE);
       CLK_LSICmd(DISABLE);
       CLK_HSICmd(ENABLE);
       while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);
       CLK_ClockSwitchCmd(ENABLE);
       CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV4);
       CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
       CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI,
       DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, ENABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
       CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);
}

void _ADC_setup(void)
{
  
      GPIO_Init(GPIOD, GPIO_PIN_3,GPIO_MODE_IN_FL_NO_IT);
      GPIO_Init(GPIOD, GPIO_PIN_4,GPIO_MODE_IN_FL_NO_IT);
      GPIO_Init(GPIOD, GPIO_PIN_5,GPIO_MODE_IN_FL_NO_IT);
      GPIO_Init(GPIOD, GPIO_PIN_6,GPIO_MODE_IN_FL_NO_IT);
  
      ADC1_DeInit();     
       ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,
                 ADC1_CHANNEL_6,
                 ADC1_PRESSEL_FCPU_D18,
                 ADC1_EXTTRIG_GPIO,
                 DISABLE,
                 ADC1_ALIGN_RIGHT,
                 ADC1_SCHMITTTRIG_CHANNEL6,
                 DISABLE);
       ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,
                 ADC1_CHANNEL_5,
                 ADC1_PRESSEL_FCPU_D18,
                 ADC1_EXTTRIG_GPIO,
                 DISABLE,
                 ADC1_ALIGN_RIGHT,
                 ADC1_SCHMITTTRIG_CHANNEL5,
                 DISABLE);
        ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,
                 ADC1_CHANNEL_4,
                 ADC1_PRESSEL_FCPU_D18,
                 ADC1_EXTTRIG_GPIO,
                 DISABLE,
                 ADC1_ALIGN_RIGHT,
                 ADC1_SCHMITTTRIG_CHANNEL4,
                 DISABLE);
         ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,
                 ADC1_CHANNEL_3,
                 ADC1_PRESSEL_FCPU_D18,
                 ADC1_EXTTRIG_GPIO,
                 DISABLE,
                 ADC1_ALIGN_RIGHT,
                 ADC1_SCHMITTTRIG_CHANNEL3,
                 DISABLE);
        
        ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, (ADC1_Channel_TypeDef)(ADC1_CHANNEL_3 | ADC1_CHANNEL_4 | ADC1_CHANNEL_5 | ADC1_CHANNEL_6), ADC1_ALIGN_RIGHT);
        ADC1_DataBufferCmd(ENABLE);
        ADC1_Cmd(ENABLE);
}
