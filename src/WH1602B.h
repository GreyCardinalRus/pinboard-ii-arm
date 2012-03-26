/*
 * WH1602B.h
 *
 *  Created on: 26.03.2012
 *      Author: valentin
 */

#ifndef WH1602B_H_
#define WH1602B_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#define port            GPIOB
#define init_port       RCC_APB2Periph_GPIOB
#define pin_e           GPIO_Pin_12
#define pin_rw          GPIO_Pin_10
#define pin_rs          GPIO_Pin_11

#define lcd_shift       6                       //номер последнего бита в 4-битной шине
#define use_gpio        GPIO_Pin_9  // старший бит                                                      9
#define pin_d7          use_gpio    // старший бит                                                      9
#define pin_d6          use_gpio>>1 // следующий по убыванию бит                                           8
#define pin_d5          use_gpio>>2 // следующий по убыванию бит                                           7
#define pin_d4          use_gpio>>3 // следующий по убыванию бит (последний бит в 4-х битной шине)         6

#define Function_set                            0b00100000//4-bit,2 - line mode, 5*8 dots
#define Display_on_off_control   	      0b00001111/// display on,cursor off,blink off
#define Display_clear                           0b00000001
#define Entry_mode_set                          0b00000100//после 2 разряда слева. increment mode,entire shift off

//#define Lcd_goto(x,y) Lcd_write_cmd(((((y)& 1)*0x40)+((x)& 7))|128)


#define rs_1    port->ODR |=  pin_rs
#define rs_0    port->ODR &=~ pin_rs
#define e_1     port->ODR |=  pin_e
#define e_0             port->ODR &=~ pin_e
#define rw_1    port->ODR |=  pin_rw
#define rw_0    port->ODR &=~ pin_rw
u32 del;



void Init_pin_out(void);
void Init_pin_in(void);
void Init_lcd(void);
void Lcd_write_data(uint8_t byte);
void Lcd_write_cmd(uint8_t byte);
void Lcd_clear(void);
void Return_home(void);
void Lcd_goto(uc8 x, uc8 y);
void Lcd_write_str(uc8 *STRING);


#endif /* WH1602B_H_ */
