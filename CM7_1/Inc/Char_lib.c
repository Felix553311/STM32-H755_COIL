/*
 * board.c
 *
 *  Created on: May 27, 2025
 *      Author: felix
 */

/**
 ******************************************************************************
 * @file           : board.c
 * @date		   : June 8, 2025
 * @author         : Felix Bahrke
 * @brief          : contains the character library for the ILI9341
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
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include <LCD_draw_lib.h>
#include <Char_lib.h>



// https://www.piskelapp.com/p/create/sprite/

const digseg zero1[] ={																// define segment types and relative pos
		{ .type = pix, .sX = 2, .sY = 2 },
		{ .type = pix, .sX = 5, .sY = 2 },
		{ .type = pix, .sX = 2, .sY = 8 },
		{ .type = pix, .sX = 5, .sY = 8 },
		{ .type = hor, .sX = 2, .sY = 0, .line.length = 3 },
		{ .type = hor, .sX = 1, .sY = 1, .line.length = 5 },
		{ .type = hor, .sX = 1, .sY = 9, .line.length = 5 },
		{ .type = hor, .sX = 2, .sY = 10, .line.length = 3 },
		{ .type = rec, .sX = 0, .sY = 2, .line.rect = { .recw = 1, .rech = 6 } },
		{ .type = rec, .sX = 6, .sY = 2, .line.rect = { .recw = 1, .rech = 6 } }

};

const Bmdef zero ={																	// define bitmap properties
		.length = zero1,
		.segcount = sizeof(zero1) / sizeof(digseg),
		.width = 7,
		.height = 11
};

const digseg one1[] ={
		{ .type = ver, .sX = 1, .sY = 6, .line.length = 1 },
		{ .type = ver, .sX = 2, .sY = 6, .line.length = 2 },
		{ .type = ver, .sX = 3, .sY = 7, .line.length = 2 },
		{ .type = ver, .sX = 4, .sY = 8, .line.length = 2 },
		{ .type = rec, .sX = 5, .sY = 0, .line.rect = { .recw = 1, .rech = 10 } }
};


const Bmdef one ={
		.length = one1,
		.segcount = sizeof(one1) / sizeof(digseg),
		.width = 7,
		.height = 11
};


const digseg two1[] ={
		{ .type = hor, .sX = 0, .sY = 7, .line.length = 1 },
		{ .type = hor, .sX = 0, .sY = 8, .line.length = 2 },
		{ .type = hor, .sX = 1, .sY = 9, .line.length = 4 },
		{ .type = hor, .sX = 2, .sY = 10, .line.length = 2 },
		{ .type = hor, .sX = 4, .sY = 8, .line.length = 2 },
		{ .type = hor, .sX = 5, .sY = 7, .line.length = 1 },
		{ .type = hor, .sX = 4, .sY = 6, .line.length = 2 },
		{ .type = hor, .sX = 3, .sY = 5, .line.length = 2 },
		{ .type = hor, .sX = 2, .sY = 4, .line.length = 2 },
		{ .type = hor, .sX = 1, .sY = 3, .line.length = 2 },
		{ .type = hor, .sX = 0, .sY = 2, .line.length = 2 },
		{ .type = rec, .sX = 0, .sY = 0, .line.rect = { .recw = 6, .rech = 1 } }
};


const Bmdef two ={
		.length = two1,
		.segcount = sizeof(two1) / sizeof(digseg),
		.width = 7,
		.height = 11
};


const digseg three1[] ={
		{ .type = hor, .sX = 0, .sY = 8, .line.length = 1 },
		{ .type = hor, .sX = 0, .sY = 9, .line.length = 5 },
		{ .type = hor, .sX = 1, .sY = 10, .line.length = 3 },
		{ .type = hor, .sX = 4, .sY = 8, .line.length = 2 },
		{ .type = hor, .sX = 5, .sY = 7, .line.length = 1 },
		{ .type = hor, .sX = 4, .sY = 6, .line.length = 2 },
		{ .type = hor, .sX = 3, .sY = 5, .line.length = 2 },
		{ .type = hor, .sX = 4, .sY = 4, .line.length = 2 },
		{ .type = hor, .sX = 5, .sY = 3, .line.length = 1 },
		{ .type = hor, .sX = 4, .sY = 2, .line.length = 2 },
		{ .type = hor, .sX = 1, .sY = 0, .line.length = 3 },
		{ .type = hor, .sX = 0, .sY = 1, .line.length = 5 },
		{ .type = hor, .sX = 0, .sY = 2, .line.length = 1 }
};


const Bmdef three ={
		.length = three1,
		.segcount = sizeof(three1) / sizeof(digseg),
		.width = 7,
		.height = 11
};


const digseg four1[] ={
		{ .type = ver, .sX = 0, .sY = 5, .line.length = 1 },
		{ .type = ver, .sX = 1, .sY = 5, .line.length = 2 },
		{ .type = ver, .sX = 2, .sY = 6, .line.length = 2 },
		{ .type = ver, .sX = 3, .sY = 7, .line.length = 2 },
		{ .type = ver, .sX = 4, .sY = 8, .line.length = 2 },
		{ .type = rec, .sX = 5, .sY = 0, .line.rect = { .recw = 1, .rech = 10 } },
		{ .type = rec, .sX = 0, .sY = 3, .line.rect = { .recw = 4, .rech = 1 } }
};


const Bmdef four ={
		.length = four1,
		.segcount = sizeof(four1) / sizeof(digseg),
		.width = 7,
		.height = 11
};


const digseg five1[] ={
		{ .type = pix, .sX = 4, .sY = 2 },
		{ .type = pix, .sX = 4, .sY = 4 },
		{ .type = ver, .sX = 5, .sY = 2, .line.length = 2 },
		{ .type = ver, .sX = 6, .sY = 1, .line.length = 4 },
		{ .type = rec, .sX = 0, .sY = 9, .line.rect = { .recw = 6, .rech = 1 } },
		{ .type = rec, .sX = 0, .sY = 7, .line.rect = { .recw = 1, .rech = 1 } },
		{ .type = rec, .sX = 0, .sY = 5, .line.rect = { .recw = 5, .rech = 1 } },
		{ .type = rec, .sX = 0, .sY = 0, .line.rect = { .recw = 5, .rech = 1 } }
};


const Bmdef five ={
		.length = five1,
		.segcount = sizeof(five1) / sizeof(digseg),
		.width = 7,
		.height = 11
};


const digseg six1[] ={
		{ .type = hor, .sX = 1, .sY = 0, .line.length = 4 },
		{ .type = hor, .sX = 2, .sY = 1, .line.length = 4 },
		{ .type = hor, .sX = 5, .sY = 8, .line.length = 1 },
		{ .type = hor, .sX = 2, .sY = 9, .line.length = 4 },
		{ .type = hor, .sX = 1, .sY = 10, .line.length = 4 },
		{ .type = ver, .sX = 6, .sY = 2, .line.length = 2 },
		{ .type = ver, .sX = 5, .sY = 2, .line.length = 1 },
		{ .type = rec, .sX = 0, .sY = 1, .line.rect = { .recw = 1, .rech = 8 } },
		{ .type = rec, .sX = 2, .sY = 4, .line.rect = { .recw = 3, .rech = 1 } }
};


const Bmdef six ={
		.length = six1,
		.segcount = sizeof(six1) / sizeof(digseg),
		.width = 7,
		.height = 11
};


const digseg seven1[] ={
		{ .type = hor, .sX = 5, .sY = 8, .line.length = 1 },
		{ .type = hor, .sX = 4, .sY = 7, .line.length = 2 },
		{ .type = hor, .sX = 3, .sY = 6, .line.length = 2 },
		{ .type = hor, .sX = 2, .sY = 5, .line.length = 2 },
		{ .type = rec, .sX = 0, .sY = 9, .line.rect = { .recw = 6, .rech = 1 } },
		{ .type = rec, .sX = 2, .sY = 0, .line.rect = { .recw = 1, .rech = 4 } }
};


const Bmdef seven ={
		.length = seven1,
		.segcount = sizeof(seven1) / sizeof(digseg),
		.width = 4,
		.height = 7
};


const digseg eight1[] ={
		{ .type = pix, .sX = 1, .sY = 5 },
		{ .type = pix, .sX = 5, .sY = 5 },
		{ .type = hor, .sX = 1, .sY = 0, .line.length = 4 },
		{ .type = hor, .sX = 0, .sY = 1, .line.length = 6 },
		{ .type = hor, .sX = 0, .sY = 9, .line.length = 6 },
		{ .type = hor, .sX = 1, .sY = 10, .line.length = 4 },
		{ .type = rec, .sX = 0, .sY = 2, .line.rect = { .recw = 1, .rech = 2 } },
		{ .type = rec, .sX = 5, .sY = 2, .line.rect = { .recw = 1, .rech = 2 } },
		{ .type = rec, .sX = 2, .sY = 4, .line.rect = { .recw = 2, .rech = 2 } },
		{ .type = rec, .sX = 0, .sY = 6, .line.rect = { .recw = 1, .rech = 2 } },
		{ .type = rec, .sX = 5, .sY = 6, .line.rect = { .recw = 1, .rech = 2 } }
};


const Bmdef eight ={
		.length = eight1,
		.segcount = sizeof(eight1) / sizeof(digseg),
		.width = 7,
		.height = 11
};


const digseg nine1[] ={
		{ .type = hor, .sX = 1, .sY = 0, .line.length = 4 },
		{ .type = hor, .sX = 0, .sY = 1, .line.length = 4 },
		{ .type = hor, .sX = 0, .sY = 2, .line.length = 1 },
		{ .type = hor, .sX = 0, .sY = 9, .line.length = 4 },
		{ .type = hor, .sX = 1, .sY = 10, .line.length = 4 },
		{ .type = ver, .sX = 0, .sY = 6, .line.length = 3 },
		{ .type = ver, .sX = 1, .sY = 7, .line.length = 1 },
		{ .type = rec, .sX = 5, .sY = 1, .line.rect = { .recw = 1, .rech = 8 } },
		{ .type = rec, .sX = 1, .sY = 5, .line.rect = { .recw = 3, .rech = 1 } }
};


const Bmdef nine ={
		.length = nine1,
		.segcount = sizeof(nine1) / sizeof(digseg),
		.width = 4,
		.height = 7
};



const Bmdef *digbit[10] ={
		&zero,	//0
		&one,	//1
		&two,	//2
		&three,	//3
		&four,	//4
		&five,	//5
		&six,	//6
		&seven,	//7
		&eight,	//8
		&nine,	//9
};



// Zeichnet ein Glyph an der Position (xpos, ypos) in der Farbe fgColor.
void sm_chars(const Bmdef *g, uint16_t xpos, uint16_t ypos, uint16_t digcol){		// read every segment of the chosen char and decide how to draw it
    for (size_t i = 0; i < g->segcount; i++) {
        digseg seg = g->length[i];
        switch (seg.type) {
        	case pix:
        		Pixel(xpos + seg.sX, ypos + seg.sY, digcol);
        		break;
            case hor:
            	Line_h(xpos + seg.sX, xpos + seg.sX + seg.line.length, ypos + seg.sY, digcol);
                break;
            case ver:
            	Line(xpos + seg.sX, ypos + seg.sY, ypos + seg.sY + seg.line.length, digcol);
                break;
            case rec:
            	Rect(xpos + seg.sX, xpos + seg.line.rect.recw + seg.sX, ypos + seg.sY, ypos + seg.line.rect.rech + seg.sY, digcol);
                break;
            default:
                break;
        }
    }
}










