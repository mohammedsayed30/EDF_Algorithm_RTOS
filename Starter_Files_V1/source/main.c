/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lpc21xx.h"
#include "semphr.h"
/* Peripheral includes. */
#include "main.h"
#include "serial.h"
#include "GPIO.h"

//to share between Tasks 
pinState_t ButtonState;
/* Perform the hardware setup required.  This is minimal as most of the
setup is managed by the settings in the project file. */
static void prvSetupHardware( void );


TaskHandle_t task_1_Handler = NULL;
TaskHandle_t task_2_Handler = NULL;
TaskHandle_t task_3_Handler = NULL;

SemaphoreHandle_t xSemaphore_1;
SemaphoreHandle_t xSemaphore_2;


void Task_1 (void* pvParameters)
{
	int i=0,j;
	unsigned int xLastWakeTime = xTaskGetTickCount();
	vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );
	
	for(;;)
	{
		xSemaphoreTake(xSemaphore_1,portMAX_DELAY);
		j=0;
		for(i=0;i<300000;i++){
			i=i;
			if(i>150000 && j==0){
				j=1;
				xSemaphoreTake(xSemaphore_2,portMAX_DELAY);
			}
		}
		xSemaphoreGive(xSemaphore_1);
		xSemaphoreGive(xSemaphore_2);
		vTaskDelayUntil(&xLastWakeTime,100);

	}
	
}

/******Task Two*********/
void Task_2 (void* pvParameters)
{
	int i=0;
	unsigned int xLastWakeTime = xTaskGetTickCount();
	vTaskSetApplicationTaskTag( NULL, ( void * ) 2 );
	for(;;)
	{
		
		for(i=0;i<60000;i++){
			i=i;
		}
		
		vTaskDelayUntil( &xLastWakeTime,120);

	}
	
}

/******Task Three*********/
void Task_3 (void* pvParameters)
{
	int i=0;
	unsigned int xLastWakeTime = xTaskGetTickCount();
	vTaskSetApplicationTaskTag( NULL, ( void * ) 3 );
	
	for(;;)
	{
		xSemaphoreTake(xSemaphore_1,portMAX_DELAY);
		xSemaphoreTake(xSemaphore_2,portMAX_DELAY);
		for(i=0;i<100000;i++){
			i=i;
		}
		xSemaphoreGive(xSemaphore_1);
		xSemaphoreGive(xSemaphore_2);
		vTaskDelayUntil( &xLastWakeTime,110);

	}
	
}


/*implement idle hook function*/

void vApplicationIdleHook( void ){

}

/*implement tick hook function*/

void vApplicationTickHook( void ){
	GPIO_write(PORT_0,PIN4,PIN_IS_HIGH);
	GPIO_write(PORT_0,PIN4,PIN_IS_LOW);
}

													
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	xSemaphore_1 = xSemaphoreCreateBinary();
	xSemaphore_2 = xSemaphoreCreateBinary();
	xSemaphoreGive(xSemaphore_1);
	xSemaphoreGive(xSemaphore_2);
	//task creation 
	xTaskCreate(
	             Task_1,     //Function that implement the task
	             "Task 1",   //Text Name for the Task
	             100,      //stack size in words
							(void *) 0,   //parameters for this task 
							1,            //Task Priority
							&task_1_Handler);
							
	xTaskCreate(
	             Task_2,     //Function that implement the task
	             "Task 2",   //Text Name for the Task
	             100,      //stack size in words
							(void *) 0,   //parameters for this task 
							2,            //Task Priority
							&task_2_Handler);							
							
	xTaskCreate(
	             Task_3,     //Function that implement the task
	             "Task 3",   //Text Name for the Task
	             100,      //stack size in words
							(void *) 0,   //parameters for this task 
							3,            //Task Priority
							&task_3_Handler);
	
	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/


static void timer1Reset(void){
	  T1TCR |=0x2;
    T1TCR &=0x1; 	
}

static void configTimer1(void){
	  T1PR =  1000;
	  T1TCR |=0x1; 
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	/*Timer configuration*/
	//configTimer1();
	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/

				

