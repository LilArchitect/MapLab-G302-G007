#ifndef TIME_H
#define TIME_H
#include "hashmap.h"

/**
 * @brief Measures the runtime of building the intersection hashmap.
 *
 * @param head Head of the Street linked list to build the map from.
 * @see build_intersection_map()
 */
void time_intersection_map(Street *head);

/**
 * @brief Measures the runtime of find_connected_streets() (linear scan).
 *
 * @param head    Head of the Street linked list.
 * @param closest Reference street segment to search connections from.
 * @see find_connected_streets()
 * @see time_connected_streets_fast()
 */
void time_connected_streets(Street *head, Street *closest);

/**
 * @brief Measures the runtime of find_connected_streets_fast() (hashmap).
 *
 * @param map     Pre-built intersection hashmap.
 * @param closest Reference street segment to search connections from.
 * @see find_connected_streets_fast()
 * @see time_connected_streets()
 */
void time_connected_streets_fast(IntersectionMap *map, Street *closest);

/**
 * @brief Compares the average runtime of BFS with linear vs hashmap visited set.
 *
 * @param all_streets Full Street linked list (used by BFS_linear).
 * @param imap        Pre-built intersection hashmap (used by BFS).
 * @param origin      Origin street segment.
 * @param dest        Destination street segment.
 * @see BFS()
 * @see BFS_linear()
 */
void time_bfs(Street *all_streets, IntersectionMap *imap, Street *origin, Street *dest);
#endif