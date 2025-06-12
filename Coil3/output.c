/**
 ******************************************************************************
 * @file           : output.c
 * @date		   : June 10, 2025
 * @author         : Juan Ignacio Fogolin Lagares, Santiago Resnik, Hyunjong Lee, Felix Bahrke
 * @brief          : contains the frontend functions
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 by the author.
 * All rights reserved.
 *
 * This file is part of the intellectual property of the owners.
 * The replication ensued under caveate of non commercial use.
 * The owner takes no liability for any damage cause by using content of this file.
 *
 ******************************************************************************
 */




#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include <output.h>




void drawScreen(const Bird *bird, const Pipe pipes[], int pipeCnt){
    system("cls");                                                          // delete the terminal (in Windows)

    printf("Score: %3d    Lives: %d\n\n",  score.tubepassed, score.lifes);  // print score to the screen

    for (int y = 0; y < SCREEN_HEIGHT; ++y){                                // move draw pos to every space on screen
        for (int x = 0; x < SCREEN_WIDTH; ++x){                             //
            if ((int)bird->y == y && x == flappy_xpos){                     // if draw pos is at flappys calculated pos
                putchar('O');                                               // draw flpy character
                continue;                                                   // avoid shift in tubas at y pos of flpy
            }

            bool isPipe = false;
            for (int i = 0; i < pipeCnt; ++i){                                              // cheack for tubes calculated pos at actual drawing pos
                bool xRange = (x >= pipes[i].x) && (x <  pipes[i].x + PIPE_WIDTH);
                bool yRange = (y <  pipes[i].gap_y) || (y >  pipes[i].gap_y + GAP_HEIGHT);

                if (xRange && yRange){                                                      // only draw till defined height and width reached
                    isPipe = true;
                    break;
                }
            }
            putchar(isPipe ? '|' : ' ');                                                    // draws tube character if "isPipe" = 1 or a spce if it is 0
        }
        putchar('\n');                                                                      // move cursor to new line
    }
}


char keyinpt(void) {
    if (_kbhit()) {         // test for key input
        return _getch();    // return the character detected
    }
    return '\0';            // return "nothing" if no input detected
}





void startscreen(Score *score){                             // print start screen
    system("cls");
    printf("*********************************\n");
    printf("*      Flappybird COIL 2025     *\n");
    printf("*********************************\n\n");


    printf("Enter your name (30 chars max) and start by pressing enter:\n> ");
    fflush(stdout);                                                             // make sure printed text shows up before folowing code

    if (fgets(score->name, sizeof(score->name), stdin)){                        // writing into name variable till new line or max len reached
        // Trailing Newline entfernen
        size_t len = strlen(score->name);                                       // calculate number of chars without termination char
        if (len > 0 && score->name[len-1] == '\n'){                             // delete the "enter" char
            score->name[len-1] = '\0';                                          // save with "clean" termination
        }
    }
    printf("\nstarting game...");
    Sleep(1000);
    system("cls");
}





void savegame(const Score *score){
    FILE *f = fopen("scores.txt", "a");                                      // open the file "scores.txt without changes to previous contents or create a new one if no file is detected
    if (!f) {                                                                // if file could not be open
        perror("error writing to scores.txt");
        return;
    }
    fprintf(f, "name: %s | score: %d\n", score->name, score->tubepassed);    // write the name as string and score as decimal from score struct with a new line at end
    fclose(f);
}
