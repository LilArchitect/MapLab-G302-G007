#ifndef STREET_H
#define STREET_H

#include "structures.h"

Street *load_streets(char *mapName);
Street *find_closest_street(Street *head, double user_lat, double user_lon);
void free_streets(Street *head);
void find_connected_streets(Street *head, Street *closest);
void street_navigation(char *mapName);

#endif