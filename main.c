#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "include/game.h"
#include "include/story.h"
#include "include/character.h"

int main() {
    // Initialize game systems
    GameState* game = initializeGame();
    if (!game) {
        fprintf(stderr, "Failed to initialize game. Exiting...\n");
        return 1;
    }

    // Display welcome message
    clear();
    attron(COLOR_PAIR(COLOR_TITLE_PAIR) | A_BOLD);
    mvprintw(LINES/2 - 2, (COLS - 33)/2, "=================================");
    mvprintw(LINES/2 - 1, (COLS - 33)/2, "Welcome to The Chronicles of Destiny");
    mvprintw(LINES/2, (COLS - 33)/2, "Where Your Choices Shape The World");
    mvprintw(LINES/2 + 1, (COLS - 33)/2, "=================================");
    attroff(COLOR_PAIR(COLOR_TITLE_PAIR) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_NORMAL_PAIR));
    mvprintw(LINES/2 + 3, (COLS - 24)/2, "Press any key to begin...");
    attroff(COLOR_PAIR(COLOR_NORMAL_PAIR));
    
    refresh();
    getch();

    // Start the game loop
    while (!game->isGameOver) {
        displayCurrentScene(game);
        processPlayerChoice(game);
        updateGameState(game);
    }

    // Clean up
    cleanupGame(game);
    return 0;
} 