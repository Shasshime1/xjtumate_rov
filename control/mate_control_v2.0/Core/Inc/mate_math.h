#ifndef __mate_math_h__
#define __mate_math_h__ 

#include "math.h"
#include "stdio.h"
#include "stdint.h"

extern uint8_t ascii_dictionary[128];

int constrain(int amt, int low, int high);
float constrainf(float amt, float low, float high);
void Ascii_ChangeFuc_Init(void);
float YAW_ErroLimit(float data,float expect_data);


#endif // !__mate_math_h__
