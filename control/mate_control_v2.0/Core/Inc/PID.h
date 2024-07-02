#if !defined(__pid_h__)
#define __pid_h__

#include "./mate_math.h"


//pid参数对象
typedef struct
{
	float desired;		//< set point
	float error;        //< error
	float prevError;    //< previous error
	float integ;        //< integral
	float deriv;        //< derivative
	float kp;           //< proportional gain
	float ki;           //< integral gain
	float kd;           //< derivative gain
	float outP;         //< proportional output (debugging)
	float outI;         //< integral output (debugging)
	float outD;         //< derivative output (debugging)
	float iLimit;       //< integral limit
	float outputLimit;  //< total PID output limit, absolute value. '0' means no limit.
	float dt;           //< delta-time dt
} PidObject;

void pidInit(PidObject* pid, float kp, float ki, float kd, float iLimit, float outputLimit, float dt);
float pidUpdate(PidObject* pid, float error);
void pidReset(PidObject* pid);
void pidResetIntegral(PidObject* pid);
void pidSetIntegral(PidObject* pid, float integ);

#endif // __pid_h__
