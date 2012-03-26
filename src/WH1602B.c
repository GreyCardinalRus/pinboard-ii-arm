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

void Lcd_write_str(uc8 *STRING) {
	char c; //символ из строки
	while (c = *STRING++) {
		if (c >= 192)
			Lcd_write_data(russian[c - 192]);
		else
			Lcd_write_data(c);
	}
}

void Lcd_goto(uc8 x, uc8 y) {
	int str;
	str = y + 0x80;
	if(x == 1)
	{
		str+= 0x40;
	}
	Lcd_write_cmd(str);
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
	del = 240000;
	while (del--) {
	}
	rs_0;
	GPIO_Write(port, ((cmd >> 4) << lcd_shift));
	e_1;
	del = 100;
	while (del--) {
	}
	e_0;
	GPIO_Write(port, (0x00) << lcd_shift);
	GPIO_Write(port, ((cmd & 0x0F) << lcd_shift));
	del = 100;
	while (del--) {
	}
	e_1;
	del = 100;
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
	del = 100;
	while (del--) {
	}
	e_0;
	GPIO_Write(port, (0x00) << lcd_shift);
	GPIO_Write(port, ((data & 0x0F) << lcd_shift));
	del = 100;
	while (del--) {
	}
	e_1;
	rs_1;
	del = 100;
	while (del--) {
	}
	e_0;
	rs_0;
	rw_0;
	GPIO_Write(port, (0x00) << lcd_shift);
}

void Init_lcd() {
	Init_pin_out();

	del = 720000;
	while (del--) {
	}
	Lcd_write_cmd(Function_set);
	del = 720000;
	while (del--) {
	}

	Lcd_write_cmd(Display_on_off_control);
	del = 720000;
	while (del--) {
	}

	Lcd_write_cmd(Display_clear);
	del = 720000;
	while (del--) {
	}

	Lcd_write_cmd(Entry_mode_set);

}

void Lcd_clear() {
	Lcd_write_cmd(Display_clear);
	del = 1000;
	while (del--) {
	};
}

void Return_home() {
	Lcd_write_cmd(0b0000001);
}
