/*
 * board.h
 *
 *  Created on: May 27, 2025
 *      Author: felix
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <stdint.h>

void board_voltage(void);


void board_oclk(volatile int clksel);


void board_start(void);


void DWT_Init(void);


#endif /* BOARD_H_ */
