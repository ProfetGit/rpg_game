#ifndef STORY_H
#define STORY_H

#include "game.h"

#define MAX_DESCRIPTION_LENGTH 1000
#define MAX_CHOICE_TEXT 100
#define MAX_CHOICES 4

typedef struct StoryNode {
    int id;
    char description[MAX_DESCRIPTION_LENGTH];
    char choices[MAX_CHOICES][MAX_CHOICE_TEXT];
    int numChoices;
    struct StoryNode* nextNodes[MAX_CHOICES];
    int requirements[MAX_CHOICES][3];  // [strength, intelligence, charisma] requirements for each choice
    void (*consequence)(GameState*);
} StoryNode;

// Story management functions
StoryNode* createStoryNode(int id, const char* description);
void addChoice(StoryNode* node, const char* choiceText, StoryNode* nextNode, int reqStr, int reqInt, int reqCha);
void displayChoices(const StoryNode* node, const GameState* game);
StoryNode* initializeStory(void);
void cleanupStory(StoryNode* root);

// Story content
extern StoryNode* createChapterOne(void);
extern StoryNode* createChapterTwo(void);
extern StoryNode* createChapterThree(void);

#endif 