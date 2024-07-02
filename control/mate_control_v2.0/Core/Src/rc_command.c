#include "./rc_command.h"

static rc_command_t rc_command;
float shasshi[10];


static void RC_CommandChange(rc_translation_t *rc_translation)
{
    if( RC_DEADBAND_DOWN <= rc_command.rudder && rc_command.rudder <=  RC_DEADBAND_UP) 
				rc_translation->yaw = 0;
		else 	
				rc_translation->yaw = (rc_command.rudder-RC_ROCKER_MEDIAN) * RC_PWM_VALUE_TRANS/2;
    if(RC_DEADBAND_DOWN <= rc_command.throttel && rc_command.throttel <= RC_DEADBAND_UP) 
				rc_translation->z = 0;
	  else	
				rc_translation->z = (rc_command.throttel-RC_ROCKER_MEDIAN) * RC_PWM_VALUE_TRANS;
    if(RC_DEADBAND_DOWN <= rc_command.ailevenom && rc_command.ailevenom <= RC_DEADBAND_UP) 
				rc_translation->y = 0;
		else	
				rc_translation->y = (rc_command.ailevenom-RC_ROCKER_MEDIAN) * RC_PWM_VALUE_TRANS;
    if(RC_DEADBAND_DOWN <= rc_command.elevator && rc_command.elevator <= RC_DEADBAND_UP) 
				rc_translation->x = 0;
	  else 
			rc_translation->x = (rc_command.elevator-RC_ROCKER_MEDIAN) * RC_PWM_VALUE_TRANS;
}

//从dma中获取遥控器摇杆值
void RC_CommandRecive(rc_translation_t *rc_translation)
{
        rc_command.ailevenom = ascii_dictionary[Getrccommandtemp(AIL_HIGH)]*16+ascii_dictionary[Getrccommandtemp(AIL_LOW)];
        rc_command.elevator = ascii_dictionary[Getrccommandtemp(ELE_HIGH)]*16+ascii_dictionary[Getrccommandtemp(ELE_LOW)];
        rc_command.throttel = ascii_dictionary[Getrccommandtemp(THR_HIGH)]*16+ascii_dictionary[Getrccommandtemp(THR_LOW)];
        rc_command.rudder = ascii_dictionary[Getrccommandtemp(RUD_HIGH)]*16+ascii_dictionary[Getrccommandtemp(RUD_LOW)];		
		rc_translation->botton = ascii_dictionary[Getrccommandtemp(BOTTON_HIGH)]*16+ascii_dictionary[Getrccommandtemp(BOTTON_LOW)];
		RC_CommandChange(rc_translation);
}
