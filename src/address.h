#ifndef ADDRESS_H
#define ADDRESS_H
#include "structures.h"

/**
 * @brief Loads all houses from a map's houses.txt file.
 *
 * Parses each line expecting the format: street,number,latitude,longitude.
 * Malformed lines are skipped.
 *
 * @param mapName Name of the map directory (e.g. "xs_1").
 * @return Head of the House linked list, or NULL if the file cannot be opened.
 * @note The caller is responsible for freeing the list with free_houses().
 */
House* load_houses(char* mapName);

/**
 * @brief Searches for a house by street name and number.
 *
 * Normalizes the input and strips the street type prefix before comparing.
 * If the street exists but the number is not found, prompts the user to
 * choose from the available numbers. If the street is not found, suggests
 * similar names using Levenshtein distance (max MAX_DISTANCE).
 *
 * @param head   Head of the House linked list.
 * @param name   Street name entered by the user.
 * @param number Street number, or -1 if the user entered a non-numeric value.
 * @return Pointer to the matching House, or NULL if cancelled.
 * @see levenshtein()
 */
House* find_house(House *head, char *name, int number);

/**
 * @brief Interactively prompts the user to enter a house address.
 *
 * Loops until a valid house is found. Delegates the search to find_house().
 *
 * @param head Head of the House linked list.
 * @return Pointer to the found House, or NULL if the list is empty.
 * @see find_house()
 */
House* house(House *head);

/**
 * @brief Frees all nodes of a House linked list.
 * @param head Head of the list to free.
 */ 
void   free_houses(House *head);
#endif