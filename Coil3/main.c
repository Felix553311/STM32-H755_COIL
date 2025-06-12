/**
 ******************************************************************************
 * @file           : main.c
 * @date		   : May 8, 2025
 * @author         : Juan Ignacio Fogolin Lagares, Santiago Resnik, Hyunjong Lee, Felix Bahrke
 * @brief          : contains the backend functions
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

#include <output.h>






//declarations
Bird bird;
Pipe pipes[pipeCount];
Score score;
bool running = true;  //


void initGame(){   // initialisation of tubes and reset of score and bird start pos
    bird.y = SCREEN_HEIGHT / 2; //start pos of flappy
    bird.velocity = 1;          // reset fallspeed

    for (int i = 0; i < pipeCount; i++){// preload tubes
        pipes[i].x = SCREEN_WIDTH + i * (SCREEN_WIDTH / pipeCount);
        pipes[i].gap_y = rand() % (SCREEN_HEIGHT - GAP_HEIGHT - 2) + 1;
        // reset score
       score.lifes = 3;
       score.tubepassed = 0;

    }
}



void coltst(Bird *bird, Pipe pipes[], int pipeCnt, bool *running){
    for (int i = 0; i < pipeCnt; i++){      // move pipe pos by 1 horizontally
        pipes[i].x--;

        if((flappy_xpos == pipes[i].x) && ((bird->y < pipes[i].gap_y) || (bird->y > pipes[i].gap_y + GAP_HEIGHT))){       // test if flpy has contact
                score.lifes--;                                                                                            // reduce lifes if contact detected
                if(score.lifes < 0){                                                                                      // if no lifes remaining save score to file and go back to start screen
                    savegame(&score);
                    printf("\nGame Over! Restarting...\n");
                    Sleep(5000);        // wait 5sec
                    startscreen(&score);
                    initGame();    // reset everything
                    return;        // leave coltst function and return to game loop

                }
        }else if((flappy_xpos == pipes[i].x) && ((bird->y >= pipes[i].gap_y) || (bird->y <= pipes[i].gap_y + GAP_HEIGHT))){ // if no contact detected increase score by 1
            score.tubepassed++;
        }

        if (pipes[i].x + PIPE_WIDTH < 0) {                                                                                  // if pipe leaves left corner generate a new one
            pipes[i].x = SCREEN_WIDTH - 1;
            pipes[i].gap_y = rand() % (SCREEN_HEIGHT - GAP_HEIGHT - 2) + 1;
        }
    }
}


void movement(Bird *bird){
    bird->velocity += 0.2;          //gravity cause increasing fallspeed
    bird->y += bird->velocity;

        char hit = keyinpt();           // read the key input
        if (hit == ' ') {               // if space is pressed
            bird->velocity = - 1.5 ;    // bird jumps up
        }

    if (bird->y >= SCREEN_HEIGHT - 1){  // test if bird leaves lower edge
        bird->y = SCREEN_HEIGHT - 1;    // restore flpy pos to lower edge
    }else if(bird->y < 0){              // test if flpy raises above upper edge
        bird->y = 0;                    // restore
    }
}

int main(){
    startscreen(&score);                             // first time start screen

    srand(time(NULL));                               // generating random numbers for tupes
    initGame();                                      // first init

    while (running) {                                // game loop
        movement(&bird);                             // calculate movement of flappy

        coltst(&bird, pipes, pipeCount, &running);   // movement of pipes and hit test

        drawScreen(&bird, pipes, pipeCount);         // Spielanzeige aktualisieren
        Sleep(50);                                   // small delay to slow down movements/sec
    }
    return 0;
}
