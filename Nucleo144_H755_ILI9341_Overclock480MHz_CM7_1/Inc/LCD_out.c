/*
 * LCD_out.c
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




#define LCD_RD   GPIO_ODR_OD6 //Serves as read signal/MCU read data at the rising edge. Pg11 Datasheet
#define LCD_WR   GPIO_ODR_OD5 //Serves as write signal/command at the rising edge
#define LCD_RS   GPIO_ODR_OD2 //D/CX (0=Command/1=Data)
#define LCD_CS   GPIO_ODR_OD4 //Chip Select Pin : Active Low
#define LCD_RST  GPIO_ODR_OD3 //Shield Reset



 void LCD_write(const uint8_t data){ // inline cause less overhead maybe
	GPIOE->ODR &= ~LCD_WR; // WR = 0

    //DATA Pins                              0|1 |2 |3 |4 |5 |6 |7
	//										D0|D1|D2|D3|D4|D5|D6|D7
	// Function Pins                        RD|WR|RS|CS|RST
	//                                      A3|C0|C3|E4|C2
    //
    //

	GPIOD->ODR = data;
	/*





    GPIOD->BSRR = ((0b1 << (15)) << 16) |      // top 16bit is reset 1 in -> 0 out
    			  (((data >> 1) & 0b1) << 15); // lower 16bit is set 1 in -> 1 out
    // Port G
    {//_______________________________________________________________________________________________//
        const uint32_t clearMaskG = (0b1 << 9)  | 													  //
        							(0b1 << 14) | 													  //
									(0b1 << 12);													  //
        const uint32_t setMaskG = (((data >> 0) & 0b1) << 9)  |										  //
                                  (((data >> 2) & 0b1) << 14) |										  // lets comp know var onl used in here as const
                                  (((data >> 7) & 0b1) << 12);										  //
        GPIOG->BSRR = (clearMaskG << 16) | setMaskG;												  //
    }//_______________________________________________________________________________________________//

    // Port E
    {
        const uint32_t clearMaskE = (0b1 << 13) |
        							(0b1 << 14) |
									(0b1 << 11);
        const uint32_t setMaskE = (((data >> 3) & 0b1) << 13) |
                                  (((data >> 4) & 0b1) << 14) |
                                  (((data >> 5) & 0b1) << 11);
        GPIOE->BSRR = (clearMaskE << 16) | setMaskE;
    }

    // Port A
    {
        const uint32_t clearMaskA = (0b1 << 8);
        const uint32_t setMaskA   = (((data >> 6) & 0b1) << 8);
        GPIOA->BSRR = (clearMaskA << 16) | setMaskA;
    }
*/

   // delay_cycles(1); //necces for make TFT working
    GPIOE->ODR |= LCD_WR; // send data after (rising edge)
   // delay_cycles(1);
}







void LCD_Init(void){
 	//Reset TFT
 	GPIOE->ODR |= LCD_RST; //active low
 	delay(5);
 	GPIOE->ODR &= ~LCD_RST; //reset TFT
 	delay(15);
 	GPIOE->ODR |= LCD_RST; // turn off rst
 	delay(15);

 	//The below is just preparation for Write Cycle Seq
 	GPIOE->ODR |= LCD_CS; //Chip-Select Active Low Signal
 	GPIOE->ODR |= LCD_WR;
 	delay_u(2000);
 	GPIOE->ODR |= LCD_RD;
 	GPIOE->ODR &= ~LCD_CS;  //cs low to actv ears of controller

 	LCD_command_write(0xC5);    //Test this Out | VCOM Control 1 : Colour Contrast Maybe
 	LCD_data_write(0x54);       //VCOM H 1111111 0x7F
 	LCD_data_write(0x00);       //VCOM L 0000000

 	LCD_command_write(0x36);    //Memory Access Control | DataSheet Page 127
 	///LCD_data_write(0x48);    //Adjust this value to get right color and starting point of x and y
 	LCD_data_write(0b100);  //
 	LCD_command_write(0x3A);    //COLMOD: Pixel Format Set  p. 134
 	LCD_data_write(0x55);       //16 Bit RGB and MCU
 	LCD_command_write(0x11);    //Sleep Out  p. 245
 	delay(10);                  //wait at least 5ms after sleepout
 	LCD_command_write(0x29);    //Display on.
 	LCD_command_write(0x2c);    //Memory Write | DataSheet Page 245
 //	LCD_command_write(0x51);    // display brightness
 	//LCD_data_write(0xFF);       // brightness = 50%

 }
