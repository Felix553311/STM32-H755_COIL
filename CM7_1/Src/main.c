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
#include <Char_lib.h>


#if !defined(__SOFT_FP__) && defined(__ARM_FP)  // make sure hw fp ops available
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define sm_digi(d, xpos, ypos, digcol)  sm_chars(digbit[(d)], (xpos), (ypos), (digcol));

//https://rgbcolorpicker.com/565
#define BLACK  		   0x00a4//0x0000
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
#define tubecolor	   0x8160

#define LCD_height     239
#define LCD_width      319
#define faspeed        3
#define tubedistance   80
#define Tubemaplen     4
#define flpy_start     119
#define flpy_width	   4
#define flpy_height    4




static uint8_t tubespace;      // find solution to avoid global variables in future
static uint8_t xposofst;


typedef struct{  //typedef to call struct by gappos
    uint8_t gaplow;  // vall
    uint8_t gaphigh;     // valh
}gappos;


typedef struct{
	int8_t rise;
	_Bool iput;    // got any ipt -> iput = 1
} dummy;

typedef struct{
	uint16_t x;
	uint8_t gapl;
	uint8_t gaph;
	uint8_t tublength;
}Tubesaver;

typedef struct{
	_Bool lencntact;
	//uint8_t lencnt;
	uint8_t tubemapnumber;
} dummy2;



dummy flappy(void);  // Prototype-decl
void bird(volatile dummy *bck);  // Prototyp-decl
void flappy_poso(uint16_t xpos1, int16_t *ypos1, uint8_t yposold);
void gamecyc(void);






void hit_tstf(Tubesaver *tubes, uint8_t flpy_h, dummy2 *tbposholder){
	static uint8_t lifes = 4;												// 3 lifes -> decrements only lifes - 1 times
	static unsigned int score;
	for(uint8_t i = 0; i < 4; i++){                 					  	//refresh every tubepos in the tubemap
		if((tubes[i].x > 0) && (tubes[i].x <= LCD_width)){
			tubes[i].x--;
		}
		if((tubes[i].x == xposofst + flpy_width) ){									// if 1st px of flpy is equal to 1st line of tube make the test
			tbposholder->lencntact = 1;
			tbposholder->tubemapnumber = i;
			if((flpy_h <= tubes[i].gaph)  || (flpy_h >= tubes[i].gapl)){	// test if flpy is outside the gap hight     LCD upside down all values fliped

				//lifes--;

				if(lifes < 1){

					for(;;){
						__WFI();
					}
				}
			} else{

				score++;
				// score + 1

			}
		}
	}



}








void tuberand(gappos *gap){  // call RNG to work;    create 2 rand values with defined space inbetween
//min !must! be in range of LCD hight = 0 to (240 - 1)
                                           //
	uint32_t val = RandomNG();            //                                                                ____________________________________
    //                            max    min      + 1     //                                                                \                 /                |
	gap->gaphigh = (val % (LCD_height - tubespace + 1));   	// shrink val to LCD writeable space  vall % 190 ->  (2^32 - 1) - {[(2^32 - 1) / 190] - residual of} * 190  = (0,...,239)
	gap->gaplow = gap->gaphigh + tubespace;					// calculate lower gap end

}


void flappy_pos(volatile uint16_t xpos1, int16_t *ypos1, dummy2 *tbposholder, Tubesaver *tubes){//, volatile int16_t ypos1){// flpy pos could be negative -> signed val for y needed
	 uint8_t yposold = *ypos1;
	//pos->yposold = pos->ypos1; 					// save old ypos
	dummy volatile bck = flappy();						// get back info of gained altitude(rise); if ipt occoured
	bird(&bck);

	*ypos1 = *ypos1 + bck.rise;							// adds rised px



	if(*ypos1 <= 0){                    			// limits for highest/lowest pos of flpy    without limits flpy would appear on opposite edge
		*ypos1 = 0;
	}else if(*ypos1 >= LCD_height - 3){
		*ypos1 = LCD_height - 3;
	}if((tbposholder->lencntact)){
		tubes[tbposholder->tubemapnumber].tublength--;
		if((tubes[tbposholder->tubemapnumber].tublength == 0)){
			tbposholder->lencntact = 0;
		}else if((*ypos1 > (tubes[tbposholder->tubemapnumber].gapl - 3)) && (tubes[tbposholder->tubemapnumber].tublength > 0)){
			*ypos1 = (tubes[tbposholder->tubemapnumber].gapl - 3);
		}else if((*ypos1 < (tubes[tbposholder->tubemapnumber].gaph)) && (tubes[tbposholder->tubemapnumber].tublength > 0)){
			*ypos1 = tubes[tbposholder->tubemapnumber].gaph ;
		}

	}

	flappy_poso(xpos1,ypos1,yposold);
}


