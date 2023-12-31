/**************************************************************************//**
* @file      Main.c
* @brief     Main file
* @author    Adi
* @date      2023-12-31

******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
#include <asf.h>
#include "FreeRTOS.h"

/******************************************************************************
* Forward Declarations
******************************************************************************/
void vApplicationIdleHook(void);
void StartTasks(void);
void vApplicationDaemonTaskStartupHook(void);
void vApplicationStackOverflowHook(void);
void vApplicationMallocFailedHook(void);
void vApplicationTickHook(void);
void LEDTask(void * parameter);
BaseType_t CreateTasks(void);

/******************************************************************************
* Function Implementations
******************************************************************************/

void LEDTask(void * parameter) {
	while(1) {
		port_pin_set_output_level(LED_0_PIN, true);
		vTaskDelay(500/portTICK_PERIOD_MS);
		port_pin_set_output_level(LED_0_PIN, false);
		vTaskDelay(500/portTICK_PERIOD_MS);
	}
}

BaseType_t CreateTasks(void) {
	BaseType_t xReturn;
	
	xReturn = xTaskCreate(LEDTask,
	"LED Task",		/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
	130,
	NULL,
	1,
	NULL);
	
	return xReturn;
}

int main (void)
{
	system_init();

	CreateTasks();
				
	vTaskStartScheduler();
}

void vApplicationDaemonTaskStartupHook(void)
{
}

void vApplicationMallocFailedHook(void)
{
	while(1);
}

void vApplicationStackOverflowHook(void)
{
	while(1);
}

void vApplicationTickHook(void)
{
}
