#ifndef _ADC_H_
#define _ADC_H_

#include "stm8s.h"

#define DEV 5

typedef struct{
  uint16_t A3;
  uint16_t A4;
  uint16_t A5;
  uint16_t A6; 
  bool start_stop;
  bool data_ready;
}str_ADC_value;

void setup_ADC();
bool get_ADC_Ready();
str_ADC_value* get_ADC_Value();
void adc_Updating();
uint16_t _Resistor_Filter(uint16_t value);
#endif	//_GPIO_H_