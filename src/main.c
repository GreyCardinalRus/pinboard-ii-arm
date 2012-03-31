/* Standard includes. */
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rtc.h"
#include <stdio.h>
#include <string.h>
//#include "lcd.h"
#include "WH1602B.h"
//#include "adc.h"
//#include "pwm.h"
//#include "serial.h"

/*
 Используется в основном схема соединений применяемая в быстром старте для STM32 с некоторыми отличиями.
 Соединения для LCD индикатора:
 RS индикатора -> A08 модуля
 RW индикатора -> любой контакт GND
 E индикатора -> A11 модуля
 D4 индикатора -> B08 модуля
 D5 индикатора -> B09 модуля
 D6 индикатора -> B10 модуля
 D7 индикатора -> B11 модуля

 Два ШИМ светодиода LED3 и LED2
 Перемычка(джампер) B05 -> LED3 удаляется.
 Соединения:
 LED2 модуля -> A06 модуля
 LED3 модуля -> A07 модуля
 */

union times___ {
	uint64_t time_ll;
	volatile uint32_t time_l; // системная переменная
							  // обозначающая кол. миллисекунд прошедщих
							  // с начала действия программы
	uint16_t time_s;
} times__ = { 0ull };

//--------------------------------------------------------------
//uint16_t ocval1=333,ocval2=332; // состояние ШИМ светодиода LED2 и LED3
//uint8_t u_d1=0,u_d2=1;          // направление изменения 0-> яркость уменьшается
// 1 -> яркость увеличивается
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName) {
	(void) pcTaskName;
	(void) pxTask;

	/* Run time stack overflow checking is performed if
	 configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	 function is called if a stack overflow is detected. */
	for (;;)
		;
}
void vTaskTimer(void *pvParameters) {
	times__.time_ll++; // обновить системное время
//if ((times__.time_l%1000)==0) // есть ли начало секунды?
//{                             // да
//  one_sec_flg=1;              // установить запрос показания
//                              // времени на экране
//}
//if (d_timer > 0)              // уменьшить счетчик запрета индикации
//                              // времени на экране
//{
//  d_timer--;
//}
	vTaskDelay(1);
}

void vTaskLED01(void *pvParameters) {
	//pbii_PWM_TIM3_Init();               // инициализация подсистемы ШИМ светодиодов
	// установка начальных значений шима
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint16_t CCR1_Val = 333;
	uint16_t CCR2_Val = 249;
	uint16_t PrescalerValue = 0;

	// разрешить клокирование ШИМа
	/* System Clocks Configuration */
	/* TIM3 clock enable */ // разрешить клокирование TIM3 и альтернативного ввода-
							// вывода на порт B
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOA and GPIOB clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// инициализация TIM3 режиме ШИМ целиком выдрана из примеров к стандартной
	// библиотеке STM32  используются 2 из 4х каналов ШИМ
	/* -----------------------------------------------------------------------
	 TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
	 The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter
	 clock at 24 MHz the Prescaler is computed as following:
	 - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
	 SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
	 and Connectivity line devices and to 24 MHz for Low-Density Value line and
	 Medium-Density Value line devices

	 The TIM3 is running at 36 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
	 = 24 MHz / 666 = 36 KHz
	 TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
	 TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
	 ----------------------------------------------------------------------- */
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t)(SystemCoreClock / 24000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 665;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);

	pvParameters = pvParameters;
	uint16_t ocval1 = 333, ocval2 = 332; // состояние ШИМ светодиода LED2 и LED3
	uint8_t u_d1 = 0, u_d2 = 1; // направление изменения 0-> яркость уменьшается
								// 1 -> яркость увеличивается
	for (;;) {
		TIM_SetCompare1(TIM3, ocval1); // установить новое значение ШИМ LED2
		TIM_SetCompare2(TIM3, ocval2); // установить новое значение ШИМ LED3
		if (u_d1 == 0) // генерация нового значения ШИМ LED2
				{ // если направление изменения яркости
				  // уменьшается
			if (ocval1 > 1) // то, если счетчик ШИМ имеет не
							// минимальное значение
					{
				ocval1--; // уменьшить значение на единицу
			} else {
				u_d1 = 1; // иначе изменить направление
						  // изменения ШИМ на противоположное
			}
		} else { // если направление изменения яркости увеличивается
			if (ocval1 < 664) // и счетчик ШИМ не достиг максимальной
							  // яркости
					{
				ocval1++; // увеличить яркость
			} else
				u_d1 = 0; // если четчик достиг максимальной яркости
						  // изменить направление изменения яркости
						  // на противоположное
		}
		if (u_d2 == 0) // генерация нового значения ШИМ LED3
				{ // аналогично LED2
			if (ocval2 > 1) {
				ocval2--;
			} else {
				u_d2 = 1;
			}
		} else {
			if (ocval2 < 664) {
				ocval2++;
			} else
				u_d2 = 0;
		}
		vTaskDelay(1);
	}
}

