/*
 * board.c
 *
 *  Created on: May 27, 2025
 *      Author: felix
 */

#include <stm32h7xx.h>
#include <stdint.h>
#include "math.h"

#include <board.h>







void board_voltage(void){ // call at very first
		volatile int b;


		//Instruction on p.329
	    RCC->APB4ENR |= 0b10;        //SYSCFGEN bit set  en clk for hw in moment of clk mux changeing

	  /*  a = PWR->CR3;             // save register in a; to change things step by step. wrong values cause register Lock
	    a = a & ~(0b11 << 1);     // LDO and SD Bits off
	    a = (a |= (0b1       |     // BYPASS bit to 1
	    		 (0b1 << 5) |     // 2,5V input for step down conv
				 (0b1 << 3)));     // slightly raise of Vcore -> more stable*/
	    PWR->CR3 = (PWR->CR3 &
	    			~(PWR_CR3_LDOEN))            |
					PWR_CR3_SMPSLEVEL_1          |
					PWR_CR3_SMPSEXTHP            |
					PWR_CR3_SMPSEN               |
					PWR_CR3_BYPASS;                 // neccesary for VOS0


	    //Instruction on p. 297
		PWR->D3CR |= (0b11 << 14);   //VOS1 Mode set
		//while(1){a = ()
		SYSCFG->PWRCR |= 0b1;        //VOS0 Mode set (Boost Mode) BYPASS active neccesary
		while(1){b = (PWR->D3CR >> 13) & 0b1; if(b == 1){break;}}

}


void board_oclk(volatile int clksel){ // 0 HSI 440MHz  1 HSE 480MHz     call 2nd
	volatile int a;

	FLASH->ACR &= ~(0b1);  //Flash standard 7 wait cycles refere page 166 in RM -> 6 WS now


	RCC->CR &= ~(0b1 << 24);         // turn PLL1 off

	RCC->D2CCIP2R |= 0b1 << 8;

	if(clksel == 1){

			RCC->CR |= 0b1 << 16;
			while(1){a = (RCC->CR >> 17) & 0b1; if(a == 0b1){break;}}


			//clk input divider -> 5MHz input
			RCC->PLLCKSELR &= ~((0b1 << 9) | (0b1 << 17) | (0b1 << 25));    // PRE 32 -> 0 for DIVM 1,2,3
			RCC->PLLCKSELR |= (0b101 << 4) | (0b101 << 12) | (0b101 << 20) | 0b10; // PRE = 5 for DIVM 1,2,3  HSE sel as PLL input


		    RCC->PLLCFGR |= (0b10 << 2) | //PLL input frq range = 4 - 8 MHz
		    				(0b10 << 6) |
							(0b10 << 10);

			//Freq calculation   PLL1 = HSI/DIVM*(DIVN+(FRACN/2^13))/DIVP     p.436 RM

			//RCC->PLL1FRACR |= (0b1 << 15); //Frac = 4096 -> 456 ->460MHz clk   (2^13 -1 max)

		    RCC->PLL1DIVR &= (0b1 << 31) | (0b1 << 23);  // all 0 without changing res Bits 31;23
		    RCC->PLL1DIVR |= ((0b10111111)   | //DIVN = 54  -> 880MHz
		    				  (0b1 << 9)   | //DIVP = 2 -> 440MHz into CLK MUX
							  (0b11 << 16) | //DIVQ = 4 -> 220MHz
							  (0b11 << 24)); //DIVR = 4 -> 220MHz


		    RCC->PLL2DIVR &= (0b1 << 31) | (0b1 << 23);  // all 0 without changing res Bits 31;23
		    RCC->PLL2DIVR |= ((0b10111111)   | //DIVN = 60  -> 960MHz
		    				 (0b11 << 9)   | //DIVP = 2 -> 460MHz into CLK MUX
							 (0b11 << 16) | //DIVQ = 4 -> 230MHz
							 (0b11 << 24)); //DIVR = 4 -> 230MHz


			RCC->PLL3DIVR &= (0b1 << 31) | (0b1 << 23);  // all 0 without changing res Bits 31;23
		    RCC->PLL3DIVR |= ((0b10111111)   | //DIVN = 60  -> 960MHz
					  	  	 (0b11 << 9)   | //DIVP = 2 -> 460MHz into CLK MUX
							 (0b11 << 16) | //DIVQ = 4 -> 230MHz
							 (0b11 << 24)); //DIVR = 4 -> 230MHz



		    RCC->CR |= (0b1 << 24);         // turn PLL1ON

		    while(1) // wait till Flag signaling PLL1 is on
		    {a = (RCC->CR >> 25) & 0b1; if(a == 1){break;}}


		    RCC->D1CFGR = (RCC->D1CFGR & ~(0b1 << 11)) | 0b1001000; // HPRE /2 & D1PPRE /2
		    RCC->D2CFGR |= (0b10001 << 6); // D2PPRE & D2PPRE2 /2
		    RCC->D3CFGR |= (0b1 << 6); // D3PPRE /2

		    RCC->CFGR |= 0b11;  // set PLL1p as input frq for main clk
		    while(1)
		    {a = (RCC->CFGR >> 3) & 0b11; if(a == 3){break;}}  // wait till SWS == 0b11000 (PLL1 == input)

			a = 0;

	}




	else{
	  //HSI selected by default (00)
		//clk input divider -> 16MHz input
		RCC->PLLCKSELR &= ~((0b1 << 9) | (0b1 << 17) | (0b1 << 25));    // PRE 32 -> 0 for DIVM 1,2,3
		RCC->PLLCKSELR |= (0b1 << 6) | (0b1 << 14) | (0b1 << 22); // PRE = 4 for DIVM 1,2,3  HSI auto set as PLL input

		RCC->PLLCFGR |= (0b11 << 2) | //PLL input clk range 8 - 16
						(0b11 << 6) |
						(0b11 << 10);


		//Freq calculation   PLL1 = HSI/DIVM*(DIVN+(FRACN/2^13))/DIVP     p.436 RM

		//RCC->PLL1FRACR |= (0b1 << 15); //Frac = 4096 -> 456 ->460MHz clk   (2^13 -1 max)

		RCC->PLL1DIVR &= (0b1 << 31) | (0b1 << 23);  // all 0 without changing res Bits 31;23
		RCC->PLL1DIVR |= ((0b110110)   | //DIVN = 54  -> 880MHz
						  (0b1 << 9)   | //DIVP = 2 -> 440MHz into CLK MUX
						  (0b11 << 16) | //DIVQ = 4 -> 220MHz
						  (0b11 << 24)); //DIVR = 4 -> 220MHz


		RCC->PLL2DIVR &= (0b1 << 31) | (0b1 << 23);  // all 0 without changing res Bits 31;23
		RCC->PLL2DIVR |= ((0b111011)   | //DIVN = 60  -> 960MHz
						 (0b11 << 9)   | //DIVP = 2 -> 460MHz into CLK MUX
						 (0b11 << 16) | //DIVQ = 4 -> 230MHz
						 (0b11 << 24)); //DIVR = 4 -> 230MHz




		RCC->PLL3DIVR &= (0b1 << 31) | (0b1 << 23);  // all 0 without changing res Bits 31;23
		RCC->PLL3DIVR |= ((0b111011)   | //DIVN = 60  -> 960MHz
						 (0b11 << 9)   | //DIVP = 2 -> 460MHz into CLK MUX
						 (0b11 << 16) | //DIVQ = 4 -> 230MHz
						 (0b11 << 24)); //DIVR = 4 -> 230MHz



		RCC->CR |= (0b1 << 24);         // turn PLL1ON

		while(1) // wait till Flag signaling PLL1 is on
		{a = (RCC->CR >> 25) & 0b1; if(a == 1){break;}}


		RCC->D1CFGR = (RCC->D1CFGR & ~(0b1 << 11)) | 0b1001000; // HPRE /2 & D1PPRE /2
		RCC->D2CFGR |= (0b10001 << 6); // D2PPRE & D2PPRE2 /2
		RCC->D3CFGR |= (0b1 << 6); // D3PPRE /2

		RCC->CFGR |= 0b11;
		while(1)
		{a = (RCC->CFGR >> 3) & 0b11; if(a == 3){break;}}  // wait till SWS == 0b11000 (PLL1 == input)

		a = 0;
	}

}

