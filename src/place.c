#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "structures.h"

/** @brief Maximum number of place name suggestions shown to the user. */
#define MAX_SUGGESTIONS 5

/** @brief Maximum Levenshtein distance to consider a suggestion valid. */
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
  }

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

    if (strcmp(temp, nameStripped) == 0)
      return current;

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
  Place *temp;
  while (head != NULL)
  {
    temp = head;
    head = head->next;
    free(temp);
  }
}