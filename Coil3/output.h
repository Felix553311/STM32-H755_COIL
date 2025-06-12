/**
 ******************************************************************************
 * @file           : output.h
 * @date		   : May 10, 2025
 * @author         : Juan Ignacio Fogolin Lagares, Santiago Resnik, Hyunjong Lee, Felix Bahrke
 * @brief          : contains struct and function definitions, constant values
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

#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include <stdbool.h>

#define SCREEN_WIDTH   60
#define SCREEN_HEIGHT  20
#define PIPE_WIDTH      3
#define GAP_HEIGHT      6
#define pipeCount       3
#define flappy_xpos     5


typedef struct {
    int lifes;        // number of remaining lifes
    int tubepassed;   // number of passed tubes
    char name[31];    // 30 chars + termination
} Score;

typedef struct {
    float y;          // height of flappy
    float velocity;   // fall speed
} Bird;


typedef struct {
    int x;       // horizontal tube pos
    int gap_y;   // height of gap position
} Pipe;

void drawScreen(const Bird *bird, const Pipe pipes[], int pipeCnt);

char keyinpt(void);

void startscreen(Score *score);

void savegame(const Score *score);

extern Score score;                 // tell compiler score is defined but not here


#endif // OUTPUT_H_INCLUDED
