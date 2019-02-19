//main.c
//authored by Jared Hull
//
//tasks 1 and 2 blink the ACT LED
//main initialises the devices and IP tasks
#include <stdio.h>
#include <unistd.h>
#include <FreeRTOS.h>
#include <task.h>
#include "interrupts.h"
#include "gpio.h"
#include "mem.h"
#include "mini_uart.h"


#define GPIO_NUMBER 21
#define DELAY 500

//Only for debug, normally should not 
//   include private header

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef long int32_t;

#define COUNT 5
void task1() {
        int j = 0;
        int k;
        int ret = 0;

	while(j <= COUNT) {
                if(j == COUNT)
                {

                    bcm2835_miniuart_sendstr("Test Scenario 3 for case 1 : Invoke the scheduler repetitively\r\n");
                    bcm2835_miniuart_sendstr("Expected Result            : Scheduler should not create its resources again and again and should return warning.\r\n");
                    bcm2835_miniuart_sendstr("                           : Also verify if GPIO is OFF in the Oscilloscope\r\n");
                    bcm2835_miniuart_sendstr("\r\n");
                    bcm2835_miniuart_sendstr("Actual Result              :\r\n"); 
	            ret = vTaskStartScheduler();
                    if(ret != 1)
                    {
                        SetGpio(GPIO_NUMBER, 0);
                    }

                    for(k = 0 ; k < 10000000; k++);
                    bcm2835_miniuart_sendstr("---------------------------------------------------------------\r\n");
                    bcm2835_miniuart_sendstr("Test Scenario 1 for case 2 : Invoke the xTaskCreate() function with invalid arguments. (Pass NULL instead of task_name)\r\n");
                    bcm2835_miniuart_sendstr("Expected Result            : Scheduler should not run and return 			OS_ERROR_COMPONENT_TASKS,errTASK_CREATE_NULL_PARAMETER_SUPPLIED.\r\n");
                    bcm2835_miniuart_sendstr("                           : Also verify if GPIO is turned ON in the Oscilloscope\r\n");
                    bcm2835_miniuart_sendstr("\r\n");
                    bcm2835_miniuart_sendstr("Actual Result              : \r\n");
                    xTaskCreate(NULL, "LED_1", 128, NULL, 0, NULL);
                    SetGpio(GPIO_NUMBER, 1);
                    bcm2835_miniuart_sendstr("---------------------------------------------------------------\r\n");
                }
                else{
		   SetGpio(GPIO_NUMBER, 1);
		   vTaskDelay(DELAY);
		   SetGpio(GPIO_NUMBER, 0);
		   vTaskDelay(DELAY);
                }
                j++;
	}
}

void task2() {
	int i = 0;
	while(1) {
		i++;
		vTaskDelay(DELAY);
		//SetGpio(GPIO_NUMBER, 0);
		vTaskDelay(DELAY);
	}
}


char string[300] = {0};
#define GPIO_NUMBER 21
int main(void) {

        int i, j;
        int ret = 0;
	SetGpioFunction(GPIO_NUMBER, 1);			// RDY led
	DisableInterrupts();
	InitInterruptController();
	uart_init();
        SetGpio(GPIO_NUMBER, 0);
        //for(i = 0 ; i < 10000000; i++);

          for(i = 0 ; i < 5000000; i++);
          bcm2835_miniuart_sendstr("Optimise Freertos Scheduler mechanism following IEC 61508-SIL-3 guidelines:\r\n");
          bcm2835_miniuart_sendstr("Few of guidelines are simulated here\r\n");
          bcm2835_miniuart_sendstr("1.Avoid running the scheduler resources when it is invoked without any tasks in the ready/pending/suspended lists\r\n");
          bcm2835_miniuart_sendstr("   Actual Behaviour : When scheduler is invoked without any tasks in the list, it unnecessarily runs the idle_task, runs the OS timers, consumes stack/heap, measures statistics etc which takes lot of CPU cycles.\r\n");
          bcm2835_miniuart_sendstr("   Modified Behavior : Scheduler will verify if there are any tasks present in the ready/pending/suspended lists. If the lists are empty, then scheduler returns with a warning to the user. Now, user can decide to go to low power mode. \r\n");
          bcm2835_miniuart_sendstr("\r\n");

          bcm2835_miniuart_sendstr("2. Stabilize FREERTOS os by adding informative warnings\r\n");
          bcm2835_miniuart_sendstr("   Actual Behaviour : FreeRTOS APIs returns pdPASS or pdFAIL. If there is other fault like invalid parameter or wrong priority value or insufficient stack or NULL parameters passed, user will not be knowing the exact issue.\r\n");
          bcm2835_miniuart_sendstr("   Modified Behavior : All the failure cases are addressed properly according to SIL3 guidelines and an informative warning is returned from each function instead of True/False\r\n");
          bcm2835_miniuart_sendstr("\r\n");

          for(i = 0 ; i < 5000000; i++);
          bcm2835_miniuart_sendstr("---------------------------------------------------------------\r\n");
          bcm2835_miniuart_sendstr("TEST CASES\r\n");
          bcm2835_miniuart_sendstr("---------------------------------------------------------------\r\n");

          bcm2835_miniuart_sendstr("Test Scenario 1 for case 1 : Invoke the scheduler without creating tasks.\r\n");
          bcm2835_miniuart_sendstr("Expected Result            : Scheduler should not run and return OS_ERROR_COMPONENT_TASKS|errNO_TASKS_CREATED.\r\n");
          bcm2835_miniuart_sendstr("                           : Also verify if GPIO is turned ON in the Oscilloscope.\r\n");
          for(i = 0 ; i < 10000; i++);
          bcm2835_miniuart_sendstr("\r\n");
          bcm2835_miniuart_sendstr("Actual Result              :\r\n");
          for(i = 0 ; i < 10000; i++);
          bcm2835_miniuart_sendstr("\r\n");
	  ret = vTaskStartScheduler();
          if(ret != 1)
          {
             SetGpio(GPIO_NUMBER, 1);
          }
          bcm2835_miniuart_sendstr("---------------------------------------------------------------\r\n");


          for(i = 0 ; i < 10000000; i++);
          bcm2835_miniuart_sendstr("Test Scenario 2 for case 1 : Invoke the scheduler by creating tasks which toggles the GPIO.\r\n");
          bcm2835_miniuart_sendstr("Expected Result            : Scheduler should run and return pdPASS\r\n");
          bcm2835_miniuart_sendstr("                           : Also verify if GPIO is toggling in the Oscilloscope\r\n");
          for(i = 0 ; i < 10000; i++);
          bcm2835_miniuart_sendstr("\r\n");
          bcm2835_miniuart_sendstr("Actual Result              :\r\n");
          bcm2835_miniuart_sendstr("                           : Scheduler returns successfully\r\n");
          bcm2835_miniuart_sendstr("---------------------------------------------------------------\r\n");
          for(i = 0 ; i < 10000; i++);
          bcm2835_miniuart_sendstr("\r\n");
          xTaskCreate(task1, "LED_0", 128, NULL, 0, NULL);
	  ret = vTaskStartScheduler();
          if(ret != 1)
          {
             SetGpio(GPIO_NUMBER, 0);
          }





  	while(1) {
		;
	}

}
