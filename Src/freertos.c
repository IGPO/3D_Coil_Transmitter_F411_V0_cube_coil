/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "usbd_custom_hid_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern uint8_t dataToReceive[64];
uint8_t dataToSend_TRND[USB_HID_DATA_TO_HOST_LENGTH + 1], *p1;
uint8_t USB_HID_dataToSend[64];
extern USBD_HandleTypeDef hUsbDeviceFS;
 uint8_t flag;
 uint8_t usb_hid_rec_mes;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myLedTaskHandle;
osThreadId myUSBRecTasHandle;
osThreadId myUSBTrTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartLedTask(void const * argument);
void StartUSBReceiveTask(void const * argument);
void StartUSBTransmitTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myLedTask */
  osThreadDef(myLedTask, StartLedTask, osPriorityIdle, 0, 128);
  myLedTaskHandle = osThreadCreate(osThread(myLedTask), NULL);

  /* definition and creation of myUSBRecTas */
  osThreadDef(myUSBRecTas, StartUSBReceiveTask, osPriorityNormal, 0, 128);
  myUSBRecTasHandle = osThreadCreate(osThread(myUSBRecTas), NULL);

  /* definition and creation of myUSBTrTask */
  osThreadDef(myUSBTrTask, StartUSBTransmitTask, osPriorityNormal, 0, 128);
  myUSBTrTaskHandle = osThreadCreate(osThread(myUSBTrTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartLedTask */
/**
* @brief Function implementing the myLedTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLedTask */
void StartLedTask(void const * argument)
{
  /* USER CODE BEGIN StartLedTask */
  /* Infinite loop */
  for(;;)
  {
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    osDelay(1000);
  }
  /* USER CODE END StartLedTask */
}

/* USER CODE BEGIN Header_StartUSBReceiveTask */
/**
* @brief Function implementing the myUSBRecTas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUSBReceiveTask */
void StartUSBReceiveTask(void const * argument)
{
  /* USER CODE BEGIN StartUSBReceiveTask */
  /* Infinite loop */
  for(;;)
  {
						if(dataToReceive[2] == 1) usb_hid_rec_mes |= 1 << 0 ;
		else usb_hid_rec_mes &= ~(1 << 0);
				if(dataToReceive[3] == 1) usb_hid_rec_mes |= 1 << 1 ;
		else usb_hid_rec_mes &= ~(1 << 1);
				if(dataToReceive[4] == 1) usb_hid_rec_mes |= 1 << 2 ;
		else usb_hid_rec_mes &= ~(1 << 2);
				if(dataToReceive[5] == 1) usb_hid_rec_mes |= 1 << 3 ;
		else {usb_hid_rec_mes &= ~(1 << 3);
		flag = 1;}
    osDelay(100);
  }
  /* USER CODE END StartUSBReceiveTask */
}

/* USER CODE BEGIN Header_StartUSBTransmitTask */
/**
* @brief Function implementing the myUSBTrTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUSBTransmitTask */
void StartUSBTransmitTask(void const * argument)
{
  /* USER CODE BEGIN StartUSBTransmitTask */
  /* Infinite loop */
  for(;;)
  {
				if(flag)	{
					flag = 0;
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, USB_HID_dataToSend, sizeof(USB_HID_dataToSend));}
    osDelay(100);
  }
  /* USER CODE END StartUSBTransmitTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
