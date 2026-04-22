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

// Array of the valid types of maps.
const char *valid_maps[] = {
  "xs_1", "xs_2", "md_1", "lg_1", "xl_1", "2xl_1"
};

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
// Functions
char* get_map_name(void);
int get_option(void);
void address(char* mapName);
House* find_coordinates(char* street, int number, char* mapName);
Place* load_places(char* mapName);
Place* find_place(Place *head, char *name);
void place(char *mapName);
void normalize(char *str);
//

int main() {
  printf("*****************\nWelcome to DSA!\n*****************\n");

  
  char* mapName = get_map_name();
  printf("%s", mapName);

  int option = get_option();
  // Different cases
  switch (option)
  {
  case 1:
    address(mapName); 
    break;
  case 2:
    printf("Not implemented yet\n");
    break;
  case 3:
    place(mapName);
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
    option = get_string(16, "How do you want to input the origin position? (address/coordinate/place): ");
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
  char street[SIZE];
  int number;

  House* result = NULL;

  do{
    printf("Enter street name (e.g. Carrer de Roc Boronat): \n");
    scanf(" %[^\n]", street);   // allows blank spaces
    normalize(street);
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
        printf("Encontrado: '%s' %d \n", house->street, house->number);
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

Place* load_places(char* mapName){
  char filename[SIZE];
  sprintf(filename, "maps/%s/places.txt", mapName);
  
  FILE* file = fopen(filename, "r");
  if(file == NULL) return NULL;

  Place *head = NULL;
  Place temp;

  while (fscanf(file, "%128[^,],%128[^,],%128[^,],%f,%f", temp.id, temp.name, temp.type, &temp.latitude, &temp.longitude) == 5) {
    Place *new = malloc(sizeof(Place));
    
    *new = temp;
    new->next = head;
    head = new;

  }

 
  fclose(file);
  return head;

}

Place* find_place(Place *head, char *name){
  Place *current = head;

  while(current != NULL){
    char temp[SIZE];
    strcpy(temp, current->name);

    normalize(temp);

    if(strcmp(temp, name) == 0) return current;

    current = current->next;

  }
  return NULL;

}

void place(char *mapName) {

    char name[SIZE];

    Place *map = load_places(mapName);
    Place *result = NULL;

    if (map == NULL) {
        printf("Error loading places.\n");
        return;
    }

    do {
      printf("Enter place name (e.g. Area Tallers):\n");
      scanf(" %[^\n]", name);

        normalize(name);

        result = find_place(map, name);

        if (result == NULL) {
            printf("Place not found. Try again.\n");
        }

    } while (result == NULL);

    printf("Found at: ( %f, %f )\n",
           result->latitude,
           result->longitude);

    
    Place *temp;
    while (map != NULL) {
        temp = map;
        map = map->next;
        free(temp);
    }
}

void normalize(char *str) {

    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}