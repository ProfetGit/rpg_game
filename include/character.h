#ifndef CHARACTER_H
#define CHARACTER_H

#define MAX_NAME_LENGTH 50
#define MAX_TRAITS 3

typedef enum {
    WARRIOR,
    SCHOLAR,
    DIPLOMAT,
    ROGUE
} CharacterClass;

typedef struct {
    char name[MAX_NAME_LENGTH];
    CharacterClass class;
    int strength;
    int intelligence;
    int charisma;
    int health;
    char traits[MAX_TRAITS][MAX_NAME_LENGTH];
    int traitCount;
} Character;

// Character creation and management
Character* createCharacter(void);
void displayCharacterStats(const Character* character);
void updateCharacterStats(Character* character, int str, int intel, int cha);
void addTrait(Character* character, const char* trait);
int hasRequiredStats(const Character* character, int reqStr, int reqInt, int reqCha);

#endif 