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
#include "main.h"
#include "FreeRTOS.h"
#include "SerialConsole/dUART.h"

/******************************************************************************
* Forward Declarations
******************************************************************************/

/******************************************************************************
* Variables
******************************************************************************/

/******************************************************************************
* Function Implementations
******************************************************************************/
#if (CURRENT_TASK == LEDBLINK_TASK)
void LEDTask1(void * parameter) {
	while(1) {
		port_pin_set_output_level(LED_0_PIN, true);
		vTaskDelay(500/portTICK_PERIOD_MS);
		port_pin_set_output_level(LED_0_PIN, false);
		vTaskDelay(500/portTICK_PERIOD_MS);
	}
}

void LEDTask2(void * parameter) {
	while(1) {
		port_pin_set_output_level(LED_0_PIN, true);
		vTaskDelay(300/portTICK_PERIOD_MS);
		port_pin_set_output_level(LED_0_PIN, false);
		vTaskDelay(300/portTICK_PERIOD_MS);
	}
}
#endif

BaseType_t CreateTasks(void) {
	BaseType_t xReturn;
#if (CURRENT_TASK == LEDBLINK_TASK)
	xReturn = xTaskCreate(LEDTask1,
						"LED Task 1",
						130,
						NULL,
						1,
						NULL);
	
	xReturn = xTaskCreate(LEDTask2,
						"LED Task2",
						130,
						NULL,
						1,
						NULL);
#endif

#if (CURRENT_TASK == QUEUE_TASK)
	xReturn = xTaskCreate(dUART_Task,
				"UART Task",
				130,
				NULL,
				1,
				NULL);

#endif
	return xReturn;
}

int main (void)
{
	system_init();
	
	/* Initialize the UART console. */
	dUART_Initialize();
	
	dUART_WriteString("Hello World\r\n");

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
