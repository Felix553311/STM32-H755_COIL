/*
 * LCD_draw_lib.h
 *
 *  Created on: May 27, 2025
 *      Author: felix
 */

#ifndef LCD_DRAW_LIB_H_
#define LCD_DRAW_LIB_H_
#include <stdint.h>

void scr_area(uint16_t topcorn, uint16_t Area, uint16_t bottomcorn);


void LCD_Scroll(const uint16_t scr_with);


void LCD_command(const uint8_t command);


void LCD_dataw(const uint8_t data);


void Pixel(uint16_t x1, uint16_t y1, uint16_t color);


void Line(uint16_t x1, uint16_t y1, uint16_t y2, uint16_t color);


void Line_h(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t color);


void Rect(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint16_t color);


#endif /* LCD_DRAW_LIB_H_ */
