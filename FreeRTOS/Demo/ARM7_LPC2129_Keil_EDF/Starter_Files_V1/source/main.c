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




//-------------------------------------------------
// Global Variables
//-------------------------------------------------

/***********************************************************************************************************************************************************
                                       Dead lines = Periodicity
***********************************************************************************************************************************************************/

#define A_PERIOD 10 //task A period
#define B_PERIOD 15 //task B period
#define C_PERIOD 20 //task C period
#define D_PERIOD 25 //task D period



int time_1_in=0,time_1_out=0,time_1_total=0;       //Task A 
int time_2_in=0,time_2_out=0,time_2_total=0;       //Task B
int time_3_in=0,time_3_out=0,time_3_total=0;       //Task C
int time_4_in=0,time_4_out=0,time_4_total=0;       //Task D  
int time_system=0 , cpu_load=0,time_tasks=0;


	
/* Perform the hardware setup required.  This is minimal as most of the
setup is managed by the settings in the project file. */
static void prvSetupHardware( void );


TaskHandle_t task_A_Handler = NULL;
TaskHandle_t task_B_Handler = NULL;
TaskHandle_t task_C_Handler = NULL;
TaskHandle_t task_D_Handler = NULL;


void Task_A (void* pvParameters)
{
	 int i=0;
   TickType_t xLastWakeTimeA = xTaskGetTickCount();
	 vTaskSetApplicationTaskTag(NULL,(void *) 1);
	while(1){
		 for(i=0;i<10000;i++){
			 i=i+0;   //just load
			 i=i+0;
			 i=i+0;
			 i=i+0;
		 }
		 vTaskDelayUntil( &xLastWakeTimeA, A_PERIOD);
  }
}


/******Task Two*********/
void Task_B (void* pvParameters)
{

	 int i=0;
   TickType_t xLastWakeTimeB = xTaskGetTickCount();
	 vTaskSetApplicationTaskTag(NULL,(void *) 2);
	while(1){
		 for(i=0;i<1000;i++){
			 i=i+0;   //just load
			 i=i+0;
			 i=i+0;
			 i=i+0;
		 }
		 vTaskDelayUntil( &xLastWakeTimeB, B_PERIOD);
  }
}


void Task_C (void* pvParameters)
{

   int i=0;
   TickType_t xLastWakeTimeC = xTaskGetTickCount();
	 vTaskSetApplicationTaskTag(NULL,(void *) 3);
	while(1){
		 for(i=0;i<1000;i++){
			 i=i+0;   //just load
			 i=i+0;
			 i=i+0;
			 i=i+0;
		 }
		 vTaskDelayUntil( &xLastWakeTimeC, C_PERIOD);
  }
}

void Task_D (void* pvParameters)
{
 
	 int i=0;
   TickType_t xLastWakeTimeD = xTaskGetTickCount();
	 vTaskSetApplicationTaskTag(NULL,(void *) 4);	
	while(1){
		 for(i=0;i<10000;i++){
			 i=i+0;   //just load
			 i=i+0;
			 i=i+0;
			 i=i+0;
		 }
		 vTaskDelayUntil( &xLastWakeTimeD, D_PERIOD);
  }
}




/*implement idle hook function*/

void vApplicationIdleHook( void ){
 
}

/*implement tick hook function*/

void vApplicationTickHook( void ){
	GPIO_write(PORT_0,PIN1,PIN_IS_HIGH);
	GPIO_write(PORT_0,PIN1,PIN_IS_LOW);
}

													
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

	//tasks creation 
/********************************************************************************************************************************
                                          Task A
********************************************************************************************************************************/	
	xTaskPeriodicCreate(
	             Task_A,     //Function that implement the task
	             "Task A",   //Text Name for the Task
	             100,      //stack size in words
							(void *) 0,   //parameters for this task 
							1,            //Task Priority
							&task_A_Handler,
							A_PERIOD);
	
/********************************************************************************************************************************
                                          Task B
********************************************************************************************************************************/
							
	xTaskPeriodicCreate(
	             Task_B,     //Function that implement the task
	             "Task B",   //Text Name for the Task
	             100,      //stack size in words
							(void *) 0,   //parameters for this task 
							1,            //Task Priority
							&task_B_Handler,
							B_PERIOD);							
							
/********************************************************************************************************************************
                                          Task C
********************************************************************************************************************************/

	xTaskPeriodicCreate(
	             Task_C,     //Function that implement the task
	             "Task C",   //Text Name for the Task
	             100,      //stack size in words
							(void *) 0,   //parameters for this task 
							1,            //Task Priority
							&task_C_Handler,
							C_PERIOD);														

/********************************************************************************************************************************
                                          Task D
********************************************************************************************************************************/							
			
	xTaskPeriodicCreate(
	             Task_D,     //Function that implement the task
	             "Task D",   //Text Name for the Task
	             100,      //stack size in words
							(void *) 0,   //parameters for this task 
							1,            //Task Priority
							&task_D_Handler,
							D_PERIOD);

							
							
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
	configTimer1();
	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/

				

