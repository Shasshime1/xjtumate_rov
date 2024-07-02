/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "./wit_c_sdk.h"
#include "tim.h"
#include "./attitude.h"
#include "./state_control_type.h"
#include "./motor_control.h"
#include "./state_control.h"
#include "./ratetype.h"
#include "./led.h"
#include "stm32f4xx_it.h"
#include "usart.h"
#include "stm32f4xx_it.h"
#include "dma.h"
#include "./rc_command.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define USART1CHECKTURE  0x0001     //串口1没有断联
#define USART1CHECKERRO  0x0000     //串口1断联 

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

sensor_data_t sensor_data;    
angle_expect_t angle_expect;
control_t control;           
rc_translation_t rc_translation;

uint16_t usart1count_temp[3] = {0};

/* USER CODE END Variables */
/* Definitions for test */
osThreadId_t testHandle;
const osThreadAttr_t test_attributes = {
  .name = "test",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for AttitudeControl */
osThreadId_t AttitudeControlHandle;
const osThreadAttr_t AttitudeControl_attributes = {
  .name = "AttitudeControl",
  .stack_size = 2000 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for USART1Check */
osThreadId_t USART1CheckHandle;
const osThreadAttr_t USART1Check_attributes = {
  .name = "USART1Check",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for USARTStart */
osThreadId_t USARTStartHandle;
const osThreadAttr_t USARTStart_attributes = {
  .name = "USARTStart",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void AllPeripheral_Init(void);


/* USER CODE END FunctionPrototypes */

void Test(void *argument);
void Attitude_Control(void *argument);
void Usart1_Check(void *argument);
void USART_Start(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  AllPeripheral_Init();
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of test */
  testHandle = osThreadNew(Test, NULL, &test_attributes);

  /* creation of AttitudeControl */
  AttitudeControlHandle = osThreadNew(Attitude_Control, NULL, &AttitudeControl_attributes);

  /* creation of USART1Check */
  USART1CheckHandle = osThreadNew(Usart1_Check, NULL, &USART1Check_attributes);

  /* creation of USARTStart */
  USARTStartHandle = osThreadNew(USART_Start, NULL, &USARTStart_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Test */
/**
  * @brief  Function implementing the test thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Test */
void Test(void *argument)
{
  /* USER CODE BEGIN Test */
  /* Infinite loop */
  for(;;)
  {
    osDelay(5);
  }
  /* USER CODE END Test */
}

/* USER CODE BEGIN Header_Attitude_Control */
/**
* @brief Function implementing the AttitudeControl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Attitude_Control */
void Attitude_Control(void *argument)
{
  /* USER CODE BEGIN Attitude_Control */
  uint32_t tick = 0;
	uint32_t lastWakeTime = osKernelGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&lastWakeTime, 1);
    {
    	if(RATE_DO_EXECUTE(_1000_HZ,tick)) 
			  Get_Attitude_Update(&sensor_data);
			RC_CommandRecive(&rc_translation);
			State_Control(&sensor_data,&angle_expect,&rc_translation,&control,tick);
			if(RATE_DO_EXECUTE(_500_HZ,tick))
      {
        if(usart1count_temp[2] == USART1CHECKTURE)
          Motor_Control(&control,&rc_translation,MATE_AUTO);
        if(usart1count_temp[2] == USART1CHECKERRO)
          Motor_LostProtect();     
      }
    }
  //  if(tick >= 5000)
  //  {
	// 	  // htim12.Instance->CCR1 = MINTHROTTLE; 
	// 		//  htim12.Instance->CCR2 = MAXTHROTTLE_LIMIT; 
  //     //  htim3.Instance->CCR4 = MAXTHROTTLE_LIMIT;  
	// 		//  htim3.Instance->CCR3= MAXTHROTTLE_LIMIT;   
  //     //  htim3.Instance->CCR2= MAXTHROTTLE_LIMIT;
	// 		//  htim3.Instance->CCR1= MAXTHROTTLE_LIMIT;
	// 		//  htim4.Instance->CCR3= MAXTHROTTLE_LIMIT;
	// 		//  htim4.Instance->CCR4= MAXTHROTTLE_LIMIT;
  //  }
    tick++;
  }
  /* USER CODE END Attitude_Control */
}

/* USER CODE BEGIN Header_Usart1_Check */
/**
* @brief Function implementing the USART1Check thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Usart1_Check */
void Usart1_Check(void *argument)
{
  /* USER CODE BEGIN Usart1_Check */
  uint32_t tick = 0;
	uint32_t lastWakeTime = osKernelGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&lastWakeTime, 20);
    if(RATE_DO_EXECUTE(_50_HZ,tick))
      usart1count_temp[0] = GetUsart1Count();
    if(RATE_DO_EXECUTE(_10_HZ,tick))
      usart1count_temp[1] = GetUsart1Count();
    if(usart1count_temp[1] == usart1count_temp[0] && tick%50 != 0)
      usart1count_temp[2] = USART1CHECKERRO;
    if(usart1count_temp[1] != usart1count_temp[0]) 
      usart1count_temp[2] = USART1CHECKTURE;
    if(rc_translation.botton/2 % 2 )
      RobLightOn();
    if(rc_translation.botton/2/2/2/2/2/2%2)
      GetServoAngle(SERVO_MIN);
    if(rc_translation.botton/2 % 2 == 0)
      RobLightOff();
    if(rc_translation.botton/2/2/2/2/2/2%2 == 0)
      GetServoAngle(SERVO_MIDDLE);
    tick = tick + 20;
  }
  /* USER CODE END Usart1_Check */
}

/* USER CODE BEGIN Header_USART_Start */
/**
* @brief Function implementing the USARTStart thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_USART_Start */
void USART_Start(void *argument)
{
  /* USER CODE BEGIN USART_Start */
  /* Infinite loop */
  for(;;)
  {
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); 
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);       
    HAL_UART_Receive_DMA(&huart1,aRxBuffer,24);
    HAL_UART_Receive_DMA(&huart6,usart6_rxdata, 44);
    RobLightOff();
    GetServoAngle(SERVO_MIDDLE);
		osThreadTerminate(USARTStartHandle);
  }
  /* USER CODE END USART_Start */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

//
static void AllPeripheral_Init()
{
  Wit_Sensor_Init();
  Motor_Init();
  PID_Init();
	Ascii_ChangeFuc_Init();
  angle_expect.pitch = 0;
  angle_expect.roll = 0;
  angle_expect.yaw = 0;
}


/* USER CODE END Application */

