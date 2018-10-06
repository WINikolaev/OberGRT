#ifndef _AllThings_H_
#define _AllThings_H_

#include "stm8s_it.h"
#include "stm8s.h"
#include "ADC.h"
#include "GPIO.h"


/// This`s for account period (T = 0.5s) so _1s = 2*0.5
typedef enum {_1s = 2, _2s = 4, _10s = 20}_e_period;
/// Состояния управляющей платы
typedef enum {Check_fuel, START_Off, START_On_and_Check, START_OK, CRASH}_e_State_machine;
/// Buttom check drz
typedef enum {First_sample, Second_sample}_e_Buttom_drz;


/// Config ALL periheral
void setup_Peripheral(void);



void setup_CLK(void);
void setup_TIM2(void);
void setup_IWDG(void);


void checkFuel_Level(_str_ADC_value* ADC_value);
void checkTemperatureOil(_str_ADC_value* ADC);
bool getOilTemperature();
#endif	//_AllThings_H_