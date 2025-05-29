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

//_________________________________________________________________________________________________
static volatile _Bool tim2Intrpt_g;   // global vars for this file
static volatile _Bool tim3Intrpt_g;   // return not possible cause IRQ called by ISR
static volatile _Bool rngIntrpt_g;    // ISR (IRQ) gets called by hardware -> it expects no return or even interact with other code than IRQ function
                                      //
static volatile _Bool SECS;           //
static volatile _Bool CECS;           //
//__________________________________________________________________________________________________
static volatile uint32_t rval;     // RNG var

static volatile uint32_t pc13;     // push button var

/*typedef struct {  //typedef to call struct by RNGv
    int32_t rval;  // RNG value
    _Bool  rsign;     // rand bit
} RNGv;*/










/*inline void delay_cycles(uint32_t cycles){// Timer in cpu clk cycl
    uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < cycles){
        __NOP(); // make it not optimizeable
    }
}*/





void delay(volatile unsigned int ___1){                //Timer for ms
	 RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;             // en clk for TIM
	 TIM2->DIER = TIM_DIER_UIE;                        //Update-Interrupt on
	 TIM2->CNT = 0;
	 TIM2->PSC = 240000 - 1;                           // devides counting clock (~4,3MHz max) 0 means AHB clk
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




int32_t RandomNG(void){
	//new random value after 216 periods of AHB clk if the output FIFO is empty RM p. 1412
	RNG->CR |= (0b1 << 2); //| (0b1 << 5) | ;        // en RNG intrpt funct ________________________________________________act once at start?

//	NVIC_EnableIRQ(RNG_IRQn);   //

/*	 do {

	 } while (!rngIntrpt_g);

	 NVIC_DisableIRQ(RNG_IRQn);*/
	//    if ((RNG->SR & RNG_SR_DRDY) != 0b0){  // if value is valid
	  //       rval = RNG->DR;                  // read value resets DRDY Flag
	   // }
	    if(((RNG->SR >> 6) & 0b1) == 1){   // invalid data detected
	    	SECS = ((RNG->SR >> 2) & 0b1);
	    }
	    if(((RNG->SR >> 5) & 0b1) == 1){   // invalid clk detected
	    	CECS = ((RNG->SR >> 1) & 0b1);
	    }

	    while((RNG->SR & 0b1) == 0){
		    if(((RNG->SR >> 6) & 0b1) == 1){   // invalid data detected
		    	SECS = ((RNG->SR >> 2) & 0b1);
		    	return 0;
		    }
		    if(((RNG->SR >> 5) & 0b1) == 1){   // invalid clk detected
		    	CECS = ((RNG->SR >> 1) & 0b1);
		    	return 0;
		    }

	    }

	    rval = RNG->DR;                  // read value resets DRDY Flag

	 if((SECS == 0b0) && (CECS == 0b0) && (rval != 0)){
		 return rval;

	 }else{
		 RNG->CR &= ~RNG_CR_RNGEN;
	 }
}



void RNG_IRQHandler(void){
	   rngIntrpt_g = 1;                // stop WFI loop
	   rval = RNG->DR;                  // read value resets DRDY Flag
	   RNG->CR &= ~(0b11 << 2);      // turn off RNG only option to reset RNG itrp Flags
}





uint8_t hwswflpy(void){
	uint8_t ue = pc13 / 4;   // can't be higher than 5
	pc13 = 0;
	return ue;

}


// Interrupt-Handler für EXTI-Linien 15 bis 10 (enthält auch EXTI13)
void EXTI15_10_IRQHandler(void)
{
    // Überprüfen, ob EXTI-Linie 13 den Interrupt ausgelöst hat
    if (EXTI->PR1 & (1 << 13))
    {
        // Interrupt-Pending-Bit löschen (durch Setzen auf 1)
        EXTI->PR1 |= (1 << 13);

        pc13 = pc13 + 200;
    }
}


