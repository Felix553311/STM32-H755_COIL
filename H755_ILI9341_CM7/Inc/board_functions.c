/*
 * board_functions.c
 *
 *  Created on: May 27, 2025
 *      Author: felix
 */
#include <stm32h7xx.h>
#include <stdint.h>
#include <math.h>

#include <LCD_draw_lib.h>
#include <board_functions.h>


volatile _Bool tim2Intrpt_g;
volatile _Bool tim3Intrpt_g;
volatile _Bool rngIntrpt_g;










/*inline void delay_cycles(uint32_t cycles){// Timer in cpu clk cycl
    uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < cycles){
        __NOP(); // make it not optimizeable
    }
}*/





void delay(volatile unsigned int ___1){          //Timer for ms
	 RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;              // en clk for TIM
	 TIM2->DIER = TIM_DIER_UIE;                         //Update-Interrupt on
	 TIM2->CNT = 0;
	 TIM2->PSC = 219999;                                // devides counting clock (~4,3MHz max) 0 means AHB clk
	 TIM2->ARR = ___1;                                 // define how long is counted
	 tim2Intrpt_g = 0;                                 // reset for "infinite" sleep loop
	 TIM2->CR1 = TIM_CR1_CEN;                          // Timer start

	 NVIC_EnableIRQ(TIM2_IRQn);                        // en Interrupt Handler

	 do {
		 __WFI();                                      // wait for interrupt -> sleep mode
	 } while (!tim2Intrpt_g);

	 TIM2->CR1 &= ~TIM_CR1_CEN;                        //Timer off
}


void TIM2_IRQHandler(void){                            // Interrupt (Handler) makes cpu work here
	 tim2Intrpt_g = 1;                                 // stop sleep loop
	 TIM2->SR &= ~TIM_SR_UIF;                          // Update Interrupt Flag = 0 -> stops generating interrupts
}



void delay_u(volatile int ___2){          //Timer for TFT output freq
	 RCC->APB1LENR |= RCC_APB1LENR_TIM3EN;
	 TIM3->DIER = TIM_DIER_UIE;                         //Update-Interrupt on
	 TIM3->CNT = 0;
	 TIM3->PSC = 57 - 1;
	 TIM3->ARR = ___2;                                  //1 = 231,82ns
	 tim3Intrpt_g = 0;
	 TIM3->CR1 = TIM_CR1_CEN;                          //Timer start

	 NVIC_EnableIRQ(TIM3_IRQn);

 do {
	 __WFI();
 } while (!tim3Intrpt_g);

 	 TIM3->CR1 &= ~TIM_CR1_CEN;                        //Timer aus
}


void TIM3_IRQHandler(void){
	 tim3Intrpt_g = 1;
	 TIM3->SR &= ~TIM_SR_UIF;
}




uint8_t RandomNG(uint8_t min, uint8_t max){
	//new random value after 216 periods of AHB clock if the output FIFO is empty RM p. 1412
	RNG->CR |= 0b1 << 3;        // en RNG intrpt funct

	NVIC_EnableIRQ(RNG_IRQn);   //

	 do {
		 __WFI();                                      // wait for interrupt -> sleep mode
	 } while (!rngIntrpt_g);

	 NVIC_DisableIRQ(RNG_IRQn);

	 return 0;
}


uint32_t RNG_IRQHandler(void){
    if ((RNG->SR & RNG_SR_DRDY) != 0U){  // if value is valid
         int val = RNG->DR;                  // read value resets DRDY Flag

         rngIntrpt_g = 1;                // stop WFI loop

         return val;                     // if val == 0
    }
    if(((RNG->SR >> 6) & 0b1) == 1){   // invalid data detected
    	RNG->CR &= ~RNG_CR_RNGEN;
    	return ((RNG->SR >> 2) & 0b1);
    }
    if(((RNG->SR >> 5) & 0b1) == 1){   // invalid clk detected
    	RNG->CR &= ~RNG_CR_RNGEN;
    	return ((RNG->SR >> 1) & 0b1);
    }

}




