/**************************************************************************//**
* @file      main.h
* @brief     Main file header
* @author    Adi
* @date      2023-12-31

******************************************************************************/
#ifndef MAIN_H_
#define MAIN_H_

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Defines
******************************************************************************/
#define LEDBLINK_TASK	0
#define QUEUE_TASK		1

#define CURRENT_TASK	QUEUE_TASK
#define QUEUE_LENGTH	20
/******************************************************************************
* Variables
******************************************************************************/
QueueHandle_t LEDQueue;
/******************************************************************************
* Function Prototypes
******************************************************************************/
void vApplicationIdleHook(void);
void StartTasks(void);
void vApplicationDaemonTaskStartupHook(void);
void vApplicationStackOverflowHook(void);
void vApplicationMallocFailedHook(void);
void vApplicationTickHook(void);
void LEDTask1(void * parameter);
void LEDTask2(void * parameter);

void LED_Task(void * parameter);
BaseType_t CreateTasks(void);

#endif /* MAIN_H_ */








