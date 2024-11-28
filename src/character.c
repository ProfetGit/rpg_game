#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "../include/character.h"
#include "../include/game.h"

Character* createCharacter(void) {
    Character* character = (Character*)malloc(sizeof(Character));
    if (!character) return NULL;

    clear();
    attron(COLOR_PAIR(COLOR_HEADER_PAIR) | A_BOLD);
    mvprintw(1, 1, "=== Character Creation ===");
    attroff(COLOR_PAIR(COLOR_HEADER_PAIR) | A_BOLD);
    
    // Get character name
    attron(COLOR_PAIR(COLOR_NORMAL_PAIR));
    mvprintw(3, 1, "Enter your character's name: ");
    echo();
    getnstr(character->name, MAX_NAME_LENGTH - 1);
    noecho();
    character->name[strcspn(character->name, "\n")] = 0;

    // Choose character class
    attron(COLOR_PAIR(COLOR_HEADER_PAIR));
    mvprintw(5, 1, "Choose your class:");
    attroff(COLOR_PAIR(COLOR_HEADER_PAIR));
    
    attron(COLOR_PAIR(COLOR_CHOICE_PAIR));
    mvprintw(7, 1, "1. Warrior - Excels in combat and feats of strength");
    mvprintw(8, 1, "2. Scholar - Masters of knowledge and magical arts");
    mvprintw(9, 1, "3. Diplomat - Skilled in persuasion and leadership");
    mvprintw(10, 1, "4. Rogue - Specializes in stealth and cunning");
    attroff(COLOR_PAIR(COLOR_CHOICE_PAIR));
    
    int classChoice;
    do {
        mvprintw(12, 1, "Enter your choice (1-4): ");
        refresh();
        echo();
        scanw("%d", &classChoice);
        noecho();
    } while (classChoice < 1 || classChoice > 4);

    character->class = (CharacterClass)(classChoice - 1);

    // Set initial stats based on class
    switch (character->class) {
        case WARRIOR:
            character->strength = 8;
            character->intelligence = 4;
            character->charisma = 5;
            break;
        case SCHOLAR:
            character->strength = 4;
            character->intelligence = 8;
            character->charisma = 5;
            break;
        case DIPLOMAT:
            character->strength = 4;
            character->intelligence = 5;
            character->charisma = 8;
            break;
        case ROGUE:
            character->strength = 5;
            character->intelligence = 6;
            character->charisma = 6;
            break;
    }

    character->health = 100;
    character->traitCount = 0;

    // Display initial character sheet
    displayCharacterStats(character);
    
    mvprintw(LINES - 1, 1, "Press any key to begin your adventure...");
    refresh();
    getch();
    
    return character;
}

void displayCharacterStats(const Character* character) {
    const char* classNames[] = {"Warrior", "Scholar", "Diplomat", "Rogue"};
    
    clear();
    attron(COLOR_PAIR(COLOR_HEADER_PAIR) | A_BOLD);
    mvprintw(1, 1, "=== Character Sheet ===");
    attroff(COLOR_PAIR(COLOR_HEADER_PAIR) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_NORMAL_PAIR));
    mvprintw(3, 1, "Name: %s", character->name);
    mvprintw(4, 1, "Class: %s", classNames[character->class]);
    
    attron(COLOR_PAIR(COLOR_STAT_PAIR));
    mvprintw(6, 1, "Stats:");
    mvprintw(7, 1, "Health: %d", character->health);
    mvprintw(8, 1, "Strength: %d", character->strength);
    mvprintw(9, 1, "Intelligence: %d", character->intelligence);
    mvprintw(10, 1, "Charisma: %d", character->charisma);
    attroff(COLOR_PAIR(COLOR_STAT_PAIR));
    
    if (character->traitCount > 0) {
        attron(COLOR_PAIR(COLOR_CHOICE_PAIR));
        mvprintw(12, 1, "Traits:");
        for (int i = 0; i < character->traitCount; i++) {
            mvprintw(13 + i, 1, "- %s", character->traits[i]);
        }
        attroff(COLOR_PAIR(COLOR_CHOICE_PAIR));
    }
    
    refresh();
}

void updateCharacterStats(Character* character, int str, int intel, int cha) {
    character->strength += str;
    character->intelligence += intel;
    character->charisma += cha;
}

void addTrait(Character* character, const char* trait) {
    if (character->traitCount < MAX_TRAITS) {
        strncpy(character->traits[character->traitCount], trait, MAX_NAME_LENGTH - 1);
        character->traits[character->traitCount][MAX_NAME_LENGTH - 1] = '\0';
        character->traitCount++;
    }
}

int hasRequiredStats(const Character* character, int reqStr, int reqInt, int reqCha) {
    return character->strength >= reqStr &&
           character->intelligence >= reqInt &&
           character->charisma >= reqCha;
} 