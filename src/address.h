#ifndef ADDRESS_H
#define ADDRESS_H
#include "structures.h"
/*
House* find_coordinates(char* street, int number, char* mapName);
void address(char* mapName);
*/
House* load_houses(char* mapName);
House* find_house(House *head, char *name, int number);
House* house(House *head);
void   free_houses(House *head);
#endif