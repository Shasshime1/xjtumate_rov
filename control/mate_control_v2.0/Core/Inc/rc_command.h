#ifndef __rc_command_H__
#define __rc_command_H__


//遥控器摇杆原始值
#define RC_DEADBAND_UP      132  //摇杆上死区
#define RC_DEADBAND_DOWN    122  //摇杆下死区
#define RC_ROCKER_MEDIAN    127  //摇杆中值
#define RC_ROCKER_MAX       255  //摇杆最大值
#define RC_PWM_VALUE_TRANS  (int)((MAXTHROTTLE_LIMIT-MINTHROTTLE)/(RC_ROCKER_MAX-RC_ROCKER_MEDIAN))   //遥控器油门值转化为绝对pwm值的放大倍数

#define AIL_HIGH            2
#define AIL_LOW             3
#define ELE_HIGH            4
#define ELE_LOW             5
#define THR_HIGH            6
#define THR_LOW             7
#define RUD_HIGH            8
#define RUD_LOW             9
#define WHEEL_HIGH          10
#define WHEEL_LOW           11
#define BOTTON_HIGH		    12
#define BOTTON_LOW          13


#include "stdint.h"
#include "./mate_math.h"
#include "stm32f4xx_it.h"
#include "./state_control_type.h"


//转化得到的pwm相对值
typedef struct 
{
    int16_t y;
    int16_t x;
    int16_t yaw;
    int16_t z;
    int16_t botton;
}rc_translation_t;


//数据分包后遥控器原始指令
typedef struct 
{
    int16_t ailevenom; //平移 
    int16_t elevator;  //前进后退
    int16_t throttel;  //油门
    int16_t rudder;    //偏航
}rc_command_t;

extern float shasshi[10];

void RC_CommandRecive(rc_translation_t *rc_translation);


#endif // 
