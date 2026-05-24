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
#define SIZE 128

#define MAX_SUGGESTIONS 5
#define MAX_DISTANCE 5
//static void ltrim(char *s);
//static StreetType get_type_and_strip_prefix(char *s);
//void address(char* mapName);
//House* find_coordinates(char* street, int number, char* mapName);


// Function to remove " "
static void ltrim(char *s) {
    while (*s == ' ') {
        memmove(s, s + 1, strlen(s));
    }
}

static StreetType get_type_and_strip_prefix(char *s) {
    ltrim(s);
    // Street
    if (strncmp(s, "carrer ", 7) == 0 ||
        strncmp(s, "calle ", 6)  == 0 ||
        strncmp(s, "c/ ", 3)     == 0 ||
        strncmp(s, "c. ", 3)     == 0) {
        if (strncmp(s, "carrer ", 7) == 0){
          memmove(s, s + 7, strlen(s + 7) + 1);
        }      
        else if (strncmp(s, "calle ", 6) == 0) {
          memmove(s, s + 6, strlen(s + 6) + 1);
        }
        else {
          memmove(s, s + 3, strlen(s + 3) + 1);
        }
        ltrim(s);
        return TYPE_STREET;
    }
    // Avenue
    if (strncmp(s, "avinguda ", 9) == 0 ||
        strncmp(s, "avenida ", 8)  == 0 ||
        strncmp(s, "av. ", 4)      == 0 ||
        strncmp(s, "av ", 3)       == 0) {
        if (strncmp(s, "avinguda ", 9) == 0) {
          memmove(s, s + 9, strlen(s + 9) + 1);
        }
        else if (strncmp(s, "avenida ", 8) == 0) {
          memmove(s, s + 8, strlen(s + 8) + 1);
        }
        else if (strncmp(s, "av. ", 4) == 0) {
          memmove(s, s + 4, strlen(s + 4) + 1);
        }
        else {
          memmove(s, s + 3, strlen(s + 3) + 1);
        }
        ltrim(s);
        return TYPE_AVENUE;
    }
    // Passage
    if (strncmp(s, "passatge ", 9) == 0 ||
        strncmp(s, "pasaje ", 7)   == 0 ||
        strncmp(s, "psg. ", 5)     == 0) {
        if (strncmp(s, "passatge ", 9) == 0) {
          memmove(s, s + 9, strlen(s + 9) + 1);
        }
        else if (strncmp(s, "pasaje ", 7) == 0) {
          memmove(s, s + 7, strlen(s + 7) + 1);
        }
        else {
          memmove(s, s + 5, strlen(s + 5) + 1);
        }
        ltrim(s);
        return TYPE_PASSAGE;
    }
    // Other cases
    return TYPE_UNKNOWN;
}
/*
void address(char* mapName){
  char street[SIZE];
  int number;

  House* result = NULL;

  do{
    printf("Enter street name (e.g. Carrer de Roc Boronat): \n");
    scanf(" %[^\n]", street);   // allows blank spaces
    normalize(street);
    //printf("%s\n", street);
    printf("Enter street number (e.g. 138): \n");
    scanf("%d", &number);
    //printf("%d\n", number);
    getchar();
    result = find_coordinates(street, number, mapName);
  }
  while(result == NULL);

  printf("Found at: ( %lf, %lf )\n", result->latitude, result->longitude);
  
}

House* find_coordinates(char* street, int number, char* mapName){
  
  House* house = malloc(sizeof(House));
  if (house == NULL) return NULL;

  char streetNorm[SIZE];
  strncpy(streetNorm, street, sizeof(streetNorm) - 1);
  streetNorm[sizeof(streetNorm) - 1] = '\0';

  StreetType userType = get_type_and_strip_prefix(streetNorm);

  int found = 0;
  char filename[64];
  sprintf(filename, "maps/%s/houses.txt", mapName);

  int *numeros = NULL;
  int size = 0;
  int capacity = 0;

  FILE *file = fopen(filename, "r");  
  if(file == NULL) {
    free(house);
    return NULL;
  }
  
  while (fscanf(file, " %127[^,],%d,%f,%f",
              house->street, &house->number,
              &house->latitude, &house->longitude) == 4) {
                
    for (size_t i = 0; i < strlen(house->street); i++) {
      house->street[i] = tolower((unsigned char)house->street[i]);
    }

    char houseNorm[SIZE];
    strncpy(houseNorm, house->street, sizeof(houseNorm) - 1);
    houseNorm[sizeof(houseNorm) - 1] = '\0';

    StreetType lineType = get_type_and_strip_prefix(houseNorm);

    if (userType != TYPE_UNKNOWN && userType != lineType) {
      
      continue;
    }

    if (strcmp(houseNorm, streetNorm) == 0) {
      found = 1;

      if (house->number == number) {
        
        fclose(file);
        free(numeros);
        return house;
      }

      if (size >= capacity) {
        capacity = capacity ? capacity * 2 : 4;
        numeros = realloc(numeros, capacity * sizeof(int));
      }
      numeros[size++] = house->number; // Stores numbers of the addres if the introduced one is not found.
    }
  }
  
  if (found) {
    int pos_num = 0;
    bubble_sort(numeros, size);
    
    int unique_size = 1;
    for (int i = 1; i < size; i++) {
      if (numeros[i] != numeros[i - 1]) {
        numeros[unique_size++] = numeros[i];
      }
    }
    size = unique_size;
    
    do{
      printf("Introduced number was not found, avaliable ones are the following: ");
      for (int i = 0; i < size; i++) {
        printf("%d, ", numeros[i]);
      }
      printf("\n");
      printf("Try another number: \n");
      scanf("%d", &number);
      for(int i = 0; i < size; i++){
        if(number == numeros[i]) pos_num = 1;
      }
      return find_coordinates(street, number, mapName);
    }
    while(pos_num == 0);

  }

  if (found) {
    printf("Introduced number was not found, available ones are the following: ");
    for (int i = 0; i < size; i++) {
      printf("%d, ", numeros[i]);
    }
    printf("\n");
  }

  free(numeros);
  fclose(file);
  free(house);
  return NULL;
}
*/

