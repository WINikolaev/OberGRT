#ifndef _AllThings_H_
#define _AllThings_H_

#include "stm8s.h"
#include "ADC.h"
#include "GPIO.h"

/// Config ALL periheral
void setup_Peripheral(void);



void setup_CLK(void);
void setup_TIM2(void);
void setup_IWDG(void);

#endif	//_AllThings_H_