void vTaskLED2(void *pvParameters) {
	pvParameters = pvParameters;
//	  GPIO_InitTypeDef  GPIO_InitStructure;
//	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
//	  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	  for (;;) {
//		GPIO_SetBits(GPIOB, GPIO_Pin_4);
//		vTaskDelay(700);
//		GPIO_ResetBits(GPIOB, GPIO_Pin_4);
//		vTaskDelay(700);
//	}

}

void vTaskLED3(void *pvParameters) {
	pvParameters = pvParameters;
	GPIO_InitTypeDef GPIO_InitStructure;
	//	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	unsigned short i = 0;
	for (;;) {
		for (i = 0; i < 200; i++) {
			//vTaskDelay(10);
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
			vTaskDelay(i / 50);
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);
			vTaskDelay(10 - i / 20);
		}
		for (i = 0; i < 200; i++) {
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);
			vTaskDelay(i / 20);
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
			vTaskDelay(10 - i / 20);

			//			GPIO_SetBits(GPIOB, GPIO_Pin_5);
//			vTaskDelay(8-i/10);
//			GPIO_ResetBits(GPIOB, GPIO_Pin_5);
//			vTaskDelay(i/10);
		}
	}

}

void vTaskLCD(void *pvParameters) {
	pvParameters = pvParameters;
//	lcd_init(); // инициализация подсистемы LCD
//	clrscr(); // очистка экрана
//	lcd_cursor_off(); // отключить мигание курсора
//	wr_lcd_str(0, 0, "Hello from PBII!"); // вывод приветствия, ипользуется API lcd.h
//

//	Init_lcd();
//	Lcd_clear();
//	Lcd_goto(0, 3);
//	Lcd_write_str("WH1602B");
//	Lcd_goto(1, 4);
//	Lcd_write_str("Test.");

	for (;;) {
	}
}
//--------------------------------------------------------------
void vTaskBZR(void *pvParameters) {
	pvParameters = pvParameters;
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
//	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  for (;;) {
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
		vTaskDelay(70);
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		vTaskDelay(70);
	}

}
int main(void) {
	//По сбросу на PB3,PB4, PA15 используются для отладки по jtag.

//	AFIO->MAPR |=AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
//	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
//	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
//
//	//и не забыть
//
//	EXTI->IMR &= ~EXTI_IMR_MR3;
//	EXTI->EMR &= ~EXTI_EMR_MR3;
//
//	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
//
//	AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI3_PB;
//	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI3_PB;
//
//	EXTI->RTSR |= EXTI_RTSR_TR3;
//	EXTI->FTSR |= EXTI_FTSR_TR3;
//
//	EXTI->IMR |= EXTI_IMR_MR3;

//	GPIO_InitTypeDef GPIO_InitStructureA, GPIO_InitStructureB;
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	/**
//	 *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
//	 */
//	GPIO_InitStructureA.GPIO_Pin = GPIO_Pin_15; // | GPIO_Pin_7 | GPIO_Pin_8	| GPIO_Pin_9;
//	GPIO_InitStructureA.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStructureA.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructureA);
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	/**
//	 *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
//	 */
//	GPIO_InitStructureB.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 ; //| GPIO_Pin_9;
//	GPIO_InitStructureB.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructureB.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructureB);
	rtc_init();
	xTaskCreate( vTaskTimer, ( signed char * ) "Timer",
			configMINIMAL_STACK_SIZE, NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate( vTaskLED01, ( signed char * ) "LED01",
			configMINIMAL_STACK_SIZE, NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate( vTaskLED2, ( signed char * ) "LED2", configMINIMAL_STACK_SIZE,
			NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate( vTaskLED3, ( signed char * ) "LED3", configMINIMAL_STACK_SIZE,
			NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate( vTaskLCD, ( signed char * ) "LCD", configMINIMAL_STACK_SIZE,
			NULL, 2, ( xTaskHandle * ) NULL);
//	xTaskCreate( vTaskBZR, ( signed char * ) "BZR", configMINIMAL_STACK_SIZE,
//			NULL, 2, ( xTaskHandle * ) NULL);
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
uint32_t gettime(void) // получить текущее значение времени
{
	return times__.time_l;
}

void wait_ms(uint32_t ms) // подпрограмма отработки задержки
// очень неточная, на малых значениях задержки
// погрешность может достигать 10%-50%
{
	uint32_t newtime = ms + gettime();
	while (gettime() < newtime)
		//while(newtime-- > 0)
		continue;
}
