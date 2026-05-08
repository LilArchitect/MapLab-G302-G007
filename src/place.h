#ifndef PLACE_H
#define PLACE_H
#include "structures.h"

Place* load_places(char* mapName);
Place* find_place(Place *head, char *name);
void place(char *mapName);
#endif