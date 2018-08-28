#ifndef _ADC_H_
#define _ADC_H_

#include "stm8s.h"


typedef struct{
  uint16_t A3;
  uint16_t A4;
  uint16_t A5;
  uint16_t A6; 
  bool fresh_data;
  bool read_data;
}_str_adc_value;

#endif	//_GPIO_H_