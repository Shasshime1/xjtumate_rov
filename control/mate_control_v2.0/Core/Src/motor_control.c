#include "./motor_control.h"

static void Motor_ControlDirect(rc_translation_t *rc_translation);
static void Motor_ControlAngle(control_t *controlt,rc_translation_t *rc_translation);
// static float StateMotorLimit(rc_translation_t *rc_translation,float motornum);


//在自稳定模式下对电机进行限制
// static float StateMotorLimit(rc_translation_t *rc_translation,float motornum)
// {
// 	if(rc_translation->z > 0)
// 	{
// 		if( motornum < 0) return 0;
// 		else return motornum;
// 	}
// 	else
// 	{
// 		if( motornum > 0 ) return 0;
// 		else return motornum;
// 	}
// }

//电机限速
static uint16_t Motor_Constrain(float motor)
{
    if(motor > MAXTHROTTLE_LIMIT) return MAXTHROTTLE_LIMIT;
    if(motor < MAXTHROTTLE_LIMIT_INV) return MAXTHROTTLE_LIMIT_INV;
    else return motor;
}


struct Motor
{
    float m1, m2, m3, m4, m5, m6, m7, m8;
}motor;

//初始化电机pwm值
void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
}



//电机断联保护
void Motor_LostProtect(void)
{
			 htim12.Instance->CCR1 = MINTHROTTLE; 
			 htim12.Instance->CCR2 = MINTHROTTLE; 

			 htim3.Instance->CCR4 = MINTHROTTLE;  
			 htim3.Instance->CCR3= MINTHROTTLE;   


			 htim3.Instance->CCR2= MINTHROTTLE;
			 htim3.Instance->CCR1= MINTHROTTLE;

			 htim4.Instance->CCR3= MINTHROTTLE;
			 htim4.Instance->CCR4= MINTHROTTLE;
	
}


//关闭pid直接控制上升和下潜
static void Motor_ControlDirect(rc_translation_t *rc_translation)
{
	motor.m1 = rc_translation->z + MINTHROTTLE;
	motor.m2 = rc_translation->z + MINTHROTTLE;
	motor.m3 = rc_translation->z + MINTHROTTLE;
	motor.m4 = rc_translation->z + MINTHROTTLE;
	motor.m5 = -rc_translation->x - rc_translation->y - rc_translation->yaw + MINTHROTTLE ;
	motor.m6 = rc_translation->x - rc_translation->y +  rc_translation->yaw + MINTHROTTLE;
	motor.m7 = rc_translation->x + rc_translation->y - rc_translation->yaw + MINTHROTTLE;
	motor.m8 = -rc_translation->x + rc_translation->y + rc_translation->yaw + MINTHROTTLE;

	//1-8号电机限速
	motor.m1 = Motor_Constrain(motor.m1);
	motor.m2 = Motor_Constrain(motor.m2);
	motor.m3 = Motor_Constrain(motor.m3);
	motor.m4 = Motor_Constrain(motor.m4);
	motor.m5 = Motor_Constrain(motor.m5);
	motor.m6 = Motor_Constrain(motor.m6);
	motor.m7 = Motor_Constrain(motor.m7);
	motor.m8 = Motor_Constrain(motor.m8);
	
}


//机器人开启自稳
static void Motor_ControlAngle(control_t *control,rc_translation_t *rc_translation)
{

	motor.m1 = rc_translation->z + control->roll - control->pitch;
	motor.m2 = rc_translation->z + control->roll + control->pitch; 
	motor.m3 = rc_translation->z - control->roll + control->pitch; 
	motor.m4 = rc_translation->z - control->roll - control->pitch;
	motor.m1 = motor.m1 + MINTHROTTLE;
	motor.m2 = motor.m2 + MINTHROTTLE;
	motor.m3 = motor.m3 + MINTHROTTLE;
	motor.m4 = motor.m4 + MINTHROTTLE;
	

	motor.m5 = -rc_translation->x - rc_translation->y - rc_translation->yaw + control->yaw + MINTHROTTLE;
	motor.m6 = rc_translation->x - rc_translation->y + rc_translation->yaw - control->yaw + MINTHROTTLE;
	motor.m7 = rc_translation->x + rc_translation->y - rc_translation->yaw + control->yaw + MINTHROTTLE;
	motor.m8 = -rc_translation->x + rc_translation->y + rc_translation->yaw - control->yaw + MINTHROTTLE;

	//电机限速
	motor.m1 = Motor_Constrain(motor.m1);
	motor.m2 = Motor_Constrain(motor.m2);
	motor.m3 = Motor_Constrain(motor.m3);
	motor.m4 = Motor_Constrain(motor.m4);
	motor.m5 = Motor_Constrain(motor.m5);
	motor.m6 = Motor_Constrain(motor.m6);
	motor.m7 = Motor_Constrain(motor.m7);
	motor.m8 = Motor_Constrain(motor.m8);
}


void Motor_Control(control_t *control,rc_translation_t *rc_translation,uint8_t controlmode)
{
	//查看开启什么模式
	if(controlmode == MATE_AUTO)
		Motor_ControlAngle(control,rc_translation);
	if(controlmode == MATE_MANUAL)
		Motor_ControlDirect(rc_translation);
	//设置电机pwm值
	htim12.Instance->CCR1 = motor.m6;
	htim12.Instance->CCR2 = motor.m5; 
	htim3.Instance->CCR4 = motor.m7;
	htim3.Instance->CCR3= motor.m8; 
	htim3.Instance->CCR2= motor.m1;
	htim3.Instance->CCR1= motor.m2;
	htim4.Instance->CCR3= motor.m4;
	htim4.Instance->CCR4= motor.m3;
}


