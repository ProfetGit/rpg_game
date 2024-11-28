#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include "character.h"

#define MAX_INVENTORY_SIZE 10
#define MAX_CHOICE_TEXT 100
#define MAX_CHOICES 4

// Color pair definitions
#define COLOR_TITLE_PAIR 1
#define COLOR_HEADER_PAIR 2
#define COLOR_NORMAL_PAIR 3
#define COLOR_CHOICE_PAIR 4
#define COLOR_STAT_PAIR 5
#define COLOR_ERROR_PAIR 6

// Forward declarations
struct StoryNode;

typedef struct GameState {
    Character* player;
    struct StoryNode* currentScene;
    int currentChapter;
    int reputation;
    int isGameOver;
    char inventory[MAX_INVENTORY_SIZE][50];
    int inventoryCount;
    int choiceHistory[100];
    int choiceHistoryCount;
} GameState;

// Core game functions
GameState* initializeGame(void);
void displayCurrentScene(GameState* game);
void processPlayerChoice(GameState* game);
void updateGameState(GameState* game);
void cleanupGame(GameState* game);

// Utility functions
void saveGame(GameState* game);
GameState* loadGame(void);
void addToInventory(GameState* game, const char* item);
void removeFromInventory(GameState* game, const char* item);

// Display functions
void initializeColors(void);
void cleanupDisplay(void);

#endif 