#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "../include/game.h"
#include "../include/story.h"

void initializeColors(void) {
    // Start color functionality
    start_color();
    // Make sure we have a black background
    use_default_colors();
    assume_default_colors(-1, -1);
    
    // Initialize color pairs with transparent (-1) background
    init_pair(COLOR_TITLE_PAIR, COLOR_YELLOW, -1);
    init_pair(COLOR_HEADER_PAIR, COLOR_GREEN, -1);
    init_pair(COLOR_NORMAL_PAIR, COLOR_WHITE, -1);
    init_pair(COLOR_CHOICE_PAIR, COLOR_CYAN, -1);
    init_pair(COLOR_STAT_PAIR, COLOR_MAGENTA, -1);
    init_pair(COLOR_ERROR_PAIR, COLOR_RED, -1);
}

GameState* initializeGame(void) {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    
    // Hide cursor
    curs_set(0);
    
    if (has_colors()) {
        initializeColors();
    }
    
    GameState* game = (GameState*)malloc(sizeof(GameState));
    if (!game) return NULL;

    game->player = createCharacter();
    if (!game->player) {
        free(game);
        return NULL;
    }

    game->currentScene = initializeStory();
    game->currentChapter = 1;
    game->reputation = 0;
    game->isGameOver = 0;
    game->inventoryCount = 0;
    game->choiceHistoryCount = 0;

    return game;
}

void displayCurrentScene(GameState* game) {
    clear();
    
    // Display chapter title
    attron(COLOR_PAIR(COLOR_TITLE_PAIR) | A_BOLD);
    mvprintw(1, 1, "=== Chapter %d ===", game->currentChapter);
    attroff(COLOR_PAIR(COLOR_TITLE_PAIR) | A_BOLD);
    
    // Display scene description
    attron(COLOR_PAIR(COLOR_NORMAL_PAIR));
    mvprintw(3, 1, "%s", game->currentScene->description);
    attroff(COLOR_PAIR(COLOR_NORMAL_PAIR));
    
    // Display choices
    attron(COLOR_PAIR(COLOR_CHOICE_PAIR));
    mvprintw(6, 1, "What will you do?\n\n");
    for (int i = 0; i < game->currentScene->numChoices; i++) {
        mvprintw(8 + i, 1, "%d. %s", i + 1, game->currentScene->choices[i]);
        const int* req = game->currentScene->requirements[i];
        if (req[0] > 0 || req[1] > 0 || req[2] > 0) {
            attron(COLOR_PAIR(COLOR_STAT_PAIR));
            printw(" (Requires: ");
            if (req[0] > 0) printw("STR %d ", req[0]);
            if (req[1] > 0) printw("INT %d ", req[1]);
            if (req[2] > 0) printw("CHA %d", req[2]);
            printw(")");
            attroff(COLOR_PAIR(COLOR_STAT_PAIR));
        }
        printw("\n");
    }
    attroff(COLOR_PAIR(COLOR_CHOICE_PAIR));
    
    refresh();
}

void processPlayerChoice(GameState* game) {
    mvprintw(LINES - 2, 1, "Enter your choice (1-%d): ", game->currentScene->numChoices);
    refresh();
    
    char input[10];
    echo();
    getnstr(input, sizeof(input) - 1);
    noecho();
    
    int choice = atoi(input);

    if (choice < 1 || choice > game->currentScene->numChoices) {
        attron(COLOR_PAIR(COLOR_ERROR_PAIR));
        mvprintw(LINES - 1, 1, "Invalid choice. Press any key to try again...");
        attroff(COLOR_PAIR(COLOR_ERROR_PAIR));
        refresh();
        getch();
        return;
    }

    // Check if player meets the requirements for this choice
    int* req = game->currentScene->requirements[choice - 1];
    if (!hasRequiredStats(game->player, req[0], req[1], req[2])) {
        attron(COLOR_PAIR(COLOR_ERROR_PAIR));
        mvprintw(LINES - 1, 1, "You don't meet the requirements for this choice. Press any key...");
        attroff(COLOR_PAIR(COLOR_ERROR_PAIR));
        refresh();
        getch();
        return;
    }

    game->choiceHistory[game->choiceHistoryCount++] = choice;

    if (game->currentScene->consequence) {
        game->currentScene->consequence(game);
    }

    game->currentScene = game->currentScene->nextNodes[choice - 1];
}

void cleanupDisplay(void) {
    endwin();
}

void cleanupGame(GameState* game) {
    if (game) {
        if (game->player) {
            free(game->player);
        }
        cleanupStory(game->currentScene);
        free(game);
    }
    cleanupDisplay();
}

void updateGameState(GameState* game) {
    if (!game->currentScene) {
        game->isGameOver = 1;
    }
}

void saveGame(GameState* game) {
    FILE* file = fopen("savegame.dat", "wb");
    if (!file) {
        printf("Error: Could not save game.\n");
        return;
    }

    fwrite(game, sizeof(GameState), 1, file);
    fclose(file);
    printf("Game saved successfully!\n");
}

GameState* loadGame(void) {
    FILE* file = fopen("savegame.dat", "rb");
    if (!file) {
        printf("No saved game found.\n");
        return NULL;
    }

    GameState* game = (GameState*)malloc(sizeof(GameState));
    if (!game) {
        fclose(file);
        return NULL;
    }

    fread(game, sizeof(GameState), 1, file);
    fclose(file);
    return game;
}

void addToInventory(GameState* game, const char* item) {
    if (game->inventoryCount < MAX_INVENTORY_SIZE) {
        strncpy(game->inventory[game->inventoryCount], item, 49);
        game->inventory[game->inventoryCount][49] = '\0';
        game->inventoryCount++;
    }
} 