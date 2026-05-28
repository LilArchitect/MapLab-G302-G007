#ifndef HASHMAP_H
#define HASHMAP_H

#include "structures.h"

IntersectionMap *hashmap_create();
void hashmap_insert(IntersectionMap *map, long long node_id, Street *street);
StreetNode *hashmap_get(IntersectionMap *map, long long node_id);
IntersectionMap *build_intersection_map(Street *head);
void find_connected_streets_fast(IntersectionMap *map, Street *street);
void hashmap_free(IntersectionMap *map);
Visited_hash *visited_hashmap_create();
void visited_hashmap_insert(Visited_hash *visited, Street *street);
int is_visited_hash(Visited_hash *visited, Street *street);
void free_visited_hash(Visited_hash *visited);
#endif