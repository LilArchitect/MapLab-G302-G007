#ifndef TIME_H
#define TIME_H
#include "hashmap.h"

void time_intersection_map(Street *head);
void time_connected_streets(Street *head, Street *closest);
void time_connected_streets_fast(IntersectionMap *map, Street *closest);
void time_bfs(Street *all_streets, IntersectionMap *imap, Street *origin, Street *dest);
#endif