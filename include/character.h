/**
 * @file character.h
 * @brief Character management and creation functionality
 * @details Defines the character structure and related functions for managing game characters
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#define MAX_NAME_LENGTH 50
#define MAX_TRAITS 3

/**
 * @enum CharacterClass
 * @brief Available character classes in the game
 */
typedef enum {
    WARRIOR,    /**< Warrior class, focused on strength */
    SCHOLAR,    /**< Scholar class, focused on intelligence */
    DIPLOMAT,   /**< Diplomat class, focused on charisma */
    ROGUE      /**< Rogue class, balanced attributes */
} CharacterClass;

/**
 * @struct Character
 * @brief Main character structure containing all character attributes
 */
typedef struct {
    char name[MAX_NAME_LENGTH];        /**< Character's name */
    CharacterClass class;              /**< Character's chosen class */
    int strength;                      /**< Physical strength attribute */
    int intelligence;                  /**< Mental capability attribute */
    int charisma;                      /**< Social interaction attribute */
    int health;                        /**< Current health points */
    char traits[MAX_TRAITS][MAX_NAME_LENGTH];  /**< Array of character traits */
    int traitCount;                    /**< Number of traits currently assigned */
} Character;

/**
 * @brief Creates a new character with default values
 * @return Pointer to the newly created character
 */
Character* createCharacter(void);

/**
 * @brief Displays the character's current statistics
 * @param character Pointer to the character whose stats to display
 */
void displayCharacterStats(const Character* character);

/**
 * @brief Updates a character's core statistics
 * @param character Pointer to the character to update
 * @param str New strength value
 * @param intel New intelligence value
 * @param cha New charisma value
 */
void updateCharacterStats(Character* character, int str, int intel, int cha);

/**
 * @brief Adds a new trait to the character
 * @param character Pointer to the character
 * @param trait The trait to add
 */
void addTrait(Character* character, const char* trait);

/**
 * @brief Checks if character meets required stat thresholds
 * @param character Pointer to the character to check
 * @param reqStr Required strength value
 * @param reqInt Required intelligence value
 * @param reqCha Required charisma value
 * @return 1 if requirements are met, 0 otherwise
 */
int hasRequiredStats(const Character* character, int reqStr, int reqInt, int reqCha);

#endif 