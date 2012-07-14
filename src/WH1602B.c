/*
 * WH1602B.c
 *
 *  Created on: 26.03.2012
 *      Author: valentin
 */

#include "WH1602B.h"
const unsigned char russian[] = { 0x41, 0xA0, 0x42, 0xA1, 0xE0, 0x45, 0xA3,
		0xA4, 0xA5, 0xA6, 0x4B, 0xA7, 0x4D, 0x48, 0x4F, 0xA8, 0x50, 0x43, 0x54,
		0xA9, 0xAA, 0x58, 0xE1, 0xAB, 0xAC, 0xE2, 0xAD, 0xAE, 0x62, 0xAF, 0xB0,
		0xB1, 0x61, 0xB2, 0xB3, 0xB4, 0xE3, 0x65, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA,
		0xBB, 0xBC, 0xBD, 0x6F, 0xBE, 0x70, 0x63, 0xBF, 0x79, 0xE4, 0x78, 0xE5,
		0xC0, 0xC1, 0xE6, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7 };

const uint8_t lcd_2x16_decode[] =
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E',
		'F' };

enum
{
	LCD_2X16_RESET = 0x30, // Команда сброса ЖКД
	LCD_2X16_4_BIT_BUS = 0x20, // 4-х разрядная шина данных
	LCD_2X16_8_BIT_BUS = 0x30, // 8-и разрядная шина данных
	LCD_2X16_LINE_ONE = 0x20, // Одна строка
	LCD_2X16_LINES_TWO = 0x28, // Две строки
	LCD_2X16_FONT_5X8 = 0x20, // Шрифт 5x8 точек
	LCD_2X16_FONT_5X10 = 0x24, // Шрифт 5x10 точек
	LCD_2X16_DISPLAY_CLEAR = 0x01, // Очистка дисплея
	LCD_2X16_DISPLAY_HOME = 0x02, // Установка нулевой позиции курсора и дисплея
	LCD_2X16_DISPLAY_ON = 0x0C, // Включить дисплей. Комб. с CURSOR_ON и CURSOR_BLINK
	LCD_2X16_DISPLAY_RIGHT = 0x1C, // Сдвинуть дисплей вправо
	LCD_2X16_DISPLAY_LEFT = 0x18, // Сдвинуть дисплей влево
	LCD_2X16_DISPLAY_SHIFT = 0x05, // Комб. с SHIFTDIR_RIGHT и SHIFTDIR_LEFT
	LCD_2X16_CURSOR_ON = 0x0A, // Включить курсор. Комб. с DISPLAY_ON и CURSOR_BLINK
	LCD_2X16_CURSOR_BLINK = 0x09, // Комб. с DISPLAY_ON и CURSOR_ON
	LCD_2X16_CURSOR_RIGHT = 0x14, // Сдвинуть курсор вправо
	LCD_2X16_CURSOR_LEFT = 0x10, // Сдвинуть курсор влево
	LCD_2X16_SHIFT_RIGHT = 0x06, // Комб. с DISPLAY_SHIFT
	LCD_2X16_SHIFT_LEFT = 0x04, // Комб. с DISPLAY_SHIFT
	LCD_2X16_CGRAM_ADDR = 0x40, // Установка адреса CGRAM
	LCD_2X16_DDRAM_ADDR = 0x80, // Установка адреса DDRAM
	LCD_2X16_BUSY_FLAG = 0x80, // Флаг занятости
	LCD_2X16_NEXT_LINE = 0x40, // Адрес новой строки
	LCD_2X16_COMMAND = 0x01,
	LCD_2X16_DATA = 0x00,
} lcd_2x16_command;

void Lcd_write_str(uc8 *STRING) {
	char c; //символ из строки
	while (c = *STRING++) {
		if (c >= 192)
			Lcd_write_data(russian[c - 192]);
		else
			Lcd_write_data(c);
	}
}

