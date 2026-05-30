#ifndef STREET_H
#define STREET_H

#include "structures.h"

/** 
 * @brief Loads all streets from streets.txt file.
 * 
 * Parses each line expecting the format:
 * node1_id,lat1,lon1,node2_id,lat2,lon2,speed,name.
 * Skips malformed lines.
 * 
 * @param mapName Name of the map directory.
 * @return Head of the Street linked list, or NULL if the file cannot be opened.
 */
Street *load_streets(char *mapName);

/**
 * @brief Finds the street segment closes to the given coordinates.
 * 
 * Computes the haversine distance from the user's prosition to the midpoint
 * of each street segment and returns the nearest one.
 * 
 * @param head Head of the Street linked list.
 * @param user_lat User's latitude in decimal degrees.
 * @param user_lon User's longitude in decimal degrees.
 * @return Pointer to the closes Street segment, or NULL if the list is empty.
 * @see haversine()
 * @see midpoint()
 */
Street *find_closest_street(Street *head, double user_lat, double user_lon);

/**
 * @brief Frees all nodes of a Street linked list.
 * @param head Head of the list to free.
 */
void free_streets(Street *head);

/**
 * @brief Prints all streets connected to a given street segment.
 *
 * Collects all nodes belonging to contiguous segments of the same street,
 * then finds every different street sharing any of those nodes.
 * 
 * @param head    Head of the Street linked list.
 * @param closest The reference street segment.
 * @see find_connected_streets_fast()
 */
void find_connected_streets(Street *head, Street *closest);

/**
 * @brief Find the closest street to given coordinates.
 *
 * Loads streets from the map, prompts the user for latitude and longitude,
 * and prints the closest street along with its connected streets.
 *
 * @param mapName Name of the map directory.
 */
void street_navigation(char *mapName);

#endif