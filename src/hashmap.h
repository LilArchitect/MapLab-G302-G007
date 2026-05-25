#ifndef HASHMAP_H
#define HASHMAP_H

#include "structures.h"

IntersectionMap *hashmap_create(void);
void hashmap_insert(IntersectionMap *map, long long node_id, Street *street);
StreetNode *hashmap_get(IntersectionMap *map, long long node_id);
IntersectionMap *build_intersection_map(Street *head);
void find_connected_streets_fast(IntersectionMap *map, Street *street);
void hashmap_free(IntersectionMap *map);

#endif