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

#define MAX_SUGGESTIONS 5
#define MAX_DISTANCE 5

Place *load_places(char *mapName)
{
  char filename[SIZE];
  sprintf(filename, "maps/%s/places.txt", mapName);

  FILE *file = fopen(filename, "r");
  if (file == NULL)
    return NULL;

  Place *head = NULL;
  Place temp;
  int skipped = 0;

  char line[512];
  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, "%127[^,],%127[^,],%127[^,],%lf,%lf",
               temp.id, temp.name, temp.type,
               &temp.latitude, &temp.longitude) == 5)
    {
      replace_em_dash(temp.name);
      Place *new = malloc(sizeof(Place));
      *new = temp;
      new->next = head;
      head = new;
    }
    else
    {
      skipped++;
    }
  }

  printf("DEBUG places: skipped %d malformed lines\n", skipped);
  fclose(file);
  return head;
}

Place *find_place(Place *head, char *name)
{
  Place *current = head;
  char suggestions[MAX_SUGGESTIONS][SIZE];
  int distances[MAX_SUGGESTIONS];
  int sug_count = 0;

  char nameStripped[SIZE];
  strncpy(nameStripped, name, SIZE - 1);
  nameStripped[SIZE - 1] = '\0';
  normalize(nameStripped);
  get_type_and_strip_prefix(nameStripped);

  while (current != NULL)
  {
    char temp[SIZE];
    strcpy(temp, current->name);
    normalize(temp);
    get_type_and_strip_prefix(temp);

    // printf("DEBUG comparing: [%s] vs [%s]\n", temp, name);
    if (strcmp(temp, name) == 0)
      return current;

    // Evitar duplicados
    int already = 0;
    for (int i = 0; i < sug_count; i++)
    {
      if (strcmp(suggestions[i], temp) == 0)
      {
        already = 1;
        break;
      }
    }
    if (already)
    {
      current = current->next;
      continue;
    }

    int dist = levenshtein(nameStripped, temp);
    if (dist <= MAX_DISTANCE)
    {
      if (sug_count < MAX_SUGGESTIONS)
      {
        strcpy(suggestions[sug_count], temp);
        distances[sug_count] = dist;
        sug_count++;
      }
      else
      {
        // Reemplazar la peor sugerencia si esta es mejor
        int worst = 0;
        for (int i = 1; i < MAX_SUGGESTIONS; i++)
          if (distances[i] > distances[worst])
            worst = i;
        if (dist < distances[worst])
        {
          strcpy(suggestions[worst], temp);
          distances[worst] = dist;
        }
      }
    }

    current = current->next;
  }

  if (sug_count > 0)
  {
    printf("Place not found. Did you mean:\n");
    for (int i = 0; i < sug_count; i++)
      printf("  %d. %s\n", i + 1, suggestions[i]);

    int choice;
    do
    {
      printf("Choose (1-%d) or 0 to cancel: \n", sug_count);
      scanf("%d", &choice);
    } while (choice < 0 || choice > sug_count);

    if (choice > 0)
    {
      return find_place(head, suggestions[choice - 1]);
    }
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
  // printf("DEBUG: Inside free_places\n");

  Place *temp;
  while (head != NULL)
  {
    temp = head;
    head = head->next;
    free(temp);
  }
}