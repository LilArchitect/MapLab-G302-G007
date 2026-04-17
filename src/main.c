#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "utils.h"
#define MAPS_SIZE 6 // Define the size of the maps array

// Array of the valid types of maps.
const char *valid_maps[] = {
  "xs_1", "xs_2", "md_1", "lg_1", "xl_1", "2xl_1"
};


int main() {
  printf("*****************\nWelcome to DSA!\n*****************\n");

  char *map_name = NULL;
  int is_valid = 0;
  // Asks for a map and compares it with the list of valid maps.
  do {
    
    char *map_name = getString(8, "Enter map name: ");
    printf("%s\n", map_name);
    // Will be optimized.
    for (int i = 0; i < MAPS_SIZE; i++) {
            if (strcmp(map_name, valid_maps[i]) == 0) {
                is_valid = 1;
                break;
      }
    }

    if(!is_valid) {
      printf("Enter valid map name.\n");
    }
  
  } while (!is_valid);
  
  free(map_name);




  return 0;
}