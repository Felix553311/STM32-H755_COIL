/**
 ******************************************************************************
 * @file           : main.c
 * @date		   : May 10, 2025
 * @author         : Felix Bahrke
 * @brief          : contains calls of mein functions for program flow & development of new functions
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
#include <LCD_draw_lib.h>


#if !defined(__SOFT_FP__) && defined(__ARM_FP)  // make sure hw fp ops available
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define BLACK  		   0x0000
#define RED  	       0xF800
#define MNIGHTBLUE     0x4000
#define GREEN  		   0x07E0
#define YELLOW  	   0x07FF
#define MAGENTA 	   0xF81F
#define CYAN   	 	   0xFFE0
#define WHITE 		   0xFFFF
#define GREEN2 		   0x2FA4
#define CYAN2 		   0x07FF
#define BLUE  		   0xFA20

#define LCD_hight      239
#define LCD_width      319
#define faspeed        3
#define tubedistance   80
#define Tubemaplen     4



static uint16_t tbcoun;
static uint8_t counsaver;
static uint8_t tblen;
static uint8_t flpyh = 119;
static uint8_t tubespace;      // find solution to avoid global variables in future
static uint8_t xposofst;
static uint8_t tubecoun;


typedef struct{  //typedef to call struct by gappos
    uint8_t gaplow;  // vall
    uint8_t gaphigh;     // valh
}gappos;


typedef struct{
	uint8_t rise;
	_Bool iput;    // got any ipt -> iput = 1
} dummy;

typedef struct{
	uint16_t x;
	uint8_t gapl;
	uint8_t gaph;
}Tubesaver;

typedef struct{
	uint8_t lifes;
	unsigned int score;
}Scrb_d;




dummy flappy(void);  // Prototyp-decl
uint8_t bird(_Bool);  // Prototyp-decl







void hit_tstf(Tubesaver *tubes, uint8_t flpy_h){							// just open the array of structs not copy
	static uint8_t lifes = 3;									// static makes the variable remember it's value
	static unsigned int score;									// call the scoreboard variables by pointer in the future
	Rect(0,LCD_width,0,LCD_hight,BLACK);								// delete whole screen 
	for(uint8_t i = 0; i < 4; i++){                 					  	//refresh every tubepos in the tubemap
		if((tubes[i].x > 0) && (tubes[i].x <= LCD_width)){
			tubes[i].x--;



			//Line(tubes[i].x + 1,0,tubes[i].gapl, BLACK);			// add for thickness first upper tube      
			//Line(tubes[i].x + 1,tubes[i].gaph,239, BLACK);			// draw first lower tube


			Line(tubes[i].x,0,tubes[i].gapl, GREEN);			// draw first upper tube      ! the screen is upside down !
			Line(tubes[i].x,tubes[i].gaph,239, GREEN);			// draw first lower tube


		}
		if((tubes[i].x == xposofst + 3) ){									// if 1st px of flpy is equal to 1st line of tube make the test
			if((flpy_h >= tubes[i].gaph)  || (flpy_h <= tubes[i].gapl)){	// test if flpy is outside the gap hight     LCD upside down all values fliped

				lifes--;

				if(lifes < 1){
					// add callback to menue screen here

					}
				}
			} else{

				score++;
				// 

			}
		}
	}

}








gappos tuberand(void){  // call RNG to work;    create 2 rand values with defined space inbetween
//min !must! be in range of LCD hight = 0 to (240 - 1)
                                           //
	uint32_t val = RandomNG();            // replace it by srand function                                ____________________________________
    //                  max    min + 1     //                                                                \                 /                |
	uint8_t vall = (val % (LCD_hight - tubespace + 1));   // shrink val to LCD writeable space  vall % 190 ->  (2^32 - 1) - {[(2^32 - 1) / 190] - residual of} * 190  = (0,...,239)
	uint8_t valh = vall + tubespace;			// calculate ypos end of the gap


	gappos result;               // result has same type as gappos
	    result.gaplow = vall;    // save vall in gaplow var of struct "result"
	    result.gaphigh = valh;   // save valh in gaphigh var of struct "result"

	    return result;           // return struct "result" -> needs to be saved in struct "gappos" or other struct of same type as gappos


}


dummy flappy_pos(uint16_t xpos1, int16_t ypos1){// flpy pos could be negative -> signed val for y needed

	uint8_t yposold = ypos1; 					// save old ypos
	dummy bck = flappy();						// get back info of gained altitude(rise); if ipt occoured
	ypos1 = (ypos1 - bck.rise);            		// adds rised px
	ypos1 = (ypos1 + bird(bck.iput));     		// subs fallen px



	if(ypos1 <= 0){                    			// limits for highest/lowest pos of flpy    without limits flpy would appear on opposite edge
		ypos1 = 0;
	}else if(ypos1 >= LCD_hight - 3){
		ypos1 = LCD_hight - 3;
	}


    Rect(xpos1,xpos1 + 3,yposold,yposold + 3,BLACK);	// del flpy old dawn pixels
	Rect(xpos1,xpos1 + 3,ypos1,ypos1 + 3,WHITE);	// draw ne position

	bck.rise = ypos1;				// abuse struct
	return bck; 					// keep old high for deleting objects
}


dummy flappy(void){
	static const uint8_t uses[10] = {10,0, 6,0, 3,0, 2,0, 1,0}; // rise Ary "0" slow down rise procedure
	static uint8_t callcoun1;

	dummy movup;
	movup.iput = hwswflpy();  					//chk for ipt and save in the input variable of the struct movup which has same structure as the dummy blueprint
	movup.rise = 0;							
	if(movup.iput && (callcoun1 > 6)){		    	  // if button pressed & timeout over -> gain alt fast
		callcoun1 = 0;
		movup.rise =  uses[callcoun1];
	}else if(movup.iput && (callcoun1 <= 6)){			// if button pressed & timeout act
		movup.rise =  uses[callcoun1];
	}else if((!movup.iput) && callcoun1 <= 10){        	// if not pressed rise just little to reach smooth movement between rise and fall
		movup.rise =  uses[callcoun1];
	}

	callcoun1++;    							   // increase cycle counter
	if(callcoun1 > 9){							   // reset counter after timeout
		callcoun1 = 10;
	}
	return movup;

}


uint8_t bird(_Bool pcpress){      // accelerated (by call cycle -> time) fallspeed
	static uint16_t callcoun2;

	if(pcpress){                // no fall, when jump button pressed
		callcoun2 = 0;
	}

	callcoun2++;
	if(callcoun2 > 35){        // set max fallspeed
		callcoun2 = 35;
	}
 uint8_t axa = (callcoun2 * callcoun2) / (faspeed * 90); // calculate final fallspeed         !!!chck on screen!!!
return (axa);	  

//	return 0;
}




int main() {
    DWT_Init();
    board_voltage();
	board_oclk(1);      // 1 means HSE -> 480MHz clk freq
	board_start();
    LCD_Init();
    delay(1000);


    gamecyc();			// only this function contains the game

}




void gamecyc(void){
	Rect(0,LCD_width,0,LCD_hight,BLACK);					// create a black screen

	for(;;){
		Tubesaver tubemap[4];						// make sure all variables have the values they supposed to have
		for(int i = 0; i < 4; i++){
			tubemap[i].gaph = 0;
			tubemap[i].gapl = 0;
			tubemap[i].x = 0;
		}
	    tubespace = 100;
		xposofst = 59;
	    tblen = 10;
		for(int i = LCD_width; i > 0; i--){

			dummy flpyhn = flappy_pos(xposofst, flpyhn.rise); 		// draw flpy and calculate rising/falling  xposofst is const distance to the edge
			//delay_u(1740);       add delays to slow down the game speed
			//delay_u(1740);

			tbcoun++;
			tblen++;
			if(tbcoun == tubedistance){                      		// every 80 cycles new generation of a tube pair
				gappos gappos = tuberand();
				tubemap[tubecoun].gapl = gappos.gaplow;				// save new tube data
				tubemap[tubecoun].gaph = gappos.gaphigh;
				tubemap[tubecoun].x = 319;							// save x position for drawing tube       realized just as a single vertical line

				Line(LCD_width,0,gappos.gaplow, 204 * i + 10);			// draw first upper tube      ! the screen is upside down !
				Line(LCD_width,gappos.gaphigh,239, 204 * i + 10);		// draw first lower tube

				tbcoun = 0;                                      	// count passed empty rotations
				tblen = 0;                                       	// make tube thickness function start     not used right now
				tubecoun++;                             			// write next time into second space of the array
			}

			if(tubecoun >= 4){
				tubecoun = 0;						// amount of tubes have to be calculated according to lcd width  320/80 = 4
			}


			hit_tstf(tubemap,flpyhn.rise);					// do hit test and scoreboard calculations


			if(i == 1){i = 319; if(tubespace > 50){tubespace--;}}  // make tubespace smaller every full rotation
		}




	}



}

