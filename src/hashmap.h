#ifndef HASHMAP_H
#define HASHMAP_H

#include "structures.h"

/**
 * @brief Allocates and initialises an empty IntersectionMap.
 *
 * @return Pointer to the new IntersectionMap, or NULL on allocation failure.
 * @see hashmap_free()
 */
IntersectionMap *hashmap_create();

/**
 * @brief Inserts a street into the hashmap under the given node ID.
 *
 * If an entry for node_id already exists, the street is prepended to
 * its StreetNode list. Otherwise a new HashEntry is created.
 *
 * @param map     Target IntersectionMap.
 * @param node_id Node ID used as the key.
 * @param street  Street segment to associate with node_id.
 */
void hashmap_insert(IntersectionMap *map, long long node_id, Street *street);

/**
 * @brief Retrieves all streets associated with a given node ID.
 *
 * @param map     IntersectionMap to search.
 * @param node_id Node ID to look up.
 * @return Head of the StreetNode list for that node, or NULL if not found.
 */
StreetNode *hashmap_get(IntersectionMap *map, long long node_id);

/**
 * @brief Builds an IntersectionMap from a Street linked list.
 *
 * Each street segment is inserted twice,once under node1_id and once
 * under node2_id, so that both endpoints can be looked up.
 *
 * @param head Head of the Street linked list.
 * @return Pointer to the populated IntersectionMap.
 * @see hashmap_insert()
 */
IntersectionMap *build_intersection_map(Street *head);

/**
 * @brief Prints all streets connected to a given segment.
 *
 * Checks node1_id, node2_id, and the node2_id of the continuation segment. Deduplicates results before printing.
 *
 * @param map    Pre-built IntersectionMap.
 * @param street Reference street segment.
 * @see find_connected_streets()
 */
void find_connected_streets_fast(IntersectionMap *map, Street *street);

/**
 * @brief Frees all memory owned by an IntersectionMap.
 *
 * @param map IntersectionMap to free.
 */
void hashmap_free(IntersectionMap *map);

/**
 * @brief Allocates and initialises an empty Visited_hash.
 *
 * @return Pointer to the new Visited_hash, or NULL on allocation failure.
 * @see free_visited_hash()
 */
Visited_hash *visited_hashmap_create();

/**
 * @brief Marks a street segment as visited in the Visited_hash.
 *
 * Inserts a Visited_entry at the front of the appropriate bucket chain.
 *
 * @param visited Target Visited_hash.
 * @param street  Street segment to mark as visited.
 * @see is_visited_hash()
 */
void visited_hashmap_insert(Visited_hash *visited, Street *street);

/**
 * @brief Checks whether a street segment has already been visited.
 * *
 * @param visited Visited_hash to query.
 * @param street  Street segment to check.
 * @return 1 if visited, 0 otherwise.
 * @see visited_hashmap_insert()
 */
int is_visited_hash(Visited_hash *visited, Street *street);

/**
 * @brief Frees all memory owned by a Visited_hash.
 *
 * @param visited Visited_hash to free.
 */
void free_visited_hash(Visited_hash *visited);
#endif