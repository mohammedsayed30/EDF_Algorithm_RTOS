/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <lpc21xx.h>
#include "GPIO.h"
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

extern int time_1_in,time_1_out,time_1_total;       //Task A 
extern int time_2_in,time_2_out,time_2_total;       //Task B
extern int time_3_in,time_3_out,time_3_total;       //Task C
extern int time_4_in,time_4_out,time_4_total;       //Task D  
extern int time_system ,cpu_load,time_tasks;


#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK			1
#define configUSE_TICK_HOOK			1
#define configCPU_CLOCK_HZ			( ( unsigned long ) 60000000 )	/* =12.0MHz xtal multiplied by 5 using the PLL. */
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES		( 4 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 90 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) 13 * 1024 )
#define configMAX_TASK_NAME_LEN		( 8 )
#define configUSE_TRACE_FACILITY	1
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		1
#define configUSE_TIME_SLICING    0                 //time slicing
#define configUSE_MUTEXES         0
#define configSUPPORT_DYNAMIC_ALLOCATION  1

//#define configUSE_CO_ROUTINES       0
//#define configMAX_CO_ROUTINES_PRIORITIES      (2)
#define configQUEUE_REGISTRY_SIZE 	0

//EDF Scheduler
#define configUSE_EDF_SCHEDULER  1

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1             
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

#define configUSE_APPLICATION_TASK_TAG  1

/*Trace*/ 


																				 
																				 
#define traceTASK_SWITCHED_OUT()        do\
                                         {\
																					if((int)pxCurrentTCB->pxTaskTag==0)\
                                          {\
                                           GPIO_write(PORT_0,PIN0,PIN_IS_LOW);\
																					}\
																					else if((int)pxCurrentTCB->pxTaskTag==1)\
                                          {\
                                           GPIO_write(PORT_0,PIN2,PIN_IS_LOW);\
																					 time_1_out=T1TC;\
                                           time_1_total+=(time_1_out-time_1_in);\
																					}\
																					else if((int)pxCurrentTCB->pxTaskTag==2)\
                                          {\
                                           GPIO_write(PORT_0,PIN3,PIN_IS_LOW);\
																					 time_2_out=T1TC;\
                                           time_2_total+=(time_2_out-time_2_in);\
																					}\
																					else if((int)pxCurrentTCB->pxTaskTag==3)\
                                          {\
                                           GPIO_write(PORT_0,PIN4,PIN_IS_LOW);\
																					 time_3_out=T1TC;\
                                           time_3_total+=(time_3_out-time_3_in);\
																					}\
																					else if((int)pxCurrentTCB->pxTaskTag==4)\
                                          {\
                                           GPIO_write(PORT_0,PIN5,PIN_IS_LOW);\
																					 time_4_out=T1TC;\
                                           time_4_total+=(time_4_out-time_4_in);\
																					}\
                                          time_system = T1TC;\
																					time_tasks = time_1_total+time_2_total+time_3_total+time_4_total;\
                                          cpu_load = ((time_tasks)/(time_system) * 100);\
																				 }while(0)	


#define traceTASK_SWITCHED_IN()         do\
                                         {\
																					if((int)pxCurrentTCB->pxTaskTag==0)\
                                          {\
                                           GPIO_write(PORT_0,PIN0,PIN_IS_HIGH);\
																					}\
																					else if((int)pxCurrentTCB->pxTaskTag==1)\
                                          {\
                                           GPIO_write(PORT_0,PIN2,PIN_IS_HIGH);\
																					 time_1_in=T1TC;\
																					}\
																					else if((int)pxCurrentTCB->pxTaskTag==2)\
                                          {\
                                           GPIO_write(PORT_0,PIN3,PIN_IS_HIGH);\
																					 time_2_in=T1TC;\
																					}\
																					else if((int)pxCurrentTCB->pxTaskTag==3)\
                                          {\
                                           GPIO_write(PORT_0,PIN4,PIN_IS_HIGH);\
																					 time_3_in=T1TC;\
																					}\
																					else if((int)pxCurrentTCB->pxTaskTag==4)\
                                          {\
                                           GPIO_write(PORT_0,PIN5,PIN_IS_HIGH);\
																					 time_4_in=T1TC;\
																					}\
																				 }while(0)																				 

//E.C. : MACROS

																	
/*config Run Time Stats*/
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
//#define configSUPPORT_DYNAMIC_ALLOCATION  1																	
#define configGENERATE_RUN_TIME_STATS   1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()																	
#define portGET_RUN_TIME_COUNTER_VALUE()   (T1TC)																	

#endif /* FREERTOS_CONFIG_H */
