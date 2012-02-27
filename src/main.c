/* Standard includes. */
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "FreeRTOS.h"
#include "task.h"

//--------------------------------------------------------------
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName) {
	(void) pcTaskName;
	(void) pxTask;

	/* Run time stack overflow checking is performed if
	 configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	 function is called if a stack overflow is detected. */
	for (;;)
		;
}

void vTaskLED0(void *pvParameters) {
	pvParameters = pvParameters;
	for (;;) {
		GPIO_SetBits(GPIOA, GPIO_Pin_15);
		vTaskDelay(500);
		GPIO_ResetBits(GPIOA, GPIO_Pin_15);
		vTaskDelay(500);
	}

}

void vTaskLED1(void *pvParameters) {
	pvParameters = pvParameters;
	for (;;) {
		GPIO_SetBits(GPIOB, GPIO_Pin_3);
		vTaskDelay(600);
		GPIO_ResetBits(GPIOB, GPIO_Pin_3);
		vTaskDelay(600);
	}
}
void vTaskLED2(void *pvParameters) {
	pvParameters = pvParameters;
	for (;;) {
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
		vTaskDelay(700);
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);
		vTaskDelay(700);
	}

}

void vTaskLED3(void *pvParameters) {
	pvParameters = pvParameters;
	for (;;) {
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		vTaskDelay(800);
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		vTaskDelay(800);
	}

}

//--------------------------------------------------------------
int main(void) {

	GPIO_InitTypeDef GPIO_InitStructureA, GPIO_InitStructureB;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/**
	 *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
	 */
	GPIO_InitStructureA.GPIO_Pin = GPIO_Pin_15; // | GPIO_Pin_7 | GPIO_Pin_8	| GPIO_Pin_9;
	GPIO_InitStructureA.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructureA.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructureA);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/**
	 *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
	 */
	GPIO_InitStructureB.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //| GPIO_Pin_9;
	GPIO_InitStructureB.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructureB.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructureB);

	xTaskCreate( vTaskLED0, ( signed char * ) "LED0", configMINIMAL_STACK_SIZE,
			NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate( vTaskLED1, ( signed char * ) "LED1", configMINIMAL_STACK_SIZE,
			NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate( vTaskLED2, ( signed char * ) "LED2", configMINIMAL_STACK_SIZE,
			NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate( vTaskLED3, ( signed char * ) "LED3", configMINIMAL_STACK_SIZE,
			NULL, 2, ( xTaskHandle * ) NULL);

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle
	 task.  The idle task is created within vTaskStartScheduler(). */
	for (;;)
		;
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void) {

//	TimingDelay_Decrement();
}
/*-----------------------------------------------------------*/