void flappy_poso(uint16_t xpos1, int16_t *ypos1, uint8_t yposold){

	if(xpos1 > (LCD_width - xposofst)){          // compensate distance between left edge and flpy xpos
		xpos1 = xpos1 + xposofst - LCD_width ;
	}else{
		xpos1 = xpos1 + xposofst;
	}


	switch (xpos1){												// draw at x = 0 forbidden
	case 1://_________________________________________________________________________________________ 1|2|3|4   -> del previous pix case 319
		Line(319,yposold,yposold + 3,BLACK);
		Rect(1,3,yposold,yposold + 3,BLACK);	// del flpy
		Line(xpos1 + 3,*ypos1,*ypos1 + 3,WHITE);
		break;
	case 317://_______________________________________________________________________________________ 317|318|319|1
		Rect(xpos1 - 1,xpos1 + 3,yposold,yposold + 3,BLACK);    // del first line of previous rect
		Rect(xpos1, xpos1 + 2,*ypos1,*ypos1 + 3,WHITE);			//
		Line(LCD_width - xpos1 - 1,*ypos1,*ypos1 + 3,WHITE);		// draw new line at px pos 1
		break;
	case 318://_______________________________________________________________________________________ 318|319|1|2
		Rect(xpos1 - 1,xpos1 + 2,yposold,yposold + 3,BLACK);     	// del
		Line(1,yposold,yposold + 3, BLACK);                         // del Line at x = 1 from 317
		Rect(xpos1,xpos1 + 1,*ypos1,*ypos1 + 3,WHITE);
		Rect(1,LCD_width - xpos1 + 1,*ypos1,*ypos1 + 3,WHITE);			// new at px pos 2
		break;
	case 319://_______________________________________________________________________________________ 319|1|2|3
		Rect(xpos1 - 1,xpos1,yposold,yposold + 3,BLACK);
		Rect(1,2,yposold,yposold + 3,BLACK);     				// del
		Line(xpos1,*ypos1,*ypos1 + 3,WHITE);
		Rect(1,3,*ypos1,*ypos1 + 3,WHITE);		// new at px pos 3
		break;
	default:
	    Rect(xpos1 - 1,xpos1 + 2,yposold,yposold + 3,BLACK);	// del flpy
		Rect(xpos1,xpos1 + 3,*ypos1,*ypos1 + 3,WHITE);

	}
}


dummy flappy(void){
	static const uint8_t uses[10] = {10,0, 6,0, 3,0, 2,0, 1,0}; // rise Ary "0" slow down rise proce
	static uint8_t callcoun1 = 10;

	dummy movup;
	movup.iput = hwswflpy();  						  //chk for ipt
	movup.rise = 0;
	if(movup.iput && (callcoun1 > 6)){		    	  // if c13 pressed & timeout over -> gain alt fast
		callcoun1 = 0;
		movup.rise =  uses[callcoun1];
	}else if(movup.iput && (callcoun1 <= 6)){			// if c13 pressed & timeout act
		movup.rise =  uses[callcoun1];
	}else if((!movup.iput) && callcoun1 <= 10){        	// if not pressed rise just little to reach smooth movement between rise and fall
		movup.rise =  uses[callcoun1];
	}

	callcoun1++;    							   // increase cycle counter
	if(callcoun1 > 9){							   // res counter after timeout
		callcoun1 = 10;
	}
	return movup;

}


void bird(volatile dummy *bck){      // accelerated (by call cyc -> time) fallspeed
	static uint16_t callcoun2;
	//uint16_t t;

	if(bck->iput){                // no fall, when jump
		callcoun2 = 0;
	}

	callcoun2++;
	if(callcoun2 > 35){        // set max fallspeed
		callcoun2 = 35;
	}
	//uint8_t axa = (callcoun2 * callcoun2) / (faspeed * 90);		// calculate final fallspeed         !!!chck on screen!!!
	bck->rise -= (callcoun2 * callcoun2) / (faspeed * 90);		// calculate final fallspeed         !!!chck on screen!!! axa;

}




int main() {
	//put a struct here containing all variables.   use the variables by callen with pointers. -> no return vals needed anymore -> more efficient
    DWT_Init();
    board_voltage();
	board_oclk(1);      // 1 means HSE -> 480MHz clk freq
	board_start();
    LCD_Init();
    delay(1000);
   // scr_area(0b1,0x140,0);      //def first line as dummy to keep screen scrollable without artifcts

    gamecyc();

   /* Rect(0,319,0,LCD_height,WHITE);
    sm_digi(0,160,120,BLACK);
    sm_digi(1,168,120,BLACK);
    sm_digi(2,176,120,BLACK);
    sm_digi(3,184,120,BLACK);
    sm_digi(4,192,120,BLACK);
    sm_digi(5,200,120,BLACK);
    sm_digi(6,208,120,BLACK);
    sm_digi(7,216,120,BLACK);
    sm_digi(8,224,120,BLACK);
    sm_digi(9,232,120,BLACK);*/
}




