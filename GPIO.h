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
void _FIRE_On(void);
void _FIRE_Off(void);
void _TEN_On(void);
void _TEN_Off(void);

// INPUT
BitStatus _IsThereLight(void); 
BitStatus _IsThereStart(void); 


// Check State input
typedef enum {_1, _2, _3, _4, _5, _6, _7, _8}_e_count_input;
typedef struct{
  uint8_t b0: 1;
  uint8_t b1: 1;
  uint8_t b2: 1;
  uint8_t b3: 1;
  uint8_t b4: 1;
  uint8_t b5: 1;
  uint8_t b6: 1;
  uint8_t b7: 1;
}str_bit;

typedef union{
  uint8_t a;
  str_bit b;
}u_input_read;


void inputReade(void);

bool get_inputStart(void);
bool get_inputLight(void);
#endif	//_GPIO_H_