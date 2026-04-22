#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "utils.h"
#include <ctype.h>
#define MAPS_SIZE 6 // Define the size of the maps array
#define STREET_SIZE 128

// Array of the valid types of maps.
const char *valid_maps[] = {
  "xs_1", "xs_2", "md_1", "lg_1", "xl_1", "2xl_1"
};

typedef struct House{

  char street[STREET_SIZE];
  int number;
  float latitude;
  float longitude;
  struct House *next;

} House;



char* get_map_name(void);
int get_option(void);
void address(char* mapName);
House* find_coordinates(char* street, int number, char* mapName);


int main() {
  printf("*****************\nWelcome to DSA!\n*****************\n");

  
  char* mapName = get_map_name();
  printf("%s", mapName);

  int option = get_option();

  switch (option)
  {
  case 1:
    /* code */
    address(mapName);
    break;
  case 2:
    printf("Not implemented yet\n");
    break;
  case 3:
    printf("Not implemented yet\n");
    break;
  case -1:
    printf("ERROR");
    break;
  }


  return 0;
}

int get_option() {
  char *option = NULL;
  int opt_value = -1;
  do {
    char *option = get_string(16, "How do you want to input the origin position? (address/coordinate/place): ");
    printf("%s\n",option);

    if(!strcmp(option, "address")) {
      opt_value = 1;

    } else if (!strcmp(option, "coordinate")){
      opt_value = 2;
    } else if (!strcmp(option, "place")) {
      opt_value = 3;
    }

    if(opt_value == -1) {
      printf("Enter valid option.\n");
    }
  

  } while (opt_value == -1);

  return opt_value;
}

char* get_map_name() {
  char *map_name = NULL;
  int is_valid = 0;
  // Asks for a map and compares it with the list of valid maps.
  do {
    
    map_name = get_string(8, "Enter map name (xs_1/xs_2/md_1/lg_1/xl_1/2xl_1): ");
    printf("%s\n", map_name);
    // Will be optimized.
    for (int i = 0; i < MAPS_SIZE; i++) {
            if (!strcmp(map_name, valid_maps[i])) {
                is_valid = 1;
                break;
      }
    }

    if(!is_valid) {
      printf("Enter valid map name.\n");
    }
  
  } while (!is_valid);
  
  return map_name;
}


void address(char* mapName){
  char street[STREET_SIZE];
  int number;

  House* result = NULL;

  do{
    printf("Enter street name (e.g. Carrer de Roc Boronat): \n");
    scanf(" %[^\n]", street);   // allows blank spaces
    for(int i = 0; i < strlen(street); i++){
      street[i] = tolower((unsigned char)street[i]);
    }
    printf("%s\n", street);
    printf("Enter street number (e.g. 138): \n");
    scanf("%d", &number);
    printf("%d\n", number);
    getchar();
    result = find_coordinates(street, number, mapName);
  }
  while(result == NULL);

  printf("%lf, %lf", result->latitude, result->longitude);






  
}

House* find_coordinates(char* street, int number, char* mapName){
  
  House* house = malloc(sizeof(House));
  if (house == NULL) return NULL;
  int found = 0;
  char filename[64];
  sprintf(filename, "../maps/%s/houses.txt", mapName);
  //printf("Filename: %s\n", filename);

  int *numeros = NULL;
  int size = 0;
  int capacity = 0;

  FILE *file = fopen(filename, "r");  

  if(file == NULL) {
    free(house);
    return NULL;
  }
  
  while(fscanf(file, "%128[^,],%d,%f,%f", house->street, &house->number, &house->latitude, &house->longitude) == 4){
    //printf("Buscando: '%s' %d \n Archivo: '%s' %d\n", street, number, house->street, house->number);
    for(int i = 0; i < strlen(house->street); i++){
      house->street[i] = tolower((unsigned char)house->street[i]);
    }
    //printf("%s", street);
    if(strstr(house->street, street) != NULL) {
      found = 1;
      if (house->number == number) {
        printf("Encontrado: '%s' %d \n", house->street, house->number);
        fclose(file);
        return house;
      }
      if (size >= capacity) {
        capacity = capacity ? capacity * 2 : 4; // If capacity has a value != 0, it doubles the capacidad, else capacity = 4
        numeros = realloc(numeros, capacity * sizeof(int));
      }
      numeros[size++] = house->number; // Stores numbers of the addres if the introduced one is not found.
    }
  }
  
  if (found) {
    printf("Indtroduced number was not found, avaliavle ones are the following: ");
    for (int i = 0; i < size; i++) {
      printf("%d, ", numeros[i]);
    }
  }

  fclose(file);
  free(house);
  return NULL;
}
