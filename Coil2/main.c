
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include <output.h>
//#include <main.h>

#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 30
#define PIPE_WIDTH 3
#define GAP_HEIGHT 6
#define pipeCount 3
#define flappy_xpos 5

// Grundprinzip der Objektbewegung:
// Der Vogel bewegt sich automatisch nach unten durch Schwerkraft.
// Der Spieler kann durch Tastendruck den Vogel nach oben flattern lassen (Sprung).
// Hindernisse (Röhren) bewegen sich von rechts nach links.
// Der Vogel bleibt horizontal statisch, nur seine y-Achse ändert sich über die Zeit.




Bird bird;
Pipe pipes[pipeCount];
Score score;
bool running = true;  // Hauptspiel-Status

                                // Initialisierung von Vogel und Röhren
void initGame() {
    bird.y = SCREEN_HEIGHT / 2;
    bird.velocity = 1;

    for (int i = 0; i < pipeCount; i++) {
        pipes[i].x = SCREEN_WIDTH + i * (SCREEN_WIDTH / pipeCount);  // Röhren gleichmäßig verteilen
        pipes[i].gap_y = rand() % (SCREEN_HEIGHT - GAP_HEIGHT - 2) + 1;

       score.lifes = 3;
       score.tubepassed = 0;
    }
}

void coltst(Bird *bird, Pipe pipes[], int pipeCnt, bool *running){
    // Röhren bewegen sich nach links
        for (int i = 0; i < pipeCnt; i++) {
            pipes[i].x--;   // Röhren bewegen sich um 1 nach links

            if((flappy_xpos == pipes[i].x) && ((bird->y < pipes[i].gap_y) || (bird->y > pipes[i].gap_y + GAP_HEIGHT))){       // test if flpy has contact
                    score.lifes--;
                    if(score.lifes < 0){
                        *running = false;
                    }
            }else if((flappy_xpos == pipes[i].x) && ((bird->y >= pipes[i].gap_y) || (bird->y <= pipes[i].gap_y + GAP_HEIGHT))){
                score.tubepassed++;
            }

            // Wenn Röhre den linken Rand verlässt, wird sie neu generiert
            if (pipes[i].x + PIPE_WIDTH < 0) {
                pipes[i].x = SCREEN_WIDTH - 1;
                pipes[i].gap_y = rand() % (SCREEN_HEIGHT - GAP_HEIGHT - 2) + 1;
            }
        }

}


void movement(Bird *bird){
     // Schwerkraft: Die Fallgeschwindigkeit nimmt ohne Tastendruck zu
        bird->velocity += 0.2;
        bird->y += bird->velocity;

        // Spielsteuerung mit Leertaste
            char hit = keyinpt();
            if (hit == ' ') {       // Leertaste gedrückt
                bird->velocity = - 1.5 ;   // Der Vogel springt nach oben
            }

                // Überprüfung: Vogel verlässt den sichtbaren Bildschirm
        if (bird->y >= SCREEN_HEIGHT - 1){
            bird->y = SCREEN_HEIGHT - 1;
        }else if(bird->y < 0){
            bird->y = 0;
        }
}






                // Hauptfunktion
int main() {
    srand(time(NULL));  // Zufallszahl für Röhrenlücken
    initGame();

    while (running) {
        movement(&bird);                                                // movement of flappy

        coltst(&bird, pipes, pipeCount, &running);                      // movement of pipes and hit test

        drawScreen(&bird, pipes, pipeCount);         // Spielanzeige aktualisieren
        Sleep(50);        // Wartezeit für Spielfluss
    }

    return 0;
}