void lcd_2x16_print_char(uint8_t data)
{
	if (data == '\n')
	{
		//lcd_2x16_write_command(LCD_2X16_DDRAM_ADDR | LCD_2X16_NEXT_LINE);
	}
	else
	{
		Lcd_write_data(data);
	}
}
void lcd_2x16_print_dec_xxx(uint8_t data)
{
	Lcd_write_data(lcd_2x16_decode[(data / 100) & 0x0F]);
	Lcd_write_data(lcd_2x16_decode[((data % 100) / 10) & 0x0F]);
	Lcd_write_data(lcd_2x16_decode[((data % 100) % 10) & 0x0F]);
}

//-----------------------------------------------------------------------------
void lcd_2x16_print_dec_xx(uint8_t data)
{
	Lcd_write_data(lcd_2x16_decode[((data % 100) / 10) & 0x0F]);
	Lcd_write_data(lcd_2x16_decode[((data % 100) % 10) & 0x0F]);
}

//void Lcd_goto(uc8 x, uc8 y) {
//	int str;
//	str = y + 0x80;
//	if(x == 1)
//	{
//		str+= 0x40;
//	}
//	Lcd_write_cmd(str);
//}
void Lcd_goto(uint8_t row, uint8_t colum)
{
	//Lcd_write_cmd((uint8_t) (0x80 | (row > 0 ? 0x40 : 0) | colum));
	uint8_t position = LCD_2X16_DDRAM_ADDR;

	if (row > 0)
	{
		position |= LCD_2X16_NEXT_LINE;
	}

	Lcd_write_cmd(position | colum);

}

void Init_pin_out() {
	RCC_APB2PeriphClockCmd(init_port, ENABLE);
	GPIO_InitTypeDef init_pin;
	init_pin.GPIO_Pin = pin_e | pin_rs | pin_rw | pin_d7 | pin_d6 | pin_d5
			| pin_d4;
	init_pin.GPIO_Mode = GPIO_Mode_Out_PP;
	init_pin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(port, &init_pin);
}

void Init_pin_in() {
	RCC_APB2PeriphClockCmd(init_port, ENABLE);
	GPIO_InitTypeDef init_pin;
	init_pin.GPIO_Pin = pin_d7 | pin_d6 | pin_d5 | pin_d4;
	init_pin.GPIO_Mode = GPIO_Mode_IPD;
	init_pin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(port, &init_pin);
}

void Lcd_write_cmd(uc8 cmd) {
	Init_pin_out();
	del = 24000;
	while (del--) {
	}
	rs_0;
	GPIO_Write(port, ((cmd >> 4) << lcd_shift));
	e_1;
	del = 1000;
	while (del--) {
	}
	e_0;
	GPIO_Write(port, (0x00) << lcd_shift);
	GPIO_Write(port, ((cmd & 0x0F) << lcd_shift));
	del = 1000;
	while (del--) {
	}
	e_1;
	del = 1000;
	while (del--) {
	}
	e_0;
	rs_0;
	rw_0;
}

void Lcd_write_data(uint8_t data) {
	Init_pin_out();
	GPIO_Write(port, ((data >> 4) << lcd_shift));
	e_1;
	rs_1;
	del = 1000;
	while (del--) {
	}
	e_0;
	GPIO_Write(port, (0x00) << lcd_shift);
	GPIO_Write(port, ((data & 0x0F) << lcd_shift));
	del = 1000;
	while (del--) {
	}
	e_1;
	rs_1;
	del = 1000;
	while (del--) {
	}
	e_0;
	rs_0;
	rw_0;
	GPIO_Write(port, (0x00) << lcd_shift);
}

void Init_lcd() {
	Init_pin_out();

	del = 7200;
	while (del--) {
	}
	Lcd_write_cmd(Function_set);
	del = 7200;
	while (del--) {
	}

	Lcd_write_cmd(Display_on_off_control);
	del = 7200;
	while (del--) {
	}

	Lcd_write_cmd(Display_clear);
	delay_ms(72);

	Lcd_write_cmd(Entry_mode_set);

}

void Lcd_clear() {
	Lcd_write_cmd(Display_clear);
	delay_ms(110);
}

void Return_home() {
	Lcd_write_cmd(0b0000001);
}