void gamecyc(void){							// declare structs with vars that should be available in all functions
/*	struct flpy_y{
		 //uint16_t xpos1 = 0;					// only static and global initialized with "0" by default
		 int16_t ypos1;
		 uint8_t yposold;
	}flpy_y1;*/

	struct{
		uint16_t tbcoun;
		uint16_t tblen;
		//uint8_t tblenset;
		uint8_t tubecoun;
	}counters;

	struct{
		volatile uint8_t lifes;
		volatile unsigned int score;
	}scoreboard;


	uint32_t twraw;
	int16_t ypos1 = flpy_start;

	gappos gap;
	dummy2 tbposholder;

	counters.tbcoun = 0;																	// count number of empty lines drawn
	counters.tblen = 10;																	// counter for width of a tube
	uint16_t tblenset = 10;																	// set the max val tblen can reach - the max width of a tube
	counters.tubecoun = 0;																	// count number of tubes stored in the ary

	scoreboard.lifes = 3;
	scoreboard.score = 0;

	tbposholder.lencntact = 0;
	//tbposholder.lencnt = 0;
	tbposholder.tubemapnumber = 0;

    tubespace = 100;
    xposofst = 59;
    Tubesaver tubemap[4];
    for(int i = 0; i < 4; i++){
    	tubemap[i].gaph = 0;
    	tubemap[i].gapl = 0;
    	tubemap[i].x = 0;
    	tubemap[i].tublength = 0;
    }
    //uint8_t ys = flpy_start;
    scr_area(0b1,0x13F,0);      //def first line as dummy to keep screen scrollable without artifcts
    Line(0,0,LCD_height,GREEN);
	for(;;){
			for(uint16_t i = 1;i < LCD_width + 1; i++){//

				//Line(i,0,LCD_height,BLACK);


				flappy_pos(i, &ypos1, &tbposholder, tubemap);											// calculate and draw flappys pos
				LCD_Scroll(i);
				hit_tstf(tubemap, ypos1, &tbposholder);											// calculate flpy hits a tube + scoreboard +
				delay_u(3480);
				//delay_u(1740);
				counters.tbcoun++;
				counters.tblen++;																//
				if(counters.tbcoun == tubedistance + tblenset){									// every 80 cycles after end of previous tubepair new generation of a tube pair
					twraw = RandomNG();															// save new raw random tube length

					if ((twraw % 10) < 9) {														// 90% chance for shrinking width to 3 - 20
						tblenset = (twraw % (20)) + 3;
					}else{
						tblenset = (twraw % (LCD_width - 4)) + 1;								// shrink from 1 - 315
					}



					tuberand(&gap);																// gappos gappos = tuberand(&gap);
					tubemap[counters.tubecoun].gaph = gap.gaphigh;								// save new tube pair data
					tubemap[counters.tubecoun].gapl = gap.gaplow;								//
					tubemap[counters.tubecoun].x = 319;											// save start pos
					tubemap[counters.tubecoun].tublength = tblenset + flpy_width + 2;

					Line(i,0,gap.gaphigh, tubecolor);												// draw first pair tube lines
					Line(i,gap.gaplow,239, tubecolor);											//
					Line(i,gap.gaphigh,gap.gaplow,BLACK);										// fill the gap with black
					counters.tbcoun = 0;                                      					// count passed empty rotations
					counters.tblen = 0;                                    						// make tube thickness function start
					counters.tubecoun++;                             							// write next time into second space of the struct arry
				}
				else if(counters.tblen <= tblenset){											// create tube thickness
					Line(i,0,tubemap[counters.tubecoun - 1].gapl, tubecolor);						//
					Line(i,tubemap[counters.tubecoun - 1].gaph,LCD_height, tubecolor);				//
					Line(i,gap.gaphigh,gap.gaplow,BLACK);										// only fill parts, that havn't been already written with tubes
				}else if((counters.tblen > tblenset) & counters.tubecoun >= 4){					// max 4 tube starting lines are displayed
																								// get new val for tube thickness
					counters.tubecoun = 0;
					Line(i,0,LCD_height,BLACK);
				}else{
					Line(i,0,LCD_height,BLACK);
				}

				 //hit_tstf(tubemap, ypos1);


				if(i == LCD_width){i = 0; if(tubespace > 50){tubespace--;}} 					 // make tubespace smaller over time
			}

	}



}


// save tblenset within the tube struct and give it to flappy_pos after first line of tube passed
// screen not upside down anymore
// save tube
