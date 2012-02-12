/* Standard includes. */
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

/* variables used by tasks */
volatile int32_t ITM_RxBuffer;

//--------------------------------------------------------------

void vTaskLED1(void *pvParameters) {

	for (;;) {
		GPIO_SetBits(GPIOF, GPIO_Pin_6);
		vTaskDelay(500);
		GPIO_ResetBits(GPIOF, GPIO_Pin_6);
		vTaskDelay(500);
	}

}

void vTaskLED2(void *pvParameters) {

	for (;;) {
		GPIO_SetBits(GPIOF, GPIO_Pin_7);
		vTaskDelay(321);
		GPIO_ResetBits(GPIOF, GPIO_Pin_7);
		vTaskDelay(321);
	}

}


//--------------------------------------------------------------
int main(void) {

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	/**
	 *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8
			| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	xTaskCreate( vTaskLED1, ( signed char * ) "LED1", configMINIMAL_STACK_SIZE, NULL, 2,
			( xTaskHandle * ) NULL);
	xTaskCreate( vTaskLED2, ( signed char * ) "LED2", configMINIMAL_STACK_SIZE, NULL, 2,
			( xTaskHandle * ) NULL);

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
