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







void hit_tstf(Tubesaver *tubes, uint8_t flpy_h){
	static uint8_t lifes = 3;
	static unsigned int score;
	Rect(0,LCD_width,0,LCD_hight,BLACK);
	for(uint8_t i = 0; i < 4; i++){                 					  	//refresh every tubepos in the tubemap
		if((tubes[i].x > 0) && (tubes[i].x <= LCD_width)){
			tubes[i].x--;



			//Line(tubes[i].x + 1,0,tubes[i].gapl, BLACK);			// draw first upper tube      ! the screen is upside down !
			//Line(tubes[i].x + 1,tubes[i].gaph,239, BLACK);			// draw first lower tube


			Line(tubes[i].x,0,tubes[i].gapl, GREEN);			// draw first upper tube      ! the screen is upside down !
			Line(tubes[i].x,tubes[i].gaph,239, GREEN);			// draw first lower tube






		}
		if((tubes[i].x == xposofst + 3) ){									// if 1st px of flpy is equal to 1st line of tube make the test
			if((flpy_h >= tubes[i].gaph)  || (flpy_h <= tubes[i].gapl)){	// test if flpy is outside the gap hight     LCD upside down all values fliped

		//		lifes--;

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








gappos tuberand(void){  // call RNG to work;    create 2 rand values with defined space inbetween
//min !must! be in range of LCD hight = 0 to (240 - 1)
                                           //
	uint32_t val = RandomNG();            //                                                                ____________________________________
    //                  max    min + 1     //                                                                \                 /                |
	uint8_t vall = (val % (LCD_hight - tubespace + 1));   // shrink val to LCD writeable space  vall % 190 ->  (2^32 - 1) - {[(2^32 - 1) / 190] - residual of} * 190  = (0,...,239)
	uint8_t valh = vall + tubespace;


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
	//int16_t xpos1;
	if(xpos1 > (LCD_width - xposofst)){          // compensate distance between left edge and flpy xpos
		xpos1 = xpos1 + xposofst - LCD_width ;
	}else{
		xpos1 = xpos1 + xposofst;
	}


	if(ypos1 <= 0){                    			// limits for highest/lowest pos of flpy    without limits flpy would appear on opposite edge
		ypos1 = 0;
	}else if(ypos1 >= LCD_hight - 3){
		ypos1 = LCD_hight - 3;
	}


/*	switch (xpos1){												// draw at x = 0 forbidden
	case 1://_________________________________________________________________________________________ 1|2|3|4   -> del previous pix case 319
		Line(319,yposold,yposold + 3,BLACK);
		Rect(1,3,yposold,yposold + 3,BLACK);	// del flpy
		Line(xpos1 + 3,ypos1,ypos1 + 3,WHITE);
		break;
	case 317://_______________________________________________________________________________________ 317|318|319|1
		Rect(xpos1 - 1,xpos1 + 3,yposold,yposold + 3,BLACK);    // del first line of previous rect
		Rect(xpos1, xpos1 + 2,ypos1,ypos1 + 3,WHITE);			//
		Line(LCD_width - xpos1 - 1,ypos1,ypos1 + 3,WHITE);		// draw new line at px pos 1
		break;
	case 318://_______________________________________________________________________________________ 318|319|1|2
		Rect(xpos1 - 1,xpos1 + 2,yposold,yposold + 3,BLACK);     	// del
		Line(1,yposold,yposold + 3, BLACK);                         // del Line at x = 1 from 317
		Rect(xpos1,xpos1 + 1,ypos1,ypos1 + 3,WHITE);
		Rect(1,LCD_width - xpos1 + 1,ypos1,ypos1 + 3,WHITE);			// new at px pos 2
		break;
	case 319://_______________________________________________________________________________________ 319|1|2|3
		Rect(xpos1 - 1,xpos1,yposold,yposold + 3,BLACK);
		Rect(1,2,yposold,yposold + 3,BLACK);     				// del
		Line(xpos1,ypos1,ypos1 + 3,WHITE);
		Rect(1,3,ypos1,ypos1 + 3,WHITE);		// new at px pos 3
		break;
	default:
	    Rect(xpos1 - 1,xpos1 + 2,yposold,yposold + 3,BLACK);	// del flpy
		Rect(xpos1,xpos1 + 3,ypos1,ypos1 + 3,WHITE);

	}*/

    Rect(xpos1,xpos1 + 3,yposold,yposold + 3,BLACK);	// del flpy
	Rect(xpos1,xpos1 + 3,ypos1,ypos1 + 3,WHITE);
  //  Line(xpos1 - 1,yposold,yposold + 3,BLACK);	// del pixels
//	Rect(xpos1,xpos1 + 3,ypos1,ypos1 + 3,WHITE);

	bck.rise = ypos1;										// abuse struct
	return bck; // keep old high for deleting objects
}


dummy flappy(void){
	static const uint8_t uses[10] = {10,0, 6,0, 3,0, 2,0, 1,0}; // rise Ary "0" slow down rise proce
	static uint8_t callcoun1;

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


uint8_t bird(_Bool pcpress){      // accelerated (by call cyc -> time) fallspeed
	static uint16_t callcoun2;
	//uint16_t t;

	if(pcpress){                // no fall, when jump
		callcoun2 = 0;
	}

	callcoun2++;
	if(callcoun2 > 35){        // set max fallspeed
		callcoun2 = 35;
	}
 uint8_t axa = (callcoun2 * callcoun2) / (faspeed * 90);
return (axa);	  // calculate final fallspeed         !!!chck on screen!!!

//	return 0;
}




int main() {
    DWT_Init();
    board_voltage();
	board_oclk(1);      // 1 means HSE -> 480MHz clk freq
	board_start();
    LCD_Init();
    delay(1000);
    scr_area(0b1,0x140,0);      //def first line as dummy to keep screen scrollable without artifcts

    gamecyc();


    tubespace = 100;
    xposofst = 59;
    tblen = 10;
   // Scrb_d scoreboard;			// build the scruct
    //scoreboard.lifes = 3;
   /* Tubesaver tubemap[4];
    scr_area(0b1,0x13F,0);      //def first line as dummy to keep screen scrollable without artifcts
    Line(0,0,LCD_hight,GREEN);
	for(;;){
			for(int i = 1;i < LCD_width + 1; i++){//
				Line(i,0,LCD_hight,BLACK);
				//dummy flpyhn = flappy_pos(i, flpyhn.rise);
				LCD_Scroll(i);
				delay_u(1740);
				delay_u(1740);
				tbcoun++;
				tblen++;
				if(tbcoun == tubedistance){                              // every 80 cycles new generation of a tube pair
					gappos gappos = tuberand();
					tubemap[tubecoun].gapl = gappos.gaplow;		// save new tube data
					tubemap[tubecoun].gaph = gappos.gaphigh;
					tubemap[tubecoun].x = 319;					// save start pos

					Line(i,0,gappos.gaplow, 204 * i);//
					Line(i,gappos.gaphigh,239, 204 * i);//
					tbcoun = 0;                                      // count passed empty rotations
					tblen = 0;                                       // make tube thickness function start
					tubecoun++;                             	// write next time into second space
				}
				if(tblen <= 10){											// create tube thickness
					Line(i,0,tubemap[tubecoun - 1].gapl, 97 * i);//
					Line(i,tubemap[tubecoun - 1].gaph,239, 97 * i);//
				}else if ((tblen > 10) & tubecoun >= 4){					// max 4 tube starting lines are displayed

					tubecoun = 0;
				}

				 //hit_tstf(tubemap,flpyhn.rise);


				if(i == 319){i = 0; if(tubespace > 50){tubespace--;}}  // make tubespace smaller over time
			}

	}*/


}




void gamecyc(void){
	Rect(0,LCD_width,0,LCD_hight,BLACK);

	for(;;){
		Tubesaver tubemap[4];
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
			//delay_u(1740);
			//delay_u(1740);

			tbcoun++;
			tblen++;
			if(tbcoun == tubedistance){                      		// every 80 cycles new generation of a tube pair
				gappos gappos = tuberand();
				tubemap[tubecoun].gapl = gappos.gaplow;				// save new tube data
				tubemap[tubecoun].gaph = gappos.gaphigh;
				tubemap[tubecoun].x = 319;							// save start pos

				Line(LCD_width,0,gappos.gaplow, 204 * i + 10);			// draw first upper tube      ! the screen is upside down !
				Line(LCD_width,gappos.gaphigh,239, 204 * i + 10);		// draw first lower tube

				tbcoun = 0;                                      	// count passed empty rotations
				tblen = 0;                                       	// make tube thickness function start
				tubecoun++;                             			// write next time into second space
			}

			if(tubecoun >= 4){
				tubecoun = 0;
			}


			hit_tstf(tubemap,flpyhn.rise);


			if(i == 1){i = 319; if(tubespace > 50){tubespace--;}}  // make tubespace smaller over time
		}




	}



}


// save tube
