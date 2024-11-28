#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "../include/story.h"
#include "../include/game.h"

StoryNode* createStoryNode(int id, const char* description) {
    StoryNode* node = (StoryNode*)malloc(sizeof(StoryNode));
    if (!node) return NULL;

    node->id = id;
    strncpy(node->description, description, MAX_DESCRIPTION_LENGTH - 1);
    node->description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    node->numChoices = 0;
    node->consequence = NULL;

    return node;
}

void addChoice(StoryNode* node, const char* choiceText, StoryNode* nextNode, int reqStr, int reqInt, int reqCha) {
    if (node->numChoices < MAX_CHOICES) {
        strncpy(node->choices[node->numChoices], choiceText, MAX_CHOICE_TEXT - 1);
        node->choices[node->numChoices][MAX_CHOICE_TEXT - 1] = '\0';
        node->nextNodes[node->numChoices] = nextNode;
        node->requirements[node->numChoices][0] = reqStr;
        node->requirements[node->numChoices][1] = reqInt;
        node->requirements[node->numChoices][2] = reqCha;
        node->numChoices++;
    }
}

void displayChoices(const StoryNode* node, const GameState* game) {
    attron(COLOR_PAIR(COLOR_CHOICE_PAIR));
    mvprintw(6, 1, "What will you do?\n\n");
    
    int y = 8;
    for (int i = 0; i < node->numChoices; i++) {
        mvprintw(y, 1, "%d. %s", i + 1, node->choices[i]);
        const int* req = node->requirements[i];
        
        // Check if the player meets the requirements and color accordingly
        if (req[0] > 0 || req[1] > 0 || req[2] > 0) {
            attron(COLOR_PAIR(COLOR_STAT_PAIR));
            printw(" (Requires: ");
            if (req[0] > 0) {
                if (game->player->strength >= req[0]) {
                    attron(COLOR_PAIR(COLOR_HEADER_PAIR));
                } else {
                    attron(COLOR_PAIR(COLOR_ERROR_PAIR));
                }
                printw("STR %d ", req[0]);
            }
            if (req[1] > 0) {
                if (game->player->intelligence >= req[1]) {
                    attron(COLOR_PAIR(COLOR_HEADER_PAIR));
                } else {
                    attron(COLOR_PAIR(COLOR_ERROR_PAIR));
                }
                printw("INT %d ", req[1]);
            }
            if (req[2] > 0) {
                if (game->player->charisma >= req[2]) {
                    attron(COLOR_PAIR(COLOR_HEADER_PAIR));
                } else {
                    attron(COLOR_PAIR(COLOR_ERROR_PAIR));
                }
                printw("CHA %d", req[2]);
            }
            attron(COLOR_PAIR(COLOR_STAT_PAIR));
            printw(")");
        }
        y++;
    }
    attroff(COLOR_PAIR(COLOR_CHOICE_PAIR));
    refresh();
}

// Chapter 1 story content
StoryNode* createChapterOne(void) {
    // Opening scene
    StoryNode* start = createStoryNode(1, 
        "The ancient city of Eldara lies before you, its towering spires reaching towards the crimson sunset. "
        "You've traveled far to reach this place, drawn by rumors of a powerful artifact hidden within its walls. "
        "As you approach the city gates, you notice three distinct paths..."
    );

    // Path choices
    StoryNode* mainGate = createStoryNode(2,
        "You approach the main gate, heavily guarded by the city watch. "
        "Their armor gleams in the dying sunlight, and they eye you suspiciously."
    );

    StoryNode* secretPath = createStoryNode(3,
        "You discover a hidden path through the city's ancient aqueduct system. "
        "The entrance is partially concealed by overgrown vines, but you can see it leads into the city."
    );

    StoryNode* merchantCaravan = createStoryNode(4,
        "A merchant caravan is preparing to enter the city. "
        "The merchants are busy with their wares, and their guards seem more focused on potential thieves than city visitors."
    );

    // Add choices to the starting node
    addChoice(start, "Approach the main gate directly", mainGate, 6, 0, 0);
    addChoice(start, "Search for a less obvious way in", secretPath, 0, 7, 0);
    addChoice(start, "Try to blend in with the merchant caravan", merchantCaravan, 0, 0, 6);

    // Main gate outcomes
    StoryNode* confrontGuards = createStoryNode(5,
        "You decide to confront the guards directly. "
        "Their captain steps forward, hand on his sword. 'State your business in Eldara, stranger.'"
    );

    StoryNode* bribeGuards = createStoryNode(6,
        "You notice one of the guards discretely eyeing your coin purse. "
        "Perhaps there's a way to make this easier..."
    );

    addChoice(mainGate, "Stand your ground and demand entry", confrontGuards, 8, 0, 0);
    addChoice(mainGate, "Offer a bribe to the guards", bribeGuards, 0, 0, 7);

    return start;
}

StoryNode* initializeStory(void) {
    return createChapterOne();
}

void cleanupStory(StoryNode* root) {
    if (!root) return;
    
    for (int i = 0; i < root->numChoices; i++) {
        cleanupStory(root->nextNodes[i]);
    }
    
    free(root);
} 