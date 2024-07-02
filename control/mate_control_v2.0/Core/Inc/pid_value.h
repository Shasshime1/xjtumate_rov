#ifndef __pid_value_h__
#define __pid_value_h__

//角速度
#define RATE_ROLL_KP  0.0
#define RATE_ROLL_KI  0.0
#define RATE_ROLL_KD  0.0

#define RATE_PITCH_KP  0.0
#define RATE_PITCH_KI  0.0
#define RATE_PITCH_KD  0.0

#define RATE_YAW_KP  0.0
#define RATE_YAW_KI  0.0
#define RATE_YAW_KD  0.0

//角度
#define ANGLE_ROLL_KP 10.0
#define ANGLE_ROLL_KI 0.05
#define ANGLE_ROLL_KD 0.0

#define ANGLE_PITCH_KP 10.0
#define ANGLE_PITCH_KI 0.05
#define ANGLE_PITCH_KD 0.0

#define ANGLE_YAW_KP 10.0
#define ANGLE_YAW_KI 0.0
#define ANGLE_YAW_KD 0.0


/*角速度PID积分限幅（单位：deg/s）*/
#define PID_RATE_ROLL_INTEGRATION_LIMIT		200.0
#define PID_RATE_PITCH_INTEGRATION_LIMIT	200.0
#define PID_RATE_YAW_INTEGRATION_LIMIT		100.0

/*角速度PID输出限幅（单位：油门值）*/
#define PID_RATE_ROLL_OUTPUT_LIMIT			500.0
#define PID_RATE_PITCH_OUTPUT_LIMIT			500.0
#define PID_RATE_YAW_OUTPUT_LIMIT			300.0

/*角度PID输出限幅（单位：deg/s）*/
#define PID_ANGLE_ROLL_OUTPUT_LIMIT    		150.0
#define PID_ANGLE_PITCH_OUTPUT_LIMIT   		150.0
#define PID_ANGLE_YAW_OUTPUT_LIMIT     		300.0


#endif // !__pid_value_h__
