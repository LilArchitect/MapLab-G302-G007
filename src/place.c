#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "utils.h"
#include <ctype.h>
#include "structures.h"
#define MAPS_SIZE 6 // Define the size of the maps array
#define SIZE 128

Place *load_places(char *mapName)
{
  char filename[SIZE];
  sprintf(filename, "maps/%s/places.txt", mapName);

  FILE *file = fopen(filename, "r");
  if (file == NULL)
    return NULL;

  Place *head = NULL;
  Place temp;

  while (fscanf(file, "%128[^,],%128[^,],%128[^,],%lf,%lf", temp.id, temp.name, temp.type, &temp.latitude, &temp.longitude) == 5)
  {
    Place *new = malloc(sizeof(Place));

    *new = temp;
    new->next = head;
    head = new;
  }

  fclose(file);
  return head;
}

Place *find_place(Place *head, char *name)
{
  Place *current = head;

  while (current != NULL)
  {
    char temp[SIZE];
    strcpy(temp, current->name);

    normalize(temp);

    if (strcmp(temp, name) == 0)
      return current;

    current = current->next;
  }
  return NULL;
}

Place *place(Place *head)
{

  char name[SIZE];
  Place *result = NULL;

  if (head == NULL)
  {
    printf("Error loading places.\n");
    return NULL;
  }

  do
  {
    printf("Enter place name (e.g. Area Tallers):\n");
    scanf(" %[^\n]", name);
    normalize(name);
    result = find_place(head, name);
    if (result == NULL)
    {
      printf("Place not found. Try again.\n");
    }
  } while (result == NULL);

  return result;
}

void free_places(Place *head)
{
  printf("DEBUG: Inside free_places\n");

  Place *temp;
  while (head != NULL)
  {
    temp = head;
    head = head->next;
    free(temp);
  }
}