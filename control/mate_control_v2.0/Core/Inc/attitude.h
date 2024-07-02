#if !defined(__attitude_h__)
#define __attitude_h__



#include "usart.h"
#include "./REG.h"
#include "./wit_c_sdk.h"
#include "./state_control_type.h"

void Wit_Sensor_Init(void);

void Get_Attitude_Update(sensor_data_t *temp);

#endif // __attitude_h__
