#ifndef _ADC_H_
#define _ADC_H_

#include "stm8s.h"


typedef struct{
  uint16_t A3;
  uint16_t A4;
  uint16_t A5;
  uint16_t A6; 
  bool start_stop;
  bool data_ready;
}_str_ADC_value;

void _ADC_setup(void);
void ADC_update(void);
#endif	//_GPIO_H_