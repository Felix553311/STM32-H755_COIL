/*
 * board_functions.h
 *
 *  Created on: May 27, 2025
 *      Author: felix
 */

#ifndef BOARD_FUNCTIONS_H_
#define BOARD_FUNCTIONS_H_
#include <stdint.h>

// inline cause function to be integrated directly in funct where it was called -> speed optimized -> higher accuracy
// inline cause invisibility in other source files whe def in a source file
#include <stdint.h>
inline void delay_cycles(uint32_t cycles){     // Timer in cpu clk cycl
    uint32_t start = DWT->CYCCNT;              // register space for cycle counts    takes val of start
    while ((DWT->CYCCNT - start) < cycles){    // wait till specif numb of cycles passed
        __NOP();                               // make it not optimizeable
    }
}


void delay(volatile unsigned int ___1);


void TIM2_IRQHandler(void);


void delay_u(volatile int ___2);


void TIM3_IRQHandler(void);


int32_t RandomNG(void);


//void RNG_IRQHandler(void);


_Bool hwswflpy(void);


//void EXTI15_10_IRQHandler(void);



#endif /* BOARD_FUNCTIONS_H_ */
