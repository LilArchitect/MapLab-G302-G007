#ifndef PLACE_H
#define PLACE_H
#include "structures.h"

/**
 * @brief Loads all places from places.txt file.
 *
 * Parses each line expecting the format: id,name,type,latitude,longitude.
 * Replaces em-dashes in names and skips malformed lines.
 *
 * @param mapName Name of the map directory.
 * @return Head of the Place linked list, or NULL if the file cannot be opened.
 */
Place *load_places(char *mapName);

/**
 * @brief Searches for a place by name.
 *
 * Normalizes the input and strips type prefixes before comparing.
 * If not found, suggests up to MAX_SUGGESTIONS similar names using
 * Levenshtein distance and prompts the user to choose one.
 *
 * @param head Head of the Place linked list.
 * @param name Place name entered by the user.
 * @return Pointer to the matching Place, or NULL if not found or cancelled.
 * @see levenshtein()
 */
Place *find_place(Place *head, char *name);

/**
 * @brief Promts the user to enter a place name
 * 
 * Loops until valid place is found. Delegates the search to find_places().
 * 
 * @param head Head of the Place linked list.
 * @return Pointer to the found Place or NULL if the list is empty.
 * @see find_places()
 */
Place *place(Place *head);

/**
 * @brief Frees all nodes of a Place linked list.
 * @param head Head of the list to free.
 */
void free_places(Place *head);
#endif