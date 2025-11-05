/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <stdio.h>
#include "usart.h"
#include "adc.h"
#include "event_groups.h"		//事件组相关头文件
#include "keyled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define  BITMASK_KEY_LEFT			0x04		//KeyLeft的事件位掩码，使用Bit2
#define  BITMASK_KEY_RIGHT		0x01		//KeyRight的事件位掩码，使用Bit0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Task_ADC */
osThreadId_t Task_ADCHandle;
const osThreadAttr_t Task_ADC_attributes = {
  .name = "Task_ADC",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_LED */
osThreadId_t Task_LEDHandle;
const osThreadAttr_t Task_LED_attributes = {
  .name = "Task_LED",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Task_ScanKeys */
osThreadId_t Task_ScanKeysHandle;
const osThreadAttr_t Task_ScanKeys_attributes = {
  .name = "Task_ScanKeys",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for myEvent01 */
osEventFlagsId_t myEvent01Handle;
const osEventFlagsAttr_t myEvent01_attributes = {
  .name = "myEvent01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_ADC(void *argument);
void AppTask_LED(void *argument);
void AppTask_ScanKeys(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

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
  /* creation of Task_ADC */
  Task_ADCHandle = osThreadNew(AppTask_ADC, NULL, &Task_ADC_attributes);

  /* creation of Task_LED */
  Task_LEDHandle = osThreadNew(AppTask_LED, NULL, &Task_LED_attributes);

  /* creation of Task_ScanKeys */
  Task_ScanKeysHandle = osThreadNew(AppTask_ScanKeys, NULL, &Task_ScanKeys_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of myEvent01 */
  myEvent01Handle = osEventFlagsNew(&myEvent01_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_ADC */
/**
  * @brief  Function implementing the Task_ADC thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_ADC */
void AppTask_ADC(void *argument)
{
  /* USER CODE BEGIN AppTask_ADC */
  /* Infinite loop */
	BaseType_t clearOnExit=pdTRUE;			//pdTRUE=退出时清除事件位
	BaseType_t waitForAllBits=pdTRUE;		//等待所有位置1，pdTRUE=逻辑与, pdFALSE=逻辑或
	EventBits_t bitsToWait=BITMASK_KEY_LEFT | BITMASK_KEY_RIGHT;  //Press the left and right buttons at the same time.
	for(;;)
	{
		xEventGroupWaitBits(myEvent01Handle, bitsToWait,
				clearOnExit,waitForAllBits,portMAX_DELAY );
		for(uint8_t i=0; i<10; i++)
		{
			HAL_ADC_Start(&hadc3);			//必须每次启动时convert
			if (HAL_ADC_PollForConversion(&hadc3,200)==HAL_OK)
			{
			  	// Data in the format of uint32_t cannot be sent through HAL_UART_Transmit(),
				// because this function can only send byte data in the format of uint8_t.
			  	uint32_t val = HAL_ADC_GetValue(&hadc3);
			  	printf("Data in ADC3_IN6 = %ld\r\n",val);

			  	// Convert the ADC3 sampling value to the engineering value
			  	// and then transmit it to the serial port through printf().
				uint32_t Volt = 3300*val;  		//mV
			  	Volt = Volt >> 12;  				//除以2^12，转换为工程值
			  	printf("Engineering Value = %ld\r\n",Volt );
			}
			HAL_ADC_Stop(&hadc3);			//可停止也可不停止
			vTaskDelay(pdMS_TO_TICKS(500));
		}
	}
  /* USER CODE END AppTask_ADC */
}

/* USER CODE BEGIN Header_AppTask_LED */
/**
* @brief Function implementing the Task_LED thread.
* 2 key pressed,LED1 shine 10 times。
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_LED */
void AppTask_LED(void *argument)
{
  /* USER CODE BEGIN AppTask_LED */
  /* Infinite loop */
	BaseType_t clearOnExit=pdTRUE;		//pdTRUE=退出时清除事件位
	BaseType_t waitForAllBits=pdTRUE;	//等待所有位置1，pdTRUE=逻辑与, pdFALSE=逻辑或
	EventBits_t bitsToWait=BITMASK_KEY_LEFT | BITMASK_KEY_RIGHT;  //等待的事件位
	for(;;)
	{
		xEventGroupWaitBits(myEvent01Handle, bitsToWait,
				clearOnExit,waitForAllBits,portMAX_DELAY );
		for(uint8_t i=0; i<10; i++)  		//使LED1闪烁几次
		{
			LED1_Toggle();
			vTaskDelay(pdMS_TO_TICKS(500));
		}
	}
  /* USER CODE END AppTask_LED */
}

/* USER CODE BEGIN Header_AppTask_ScanKeys */
/**
* @brief Function implementing the Task_ScanKeys thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_ScanKeys */
void AppTask_ScanKeys(void *argument)
{
  /* USER CODE BEGIN AppTask_ScanKeys */
  /* Infinite loop */

	KEYS  keyCode = KEY_NONE;
	for(;;)
	{
		EventBits_t curBits=xEventGroupGetBits(myEvent01Handle);	//读取事件组当前值
		printf("Current event bits = %ld\r\n",curBits);

		keyCode=ScanPressedKey(50);	//最多等待50ms，不能使用参数KEY_WAIT_ALWAYS
		switch (keyCode)
		{
			case	KEY_LEFT:		//Press S4 to set bit2 to 1.
				xEventGroupSetBits(myEvent01Handle, BITMASK_KEY_LEFT);		//事件位Bit2置位
				break;

			case	KEY_RIGHT:	//Press S5 to set bit2 to 1.
				xEventGroupSetBits(myEvent01Handle, BITMASK_KEY_RIGHT);	//事件位Bit0置位
				break;

			case	KEY_DOWN:	//press S3 to reset all bits.
				xEventGroupClearBits(myEvent01Handle,BITMASK_KEY_LEFT | BITMASK_KEY_RIGHT);	//两个事件位都清零

			default:
				break;
		}

		if (keyCode == KEY_NONE)
			vTaskDelay(50);		//未按下任何按键，延时不能太长，否则按键响应慢
		else
			vTaskDelay(300);	//消除按键后抖动影响，也用于事件调度
	}
  /* USER CODE END AppTask_ScanKeys */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart3,(uint8_t*)&ch,1,0xFFFF);
	return ch;
}
/* USER CODE END Application */

