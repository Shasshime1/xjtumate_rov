#include "./attitude.h"

#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80
static volatile char s_cDataUpdate = 0, s_cCmd = 0xff;
float fAcc[3], fGyro[3], fAngle[3];

static void AutoScanSensor(void);
static void SensorUartSend(uint8_t *p_data, uint32_t uiSize);
static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);

static void delay_ms()
{
	uint32_t times = 10000000;
	while (times--);	
}

void Wit_Sensor_Init(void)
{
    WitInit(WIT_PROTOCOL_NORMAL, 0x50);
	WitSerialWriteRegister(SensorUartSend);
	WitRegisterCallBack(SensorDataUpdata);
	AutoScanSensor();
}

void Get_Attitude_Update(sensor_data_t *temp)
{
	int i;
	if(s_cDataUpdate)
	{
		for(i = 0; i < 3; i++)
		{
			fAcc[i] = sReg[AX+i] / 32768.0f * 16.0f;
			fGyro[i] = sReg[GX+i] / 32768.0f * 2000.0f;
			fAngle[i] = sReg[Roll+i] / 32768.0f * 180.0f;
		}
		if(s_cDataUpdate & ACC_UPDATE)
		{
			temp->acc[0] = fAcc[0];
			temp->acc[1] = fAcc[1];
			temp->acc[2] = fAcc[2];
			s_cDataUpdate &= ~ACC_UPDATE;
		}
		if(s_cDataUpdate & GYRO_UPDATE)
		{
			temp->gyro[0] =  fGyro[1];
			temp->gyro[1] = -fGyro[0];
			temp->gyro[2] = fGyro[2];
			s_cDataUpdate &= ~GYRO_UPDATE;
		}
		if(s_cDataUpdate & ANGLE_UPDATE)
		{
			temp->angle[0] = fAngle[1];
			temp->angle[1] = -fAngle[0];
			temp->angle[2] = fAngle[2];
			s_cDataUpdate &= ~ANGLE_UPDATE;
		}
	}
}


static void SensorUartSend(uint8_t *p_data, uint32_t uiSize)
{
	uint16_t Size = (uint16_t)uiSize;
	HAL_UART_Transmit(&huart6, p_data, Size, HAL_MAX_DELAY);
}


static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum)
{
	int i;
    for(i = 0; i < uiRegNum; i++)
    {
        switch(uiReg)
        {
//            case AX:
//            case AY:
            case AZ:
				s_cDataUpdate |= ACC_UPDATE;
            break;
//            case GX:
//            case GY:
            case GZ:
				s_cDataUpdate |= GYRO_UPDATE;
            break;
//            case HX:
//            case HY:
            case HZ:
				s_cDataUpdate |= MAG_UPDATE;
            break;
//            case Roll:
//            case Pitch:
            case Yaw:
				s_cDataUpdate |= ANGLE_UPDATE;
            break;
            default:
				s_cDataUpdate |= READ_UPDATE;
			break;
        }
		uiReg++;
    }
}

static void AutoScanSensor(void)
{
	int i, iRetry;
	
	for(i = 1; i < 10; i++)
	{
		iRetry = 2;
		do
		{
			s_cDataUpdate = 0;
			WitReadReg(AX, 3);
			delay_ms();
			if(s_cDataUpdate != 0)
			{
				return ;
			}
			iRetry--;
		}while(iRetry);		
	}
}