// DATA Pins |0|1|2|3|4|5|6|7|     || G9|D15|G14|E13|E14|E11|A8|G12
// Function Pins                        RD|WR|RS|CS|RST
//                                      A3|C0|C3|B1|C2

void board_start(void){//    call 3rd
	RCC->APB4ENR |= 0b10;        //SYSCFGEN bit set   also in board_voltage
	RCC->AHB4ENR |= 0b111 << 2;	    //clk for Ports C,D,E act
 // define pin type
	GPIOC->MODER &= ~(0b11 << (2 * 13)); // 00 = Input C13 blue PB
	GPIOD->MODER &= 0x5555; // 0 - 7
	GPIOE->MODER &= (0x1550);    // 2 - 6

	//GPIOC->PUPDR |= 0b11 << (2 * 13);  // Pull up

	//define pin rising/falling edges precision/time
	GPIOD->OSPEEDR |= 0xFFFF;
	GPIOE->OSPEEDR |= 0x3FF0;


	RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;     // Timer clk on
	RCC->APB1LENR |= RCC_APB1LENR_TIM3EN;

	RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;        // bus clk for random number generator on
	RNG->CR |= RNG_CR_RNGEN;                  // RNG on



	SCB->CPACR |= ((0b11 << 20) | (0b11 << 22));  //activate FPU refere ARM Manual (ARM DUI 0646B ID082615) 4-56




	// 5. EXTI-Konfiguration:
	    //    - Verbinde EXTI-Linie 13 mit Port C.
	    //      Dazu gehört EXTICR[3] (für EXTI12 bis EXTI15):
	    //      Für Pin 13: (13 % 4 = 1) – also Bits [7:4] in EXTICR[3] müssen auf 0x2 (Port C) gesetzt werden.
	    SYSCFG->EXTICR[3] &= ~(0xF << 4);  // Lösche die Bits für EXTI13
	    SYSCFG->EXTICR[3] |=  (0x2 << 4);    // Wähle Port C (0: A, 1: B, 2: C, …)

	    // 6. Konfiguriere EXTI-Linie 13:
	    //    a) Interrupt unmasken: IMR1
	    EXTI->IMR1 |= (1 << 13);
	    //    b) Trigger: Da der Button in der Regel aktiv niedrig ist, wählen wir den fallenden Flanken-Trigger.
	    EXTI->FTSR1 |= (1 << 13);
	    //    c) (Optional) Rising Edge abschalten, falls nicht benötigt:
	    EXTI->RTSR1 &= ~(1 << 13);

	    // 7. NVIC: EXTI15_10-Interrupt aktivieren (da EXTI13 in diesem IRQ zusammengefasst ist)
	   NVIC_SetPriority(EXTI15_10_IRQn, 2);   // Priorität einstellen (2 = Beispielwert)
	   NVIC_EnableIRQ(EXTI15_10_IRQn);          // Interrupt freigeben

}





void DWT_Init(void){   // call when u need
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // activate trace and debug
    DWT->CYCCNT = 0;                                // clk cycle cnt = 0
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // act DWT
}

