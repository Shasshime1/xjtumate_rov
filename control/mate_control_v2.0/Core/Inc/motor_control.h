#ifndef __motor_control_h__
#define __motor_control_h__

#include "tim.h"
#include "./state_control.h"
#include "./state_control_type.h"
#include "./rc_command.h"
#include "stm32f4xx_it.h"
#include "./mate_math.h"



void Motor_Init(void);
void Motor_Control(control_t *control,rc_translation_t *rc_translation,uint8_t controlmode);
void Motor_LostProtect(void);



#endif // !__motor_control_h__
