#include "./mate_math.h"


uint8_t ascii_dictionary[128];

// uint16_t throttel_curve


//对int形数据进行限幅
int constrain(int amt, int low, int high)
{
    if (amt < low)
        return low;
    else if (amt > high)
        return high;
    else
        return amt;
}


// 对float数据进行限幅
float constrainf(float amt, float low, float high)
{
    if (amt < low)
        return low;
    else if (amt > high)
        return high;
    else
        return amt;
}



//将ascii码转化为16进制
void Ascii_ChangeFuc_Init(void)
{
	ascii_dictionary[0x30] = 0;
	ascii_dictionary[0x31] = 1;
	ascii_dictionary[0x32] = 2;
	ascii_dictionary[0x33] = 3;
	ascii_dictionary[0x34] = 4;
	ascii_dictionary[0x35] = 5;
	ascii_dictionary[0x36] = 6;
	ascii_dictionary[0x37] = 7;
	ascii_dictionary[0x38] = 8;
	ascii_dictionary[0x39] = 9;
	ascii_dictionary[0x61] = 10;
	ascii_dictionary[0x62] = 11;
	ascii_dictionary[0x63] = 12;
	ascii_dictionary[0x64] = 13;
	ascii_dictionary[0x65] = 14;
	ascii_dictionary[0x66] = 15;
}




//由于陀螺仪的角度是0到正负180度，所以需要对YAW轴计算所得的误差进行判断
float YAW_ErroLimit(float data,float expect_data)
{
	if((expect_data > 0 && data > 0) || (expect_data < 0 && data < 0))
		return (expect_data - data);
	if(expect_data < 0 && data > 0 )
		return (360.0f + expect_data - data);
	if(expect_data > 0 && data < 0)
		return (data - expect_data - 360.0f);
	return 0;
}
