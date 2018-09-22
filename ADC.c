#include "ADC.h"
#include "GPIO.h"
_str_ADC_value ADC_value;



void ADC_Updating(void)
{
  if(!ADC_value.start_stop){     
     ADC1_ScanModeCmd(ENABLE);
     ADC1_StartConversion();
     ADC_value.data_ready = false;
     ADC_value.start_stop = true;
   }
  
   if(ADC1_GetFlagStatus(ADC1_FLAG_EOC)){
     ADC1_ClearFlag(ADC1_FLAG_EOC);
     ADC_value.start_stop = false;
      
     ADC_value.A6 = ADC1_GetBufferValue(6);
     ADC_value.A5 = ADC1_GetBufferValue(5);
     ADC_value.A4 = ADC1_GetBufferValue(4);
     ADC_value.A3 = ADC1_GetBufferValue(3);
     ADC_value.data_ready = true;
   }
}




void setup_ADC(void)
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
        /// Update flag value
        ADC_value.start_stop = false;
}