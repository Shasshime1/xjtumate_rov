#if !defined(__state_control_type_h__)
#define __state_control_type_h__

#include "stdio.h"
#include "stdint.h"

#define MINTHROTTLE						2250 //电机停转值
#define MAXTHROTTLE_LIMIT				2750 //限制70%正向油门最大值
#define MAXTHROTTLE_LIMIT_INV 			1750 //限制70%反向油门最大值
#define COMMANDER_MINCHECK 				2325 //正向怠速值
#define COMMANDER_MINCHECK_INV 			2175 //反向怠速值
#define COMMANDER_MINCHECK_RELATIVE 	75   //正向油门怠速值相对值
#define COMMANDER_MINCHECK_RELATIVE_INV -75  //反向油门怠速相对值
#define MATE_AUTO                            0    //自稳模式
#define MATE_MANUAL                          1    //手动模式

#define SERVO_MIDDLE                    130  //云台舵机中值
#define SERVO_MIN                       100  //云台舵机最小值
#define SERVO_MAX                       180 //云台舵机最大值

#define ROLL   0
#define PITCH  1
#define YAW    2

#define ACC_X  0
#define ACC_Y  1
#define ACC_Z  2

#define GYRO_X 0
#define GYRO_Y 1
#define GYRO_Z 2

#define Z_UP   0
#define Z_DOWN 1


//陀螺仪数据整合
typedef struct {
    float angle[3];
    float acc[3];
    float gyro[3];

}sensor_data_t;

//姿态数据预期值
typedef struct 
{
	float yaw;
	float pitch;
	float roll;
}angle_expect_t;



// 姿态控制数据
typedef struct
{
	float roll;
	float pitch;
	float yaw;
}control_t;




#endif // __state_control_type_h__
