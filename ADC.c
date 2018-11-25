#include "ADC.h"
#include "GPIO.h"


str_ADC_value ADC;

void adc_Updating()
{
  if(!ADC.start_stop){     
     ADC1_ScanModeCmd(ENABLE);
     ADC1_StartConversion();
     ADC.data_ready = false;
     ADC.start_stop = true;
   }
  
   if(ADC1_GetFlagStatus(ADC1_FLAG_EOC)){
     ADC1_ClearFlag(ADC1_FLAG_EOC);
     ADC.start_stop = false;
      
     //ADC.A6 = ADC1_GetBufferValue(6);
     //ADC->A5 = ADC1_GetBufferValue(5);
     ADC.A4 = _Resistor_Filter(ADC1_GetBufferValue(4));
     
     ADC.A3 = ADC1_GetBufferValue(3);
     ADC.data_ready = true;
   }
}


char cntr_i = 0x00;
uint16_t sum = 0x00;
uint16_t sum_save = 0x00;
uint16_t _Resistor_Filter(uint16_t value)
{
  if(cntr_i == DEV){
      sum_save = sum/DEV;
      sum = 0x00;
      cntr_i = 0x00;
      return sum_save;
  }
  sum += value;
  cntr_i++;
  return sum_save;
}


bool get_ADC_Ready()
{
  return ADC.data_ready;
}

str_ADC_value* get_ADC_Value()
{
  return &ADC;
}

void setup_ADC()
{
  
      ADC.A3 = 0x00;
      ADC.A4 = 0x00;
      ADC.A5 = 0x00;
      ADC.A6 = 0x00;
      ADC.data_ready = false;
      ADC.start_stop = false;
      
      GPIO_Init(GPIOD, GPIO_PIN_3,GPIO_MODE_IN_FL_NO_IT);
      GPIO_Init(GPIOD, GPIO_PIN_4,GPIO_MODE_IN_FL_NO_IT);
      //GPIO_Init(GPIOD, GPIO_PIN_6,GPIO_MODE_IN_FL_NO_IT);
  
      ADC1_DeInit();
      /*
       ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,
                 ADC1_CHANNEL_6,
                 ADC1_PRESSEL_FCPU_D18,
                 ADC1_EXTTRIG_GPIO,
                 DISABLE,
                 ADC1_ALIGN_RIGHT,
                 ADC1_SCHMITTTRIG_CHANNEL6,
                 DISABLE);
        */
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
        
        ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, (ADC1_Channel_TypeDef)(ADC1_CHANNEL_3 | ADC1_CHANNEL_4), ADC1_ALIGN_RIGHT);
        ADC1_DataBufferCmd(ENABLE);
        ADC1_Cmd(ENABLE);
        /// Update flag value
        ADC.start_stop = false;
}