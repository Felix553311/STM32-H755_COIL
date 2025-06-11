
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <windows.h>


#define _CRT_SECURE_NO_WARNINGS  /* für MSVC/MinGW */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>             /* system("cls"), Sleep   */
#include <output.h>
#include <main.h>



void drawScreen(const Bird *bird, const Pipe pipes[], int pipeCnt) {
    system("cls");   /* Terminal löschen (Windows) */

        // ── Score in der oberen Zeile ──
    printf("Score: %3d    Lives: %d\n\n",
           score.tubepassed, score.lifes);



    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        for (int x = 0; x < SCREEN_WIDTH; ++x)
        {
            /* Vogel? */
            if ((int)bird->y == y && x == FLAPPY_XPOS)
            {
                putchar('O');
                continue;
            }

            /* Hindernisse? */
            bool isPipe = false;
            for (int i = 0; i < pipeCnt; ++i)
            {
                bool xRange = (x >= pipes[i].x) &&
                              (x <  pipes[i].x + PIPE_WIDTH);

                bool yRange = (y <  pipes[i].gap_y) ||
                              (y >  pipes[i].gap_y + GAP_HEIGHT);

                if (xRange && yRange)
                {
                    isPipe = true;
                    break;
                }
            }
            putchar(isPipe ? '|' : ' ');
        }
        putchar('\n');
    }
}


char keyinpt(void) {
    if (_kbhit()) {   // Prüfen, ob eine Tasteneingabe vorliegt
        return _getch();
    }
    return '\0';
}

