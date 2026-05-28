#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "utils.h"
#include <ctype.h>
#include "structures.h"
#include "place.h"
#define MAPS_SIZE 6 // Define the size of the maps array
#define SIZE 256

#define MAX_SUGGESTIONS 5
#define MAX_DISTANCE 5


House *load_houses(char *mapName)
{
  char filename[SIZE];
  sprintf(filename, "maps/%s/houses.txt", mapName);

  FILE *file = fopen(filename, "r");
  if (file == NULL)
    return NULL;

  House *head = NULL;
  House temp;
  int skipped = 0;

  char line[512];
  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, " %127[^,],%d,%lf,%lf",
               temp.street, &temp.number,
               &temp.latitude, &temp.longitude) == 4)
    {
      House *new = malloc(sizeof(House));
      *new = temp;
      new->next = head;
      head = new;
    }
    else
    {
      skipped++;
    }
  }

  printf("DEBUG houses: skipped %d malformed lines\n", skipped);
  fclose(file);
  return head;
}

House *find_house(House *head, char *name, int number)
{
  House *current = head;
  int *doors = NULL; // array of possible numbers
  int capacity = 0;
  int size = 0;
  int found = 0;

  char nameStripped[SIZE];
  strncpy(nameStripped, name, SIZE - 1);
  nameStripped[SIZE - 1] = '\0';
  normalize(nameStripped);
  get_type_and_strip_prefix(nameStripped);

  while (current != NULL)
  {
    char temp[SIZE];
    strcpy(temp, current->street);

    normalize(temp);
    get_type_and_strip_prefix(temp);

    if (strcmp(temp, nameStripped) == 0)
    {
      found = 1;

      if (size >= capacity)
      {
        capacity = capacity ? capacity * 2 : 4;
        doors = realloc(doors, capacity * sizeof(int));
      }
      doors[size++] = current->number; // Stores numbers of the addres if the introduced one is not found.
      if (current->number == number)
      {
        free(doors);
        return current;
      }
    }
    current = current->next;
  }
  if (found)
  {

    bubble_sort(doors, size);

    int unique_size = 1;
    for (int i = 1; i < size; i++)
    {
      if (doors[i] != doors[i - 1])
      {
        doors[unique_size++] = doors[i];
      }
    }
    size = unique_size;
    if (number == -1) {
      printf("Introduced street number was not an number.\n");
    } else {
      printf("Introduced street number is %i, and was not found for selected street.\n", number);
    }
    printf("Available numbers are:\n");
    for (int i = 0; i < size; i++)
    {
      printf("%d ", doors[i]);
    }
    printf("\n");

    int new_number;

    do
    {
      printf("Try another number:\n");
      if (scanf("%d", &new_number) != 1) {
        int c;
        while ((c = getchar() != '\n' && c != EOF));
        new_number = -1;
      }
      printf("choice: %i\n", new_number);
      for (int i = 0; i < size; i++)
      {
        if (new_number == doors[i])
        {
          free(doors);
          return find_house(head, name, new_number);
        }
      }
      printf("Invalid number.\n");

    } while (1);
  }

  char suggestions[MAX_SUGGESTIONS][SIZE];
  int distances[MAX_SUGGESTIONS];
  int sug_count = 0;

  current = head;
  while (current != NULL)
  {
    char temp[SIZE];
    strcpy(temp, current->street);
    normalize(temp);
    get_type_and_strip_prefix(temp);

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
    printf("Street not found. Did you mean:\n");
    for (int i = 0; i < sug_count; i++)
      printf("  %d. %s\n", i + 1, suggestions[i]);

    int choice;
    do
    {
      printf("Choose (1-%d) or 0 to cancel: \n", sug_count);
      if (scanf("%d", &choice) != 1) {
        int c;
        while ((c = getchar() != '\n' && c != EOF));
        choice = -1;
      }
      printf("choice: %i\n", choice);
    } while (choice < 0 || choice > sug_count);

    if (choice > 0)
    {
      free(doors);
      return find_house(head, suggestions[choice - 1], number);
    }
  }

  free(doors);
  return NULL;
}

House *house(House *head)
{

  char street[SIZE];
  int number;
  House *result = NULL;

  if (head == NULL)
  {
    printf("Error loading houses.\n");
    return NULL;
  }

  do
  {
    printf("Enter street name (e.g. Carrer de Roc Boronat): \n");
    scanf(" %[^\n]", street);
    printf("Enter street number (e.g. 138): \n");
    if (scanf("%d", &number) != 1) {
      int c;
      while ((c = getchar() != '\n' && c != EOF));
      number = -1; 
    }
    normalize(street);

    result = find_house(head, street, number);

    if (result == NULL)
    {
      printf("House not found. Try again.\n");
    }

  } while (result == NULL);

  return result;
}

void free_houses(House *head)
{
  // printf("DEBUG: Inside free_houses\n");
  House *temp;
  while (head != NULL)
  {
    temp = head;
    head = head->next;
    free(temp);
  }
}