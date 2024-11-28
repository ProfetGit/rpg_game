/**
 * @file game.h
 * @brief Core game functionality and state management
 * @details Defines the game state structure and core functions for managing the game
 */

#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include "character.h"

#define MAX_INVENTORY_SIZE 10
#define MAX_CHOICE_TEXT 100
#define MAX_CHOICES 4

/**
 * @brief Color pair definitions for ncurses display
 */
#define COLOR_TITLE_PAIR 1    /**< Color pair for title text */
#define COLOR_HEADER_PAIR 2   /**< Color pair for headers */
#define COLOR_NORMAL_PAIR 3   /**< Color pair for normal text */
#define COLOR_CHOICE_PAIR 4   /**< Color pair for choices */
#define COLOR_STAT_PAIR 5     /**< Color pair for statistics */
#define COLOR_ERROR_PAIR 6    /**< Color pair for error messages */

// Forward declarations
struct StoryNode;

/**
 * @struct GameState
 * @brief Main game state structure containing all game-related data
 */
typedef struct GameState {
    Character* player;                    /**< Pointer to the player character */
    struct StoryNode* currentScene;       /**< Current story scene */
    int currentChapter;                   /**< Current chapter number */
    int reputation;                       /**< Player's reputation score */
    int isGameOver;                       /**< Flag indicating if game is over */
    char inventory[MAX_INVENTORY_SIZE][50];  /**< Player's inventory items */
    int inventoryCount;                   /**< Number of items in inventory */
    int choiceHistory[100];              /**< History of player choices */
    int choiceHistoryCount;              /**< Number of choices made */
} GameState;

/**
 * @brief Initializes a new game state
 * @return Pointer to the newly created game state
 */
GameState* initializeGame(void);

/**
 * @brief Displays the current scene to the player
 * @param game Pointer to the current game state
 */
void displayCurrentScene(GameState* game);

/**
 * @brief Processes the player's choice in the current scene
 * @param game Pointer to the current game state
 */
void processPlayerChoice(GameState* game);

/**
 * @brief Updates the game state based on player actions
 * @param game Pointer to the current game state
 */
void updateGameState(GameState* game);

/**
 * @brief Cleans up and frees all game resources
 * @param game Pointer to the game state to clean up
 */
void cleanupGame(GameState* game);

/**
 * @brief Saves the current game state to a file
 * @param game Pointer to the game state to save
 */
void saveGame(GameState* game);

/**
 * @brief Loads a saved game state from a file
 * @return Pointer to the loaded game state
 */
GameState* loadGame(void);

/**
 * @brief Adds an item to the player's inventory
 * @param game Pointer to the current game state
 * @param item Name of the item to add
 */
void addToInventory(GameState* game, const char* item);

/**
 * @brief Removes an item from the player's inventory
 * @param game Pointer to the current game state
 * @param item Name of the item to remove
 */
void removeFromInventory(GameState* game, const char* item);

/**
 * @brief Initializes color pairs for ncurses display
 */
void initializeColors(void);

/**
 * @brief Cleans up the display system
 */
void cleanupDisplay(void);

#endif 