/**
 * @file story.h
 * @brief Story management and branching narrative functionality
 * @details Defines the story structure and functions for managing the game's narrative
 */

#ifndef STORY_H
#define STORY_H

#include "game.h"

#define MAX_DESCRIPTION_LENGTH 1000
#define MAX_CHOICE_TEXT 100
#define MAX_CHOICES 4

/**
 * @struct StoryNode
 * @brief Represents a single node in the branching story structure
 */
typedef struct StoryNode {
    int id;                                /**< Unique identifier for the node */
    char description[MAX_DESCRIPTION_LENGTH];  /**< Main story text for this node */
    char choices[MAX_CHOICES][MAX_CHOICE_TEXT];  /**< Available choices at this node */
    int numChoices;                        /**< Number of available choices */
    struct StoryNode* nextNodes[MAX_CHOICES];  /**< Next story nodes for each choice */
    int requirements[MAX_CHOICES][3];      /**< Stat requirements for each choice [strength, intelligence, charisma] */
    void (*consequence)(GameState*);       /**< Function pointer for node-specific effects */
} StoryNode;

/**
 * @brief Creates a new story node
 * @param id Unique identifier for the node
 * @param description Main story text for the node
 * @return Pointer to the newly created story node
 */
StoryNode* createStoryNode(int id, const char* description);

/**
 * @brief Adds a choice to a story node
 * @param node Pointer to the story node
 * @param choiceText Text describing the choice
 * @param nextNode Pointer to the next story node for this choice
 * @param reqStr Required strength for this choice
 * @param reqInt Required intelligence for this choice
 * @param reqCha Required charisma for this choice
 */
void addChoice(StoryNode* node, const char* choiceText, StoryNode* nextNode, int reqStr, int reqInt, int reqCha);

/**
 * @brief Displays available choices for the current story node
 * @param node Pointer to the current story node
 * @param game Pointer to the current game state
 */
void displayChoices(const StoryNode* node, const GameState* game);

/**
 * @brief Initializes the complete story structure
 * @return Pointer to the root story node
 */
StoryNode* initializeStory(void);

/**
 * @brief Cleans up and frees all story resources
 * @param root Pointer to the root story node
 */
void cleanupStory(StoryNode* root);

/**
 * @brief Creates the story content for chapter one
 * @return Pointer to the first node of chapter one
 */
extern StoryNode* createChapterOne(void);

/**
 * @brief Creates the story content for chapter two
 * @return Pointer to the first node of chapter two
 */
extern StoryNode* createChapterTwo(void);

/**
 * @brief Creates the story content for chapter three
 * @return Pointer to the first node of chapter three
 */
extern StoryNode* createChapterThree(void);

#endif 