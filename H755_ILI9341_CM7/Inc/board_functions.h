/*
 * board_functions.h
 *
 *  Created on: May 27, 2025
 *      Author: felix
 */

#ifndef BOARD_FUNCTIONS_H_
#define BOARD_FUNCTIONS_H_
#include <stdint.h>

inline void delay_cycles(uint32_t cycles){// Timer in cpu clk cycl
    uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < cycles){
        __NOP(); // make it not optimizeable
    }
}


void delay(volatile unsigned int ___1);


void TIM2_IRQHandler(void);


void delay_u(volatile int ___2);


void TIM3_IRQHandler(void);


uint8_t RandomNG(uint8_t min, uint8_t max);


uint32_t RNG_IRQHandler(void);



#endif /* BOARD_FUNCTIONS_H_ */
