#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm8s.h"

void _GPIO_setup(void);
// OUTPUT
void _LED_On(void);
void _LED_Off(void);
void _Pump_On(void);
void _Pump_Off(void);
void _AIR_On(void);
void _AIR_Off(void);
void _TEN_On(void);
void _TEN_Off(void);

// INPUT
BitStatus _IsThereLight(void); 
BitStatus _IsThereStart(void); 

#endif	//_GPIO_H_