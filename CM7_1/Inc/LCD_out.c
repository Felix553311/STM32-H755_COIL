
/*******************************************************************************
 * @file           : LCD_out.c
 * @date		   : May 27, 2025
 * @author         : Felix Bahrke
 * @brief          : contains initializing of the LCD for output and sending rough data to ILI9341
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 by the author.
 * All rights reserved.
 *
 * This file is part of the intellectual property of the owner.
 * The replication ensued under caveate of non commercial use.
 * The owner takes no liability for any damage cause by using content of this file.
 *
 ******************************************************************************
 */
#include <stm32h7xx.h>
#include <stdint.h>
#include <math.h>

#include <board.h>
#include <board_functions.h>
#include <LCD_out.h>




#define LCD_RD   GPIO_ODR_OD6 //Serves as read signal/MCU read data at the rising edge. Pg11 Datasheet
#define LCD_WR   GPIO_ODR_OD5 //Serves as write signal/command at the rising edge
#define LCD_RS   GPIO_ODR_OD2 //D/CX (0=Command/1=Data)
#define LCD_CS   GPIO_ODR_OD4 //Chip Select Pin : Active Low
#define LCD_RST  GPIO_ODR_OD3 //Shield Reset



 void LCD_write(const uint8_t data){ //
	GPIOE->ODR &= ~LCD_WR; // WR = 0

    //DATA Pins                              0| 1| 2| 3| 4| 5| 6| 7
	//										D0|D1|D2|D3|D4|D5|D6|D7
	GPIOD->ODR = data;					// use "bitbanging"

    GPIOE->ODR |= LCD_WR; // send data after (rising edge)

}







void LCD_Init(void){
 	//Reset TFT
 	GPIOE->ODR |= LCD_RST; //active low
 	delay(5);
 	GPIOE->ODR &= ~LCD_RST; //reset TFT
 	delay(15);
 	GPIOE->ODR |= LCD_RST; // turn off rst
 	delay(15);

 	//make right lvls for write
 	GPIOE->ODR |= LCD_CS; //Chip-Select Active Low Signal
 	GPIOE->ODR |= LCD_WR;
 	delay_u(20);
 	GPIOE->ODR |= LCD_RD;
 	GPIOE->ODR &= ~LCD_CS;  //cs low to actv ears of controller

 	LCD_command(0xC5);    //VCOM Control 1 : Colour Contrast Maybe;      need to test more
 	LCD_dataw(0x54);       //VCOM H 1111111 0x7F
 	LCD_dataw(0x00);       //VCOM L 0000000

 	LCD_command(0x36);    //Memory Access Control  r/w scan direction and RGB/BGR color set p. 127
 	LCD_dataw(0X48);		// 0x0 rst -> BGR on -> 0x8 RGB on   &  upside down screen
 	LCD_command(0x3A);    //COLMOD: Pixel Format Set  p. 134
 	LCD_dataw(0x55);       //set 16 bit RGB and MCU controlled
 	LCD_command(0x11);    //sleep out  p. 245
 	delay(10);                  //wait at least 5ms after sleepout
 	LCD_command(0x29);    //Display on.
 	LCD_command(0x2c);    //Memory Write | DataSheet Page 245
 //	LCD_command_write(0x51);    // display brightness
 	//LCD_data_write(0xFF);       // brightness = 50%

 }
