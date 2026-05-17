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
#include "address.h"
#define MAPS_SIZE 6 // Define the size of the maps array
#define SIZE 128

// Array of the valid types of maps.
const char *valid_maps[] = {
  "xs_1", "xs_2", "md_1", "lg_1", "xl_1", "2xl_1"
};




// Functions
int get_option(void);
char* get_map_name(void);

//

int main() {
  printf("*****************\nWelcome to DSA!\n*****************\n");

  
  char* mapName = get_map_name();
  //printf("%s", mapName);

  int option = get_option();
  // Different cases
  switch (option)
  {
  case 1:
    //address(mapName); 
    house(mapName);
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
    //printf("%s\n",option);

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


