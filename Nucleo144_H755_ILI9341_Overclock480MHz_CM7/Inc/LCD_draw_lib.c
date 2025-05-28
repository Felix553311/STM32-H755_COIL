/*
 * LCD_draw_lib.c
 *
 *  Created on: May 27, 2025
 *      Author: felix
 */

#include <stm32h7xx.h>
#include <stdint.h>
#include <math.h>

#include <board.h>
#include <board_functions.h>
#include <LCD_out.h>
#include <LCD_draw_lib.h>



#define LCD_RD   GPIO_ODR_OD6 //Serves as read signal/MCU read data at the rising edge. Pg11 Datasheet
#define LCD_WR   GPIO_ODR_OD5 //Serves as write signal/command at the rising edge
#define LCD_RS   GPIO_ODR_OD2 //D/C (0=Command/1=Data)
#define LCD_CS   GPIO_ODR_OD4 //Chip Select Pin : Active Low
#define LCD_RST  GPIO_ODR_OD3 //Shield Reset




void scr_area(uint16_t topcorn, uint16_t Area, uint16_t bottomcorn){
    LCD_command_write(0x33);
    LCD_data_write(topcorn >> 8);
    LCD_data_write(topcorn & 0xFF);
    LCD_data_write(Area >> 8);
    LCD_data_write(Area & 0xFF);
    LCD_data_write(bottomcorn >> 8);
    LCD_data_write(bottomcorn & 0xFF);
}





void LCD_Scroll(const uint16_t scr_with){ //shifting the starting RAM addr negative
	GPIOE->ODR &= ~LCD_CS;// Chip Select active
	delay_cycles(1);
	//define area where pixels will be changed/written
	LCD_command_write(0x2a);  //code for column address p 110
	LCD_data_write(0x0);  //upper 8 Bit sent to LCD       start addr
	LCD_data_write(0x0);       //lower 8 Bit sent to LCD
	LCD_data_write(0x0);  //
	LCD_data_write(0xF0);       // end addr

	LCD_command_write(0x2b);  //code for row address
	LCD_data_write(0x0);  //
	LCD_data_write(0x0);       //
	LCD_data_write(0x1);  //
	LCD_data_write(0x40);       //
	LCD_command_write(0x37);  // p. 129*/
//                                                      317|318|319|0|1
	LCD_data_write(scr_with >> 8); //                   318|319| 0 |1|2
	LCD_data_write(scr_with & 0xFF);//Start addr after shift-----^

}






void LCD_command_write(const uint8_t command){
  //  p. 11
  GPIOE->ODR &= ~LCD_RS;

  LCD_write(command);
}

void LCD_data_write(const uint8_t data){
  //  p. 11
	GPIOE->ODR |= LCD_RS;

  LCD_write(data);
}



void Pixel(uint16_t x1, uint16_t y1, uint16_t color){
	const uint16_t x2 = x1 + 1; const uint16_t y2 = y1 + 1;
	GPIOE->ODR &= ~LCD_CS;// Chip Select active
	//define area where pixels will be changed/written
	LCD_command_write(0x2a);  //code for column address p 110
	LCD_data_write(y1 >> 8);  //upper 8 Bit sent to LCD
	LCD_data_write(y1 & 0xFF);       //lower 8 Bit sent to LCD
	LCD_data_write(y2 >> 8);  // start addr
	LCD_data_write(y2 & 0xFF);       // end addr

	LCD_command_write(0x2b);  //code for row address
	LCD_data_write(x1 >> 8);  //
	LCD_data_write(x1 & 0xFF);       //
	LCD_data_write(x2 >> 8);  //
	LCD_data_write(x2 & 0xFF);       //

	LCD_command_write(0x2c);  // Memory Write
	LCD_data_write(color >> 8);
	LCD_data_write(color);

}


void Line(uint16_t x1, uint16_t y1, uint16_t y2, uint16_t color){
	const uint32_t pixels = (y2 - y1 + 1);
	const int a = 44;
	GPIOE->ODR &= ~LCD_CS;// Chip Select active
	//delay_cycles(1);
	//define area where pixels will be changed/written
	LCD_command_write(0x2a);  //code for column address p 110
	LCD_data_write(y1 >> 8);  //upper 8 Bit sent to LCD
	LCD_data_write(y1 & 0xFF);       //lower 8 Bit sent to LCD
	LCD_data_write(y2 >> 8);  // start addr
	LCD_data_write(y2 & 0xFF);       // end addr

	LCD_command_write(0x2b);  //code for row address
	LCD_data_write(x1 >> 8);  //
	LCD_data_write(x1 & 0xFF);       //
	LCD_data_write(x1 >> 8);  //
	LCD_data_write(x1 & 0xFF);       //

	LCD_command_write(0x2c);  // Memory Write
	for(int i = 0; i < pixels; i++){
		LCD_data_write(color >> 8);
		delay_cycles(a);
		LCD_data_write(color & 0xFF);
		delay_cycles(a);
	}
}

void Rect(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint16_t color){
	const uint32_t pixels = (x2 - x1 + 1) * (y2 - y1 + 1);
	GPIOE->ODR &= ~LCD_CS;// Chip Select active
	//delay_cycles(44);
	//define area where pixels will be changed/written
	LCD_command_write(0x2a);  //code for column address p 110
	LCD_data_write((y1 >> 8));  //upper 8 Bit sent to LCD
	LCD_data_write((y1 & 0xFF));       //lower 8 Bit sent to LCD
	LCD_data_write((y2 >> 8));  // start addr
	LCD_data_write((y2 & 0xFF));       // end addr

	LCD_command_write(0x2b);  //code for row address
	LCD_data_write(x1 >> 8);  //
	LCD_data_write(x1 & 0xFF);       //
	LCD_data_write(x2 >> 8);  //
	LCD_data_write(x2 & 0xFF);       //

	LCD_command_write(0x2c);  // Memory Write
	for(int i = 0; i < pixels; i++){
		LCD_data_write(color >> 8);
		LCD_data_write(color & 0xFF);
	}
}
