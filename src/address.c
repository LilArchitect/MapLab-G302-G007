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

static void ltrim(char *s);
static StreetType get_type_and_strip_prefix(char *s);
void address(char* mapName);
House* find_coordinates(char* street, int number, char* mapName);


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

// done but not implemented yet
House* load_houses(char* mapName){
  char filename[SIZE];
  sprintf(filename, "maps/%s/houses.txt", mapName);
  
  FILE* file = fopen(filename, "r");
  if(file == NULL) return NULL;

  House *head = NULL;
  House temp;

  while (fscanf(file, "%128[^,],%d,%f,%f", temp.street, temp.number, &temp.latitude, &temp.longitude) == 4) {
    House *new = malloc(sizeof(House));
    
    *new = temp;
    new->next = head;
    head = new;

  }

  fclose(file);
  return head;

}