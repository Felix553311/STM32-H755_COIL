#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include <stdbool.h>

#define SCREEN_WIDTH   50
#define SCREEN_HEIGHT  30
#define PIPE_WIDTH      3
#define GAP_HEIGHT      6
#define FLAPPY_XPOS     5



typedef struct {
    int lifes;
    int tubepassed;

} Score;

typedef struct {
    float y;          // Vertikale Position des Vogels
    float velocity;   // Geschwindigkeit des Vogels (nach unten positiv)
} Bird;


typedef struct {
    int x;       // Horizontale Position der Röhre
    int gap_y;   // Vertikale Position der Lücke in der Röhre
} Pipe;

void drawScreen(const Bird *bird, const Pipe pipes[], int pipeCnt);


char keyinpt(void) ;

extern Score score;


#endif // OUTPUT_H_INCLUDED
