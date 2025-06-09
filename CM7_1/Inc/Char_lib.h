/*
 * Char_lib.h
 *
 *  Created on: Jun 8, 2025
 *      Author: Felix
 */

#ifndef CHAR_LIB_H_
#define CHAR_LIB_H_


#include <stdint.h>
#include <stddef.h>



typedef enum{		// define types of segment
	pix,
	hor,
	ver,
	rec
}segtype;



typedef struct {
    segtype type;   // type of the segment
    uint8_t sX;     // relative px pos inside the bitmap
    uint8_t sY;     //
    union {
        uint8_t length;		// for writing lines
        struct {			// for rectangulars
            uint8_t recw;   // width
            uint8_t rech;  	// height
        }rect;
    }line;
}digseg;





typedef struct{
	const digseg *length;		// size of the Bitmap ary
	size_t segcount;			// number of defined segments
    uint8_t width;           // width and hight of writeable space
    uint8_t height;
}Bmdef;


extern const Bmdef *digbit[10];		// lookup ary


extern const Bmdef zero;			// inside ary
extern const Bmdef one;
extern const Bmdef two;
extern const Bmdef three;
extern const Bmdef four;
extern const Bmdef five;
extern const Bmdef six;
extern const Bmdef seven;
extern const Bmdef eight;
extern const Bmdef nine;


void sm_chars(const Bmdef *g, uint16_t xpos, uint16_t ypos, uint16_t digcol);


#endif /* CHAR_LIB_H_ */