// done but not implemented yet
House* load_houses(char* mapName){
  char filename[SIZE];
  sprintf(filename, "maps/%s/houses.txt", mapName);
  //printf("Trying to open: %s\n", filename); //prueba

  FILE* file = fopen(filename, "r");
  if(file == NULL) return NULL;

  House *head = NULL;
  House temp;

  while (fscanf(file, " %127[^,],%d,%lf,%lf", temp.street, &temp.number, &temp.latitude, &temp.longitude) == 4) {
    
    House *new = malloc(sizeof(House));
    
    *new = temp;
    new->next = head;
    head = new;

  }

  fclose(file);
  return head;

}


House* find_house(House *head, char *name, int number){
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

  while(current != NULL){
    char temp[SIZE];
    strcpy(temp, current->street);

    normalize(temp);
    get_type_and_strip_prefix(temp);
    
    if(strcmp(temp, nameStripped) == 0){
      found = 1;
      
      if (size >= capacity) {
        capacity = capacity ? capacity * 2 : 4;
        doors = realloc(doors, capacity * sizeof(int));
      }
      doors[size++] = current->number; // Stores numbers of the addres if the introduced one is not found.
      if(current->number == number) {
        free(doors);
        return current;
      }
      
    }
    current = current->next;
  }
  if (found) {
    
    bubble_sort(doors, size);
    
    int unique_size = 1;
    for (int i = 1; i < size; i++) {
      if (doors[i] != doors[i - 1]) {
        doors[unique_size++] = doors[i];
      }
    }
    size = unique_size;
    
    printf("Introduced number was not found.\n");
    printf("Available numbers are:\n");
    for (int i = 0; i < size; i++) {
      printf("%d ", doors[i]);
    }
    printf("\n");

    int new_number;
    
    do{
      printf("Try another number:\n");
      scanf("%d", &new_number);
      for(int i = 0; i < size; i++){
        if(new_number == doors[i]){
          free(doors);
          return find_house(head, name, new_number);
        }
      }
      printf("Invalid number.\n");

    } while(1);
  }

  char suggestions[MAX_SUGGESTIONS][SIZE];
  int  distances[MAX_SUGGESTIONS];
  int  sug_count = 0;

  current = head;
  while (current != NULL) {
    char temp[SIZE];
    strcpy(temp, current->street);
    normalize(temp);
    get_type_and_strip_prefix(temp);

    // Evitar duplicados
    int already = 0;
    for (int i = 0; i < sug_count; i++) {
        if (strcmp(suggestions[i], temp) == 0) { already = 1; break; }
    }
    if (already) { current = current->next; continue; }

    int dist = levenshtein(nameStripped, temp);
    if (dist <= MAX_DISTANCE) {
        if (sug_count < MAX_SUGGESTIONS) {
            strcpy(suggestions[sug_count], temp);
            distances[sug_count] = dist;
            sug_count++;
        } else {
            // Reemplazar la peor sugerencia si esta es mejor
            int worst = 0;
            for (int i = 1; i < MAX_SUGGESTIONS; i++)
                if (distances[i] > distances[worst]) worst = i;
            if (dist < distances[worst]) {
                strcpy(suggestions[worst], temp);
                distances[worst] = dist;
            }
        }
    }
    current = current->next;
  }

  if (sug_count > 0) {
    printf("Street not found. Did you mean:\n");
    for (int i = 0; i < sug_count; i++)
        printf("  %d. %s\n", i + 1, suggestions[i]);

    int choice;
    do {
        printf("Choose (1-%d) or 0 to cancel: \n", sug_count);
        scanf("%d", &choice);
    } while (choice < 0 || choice > sug_count);

    if (choice > 0) {
        free(doors);
        return find_house(head, suggestions[choice - 1], number);
    }
  }

  free(doors);
  return NULL;
}


House* house(House *head) {

  char street[SIZE];
  int number;
  House *result = NULL;

  if (head == NULL) {
      printf("Error loading houses.\n");
      return NULL;
  }

  do {
    printf("Enter street name (e.g. Carrer de Roc Boronat): \n");
    scanf(" %[^\n]", street);
    printf("Enter street number (e.g. 138): \n");
    scanf(" %d", &number);
    normalize(street);

    result = find_house(head, street, number);

    if (result == NULL) {
      printf("House not found. Try again.\n");
      }

  } while(result == NULL);

  return result;
}

void free_houses(House *head) {
  printf("DEBUG: Inside free_houses\n");
  House *temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
  }
}