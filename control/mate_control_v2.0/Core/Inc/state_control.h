#ifndef __state_control_h__
#define __state_control_h__

#include "./state_control_type.h"
#include "./attitude.h"
#include "./ratetype.h"
#include "./PID.h"
#include "./pid_value.h"
#include "./motor_control.h"
#include "./mate_math.h"
#include "./rc_command.h"

enum pidIndex
{
	RATE_ROLL = 0,
	RATE_PITCH,
	RATE_YAW,
	ANGLE_ROLL,
	ANGLE_PITCH,
	ANGLE_YAW,
	PID_NUM
};

extern float temp[4];
int constrain(int amt, int low, int high);
void PID_Init(void);

void State_Control(const sensor_data_t *sensor_data, angle_expect_t *angle_expect,rc_translation_t *rc_translation_t, control_t *control,  uint32_t tick);

#endif // !__state_control_h__
