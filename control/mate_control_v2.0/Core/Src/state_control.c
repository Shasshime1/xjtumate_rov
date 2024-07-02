#include "./state_control.h"

PidObject pid[PID_NUM];

float temp[4] ;

//角度传参数组
static struct 
{
    float pitch;
    float roll;
    float yaw;
}AngleExcept;


//角速度传参数组
static struct 
{
    float pitch;
    float roll;
    float yaw;
}AngleRateExpect;


//对int类型的数据进行限幅


//pid参数初始化
static void All_PID_Init(void)
{
    //角速度PID（roll\pitch\yaw）
    pidInit(&pid[RATE_ROLL],RATE_ROLL_KP, RATE_ROLL_KI, RATE_ROLL_KD, 
			PID_RATE_ROLL_INTEGRATION_LIMIT, PID_RATE_ROLL_OUTPUT_LIMIT, 0.001f);
	pidInit(&pid[RATE_PITCH], RATE_PITCH_KP, RATE_PITCH_KI, RATE_PITCH_KD, 
			PID_RATE_PITCH_INTEGRATION_LIMIT, PID_RATE_PITCH_OUTPUT_LIMIT, 0.001f);
	pidInit(&pid[RATE_YAW],RATE_YAW_KP, RATE_YAW_KI, RATE_YAW_KD, 
			PID_RATE_YAW_INTEGRATION_LIMIT, PID_RATE_YAW_OUTPUT_LIMIT, 0.001f);
	
	//角度PID（roll\pitch\yaw）
	pidInit(&pid[ANGLE_ROLL], ANGLE_ROLL_KP, ANGLE_ROLL_KI, ANGLE_ROLL_KD,
		 	0, PID_ANGLE_ROLL_OUTPUT_LIMIT, 0.001f);
	pidInit(&pid[ANGLE_PITCH], ANGLE_PITCH_KP, ANGLE_PITCH_KI, ANGLE_PITCH_KD, 
			0, PID_ANGLE_PITCH_OUTPUT_LIMIT, 0.001f);
	pidInit(&pid[ANGLE_YAW], ANGLE_YAW_KP, ANGLE_YAW_KI, ANGLE_YAW_KD, 
			0, PID_ANGLE_YAW_OUTPUT_LIMIT, 0.001f);
}

void PID_Init(void)
{

	All_PID_Init();

}

void State_Control(const sensor_data_t *sensor_data, angle_expect_t *angle_expect,rc_translation_t *rc_translation, control_t *control,  uint32_t tick)
{

    AngleExcept.roll = angle_expect->roll;
    AngleExcept.pitch = angle_expect->pitch;
    AngleExcept.yaw = angle_expect->yaw;

	//角度PID
	if (RATE_DO_EXECUTE(_500_HZ, tick))
	{
		AngleRateExpect.roll = pidUpdate(&pid[ANGLE_ROLL], AngleExcept.roll - sensor_data->angle[ROLL]);
		AngleRateExpect.pitch = pidUpdate(&pid[ANGLE_PITCH], AngleExcept.pitch - sensor_data->angle[PITCH]);
		
		//判断偏航油门是否为0（限定死区后），如果在，就启用pid
		if( rc_translation->yaw == 0)
		{
			AngleRateExpect.yaw = pidUpdate(&pid[ANGLE_YAW], YAW_ErroLimit(sensor_data->angle[YAW],AngleExcept.yaw));
			if(-2.0f < sensor_data->angle[YAW] - AngleExcept.yaw && sensor_data->angle[YAW] - AngleExcept.yaw< 2.0f)
				AngleRateExpect.yaw = 0.0;
		}
		if(  rc_translation->yaw != 0 )
		{
		 	AngleRateExpect.yaw = 0;
			angle_expect->yaw = sensor_data->angle[YAW];
		}

		if(	COMMANDER_MINCHECK_RELATIVE_INV <= rc_translation->z <= COMMANDER_MINCHECK_RELATIVE)
		{
			pidResetIntegral(&pid[RATE_ROLL]);
			pidResetIntegral(&pid[RATE_PITCH]);
			pidResetIntegral(&pid[RATE_YAW]);
			// stateControlResetYawHolding();//复位航向角锁定
		}
    }
    //角速度PID
//	if (RATE_DO_EXECUTE(_500_HZ, tick))
//    {
//		// 如果油门值小于MINCHECK时即怠速状态，清除PID积分，防止积分累计导致电机转速不一致
//		if(	COMMANDER_MINCHECK_RELATIVE_INV <= rc_translation->z <= COMMANDER_MINCHECK_RELATIVE)
//		{
//			pidResetIntegral(&pid[RATE_ROLL]);
//			pidResetIntegral(&pid[RATE_PITCH]);
//			pidResetIntegral(&pid[RATE_YAW]);
//			// stateControlResetYawHolding();//复位航向角锁定
//		}
//		control->roll = pidUpdate(&pid[RATE_ROLL], AngleRateExpect.roll - sensor_data->gyro[ROLL]);
//		control->pitch = pidUpdate(&pid[RATE_PITCH], AngleRateExpect.pitch - sensor_data->gyro[PITCH]);
//		
//		// if( rc_translation->yaw == 0 )
//		// 	control->yaw = pidUpdate(&pid[RATE_YAW], AngleRateExpect.yaw - sensor_data->gyro[YAW]);
//		// else 
//			control->yaw = 0;
//	}
		
		control->pitch = AngleRateExpect.pitch;
		control->roll = AngleRateExpect.roll;
		control->yaw = AngleRateExpect.yaw;
	//更新航向角
}

// //锁定当前航向角
// void stateControlResetYawHolding(void)
// {
// 	attitudeDesired.yaw = state.attitude.yaw;
// }
