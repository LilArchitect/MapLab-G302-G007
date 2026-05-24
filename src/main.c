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
#include "street.h"
#define MAPS_SIZE 6 // Define the size of the maps array
#define SIZE 128

// Array of the valid types of maps.
const char *valid_maps[] = {
    "xs_1", "xs_2", "md_1", "lg_1", "xl_1", "2xl_1"};

// Functions
int get_option(void);
char *get_map_name(void);

//

int main()
{
  printf("*****************\nWelcome to DSA!\n*****************\n");

  char *mapName = get_map_name();
  // printf("%s", mapName);
  printf("--- ORIGIN ---\n");
  double lat = 0, lon = 0;
  int option = get_option();

  House *houses = load_houses(mapName);
  Street *streets = load_streets(mapName);
  Place *places = load_places(mapName);

  if (houses == NULL)  printf("Warning: no houses loaded\n");
  if (streets == NULL) printf("Warning: no streets loaded\n");
  if (places == NULL)  printf("Warning: no places loaded\n");

  // Different cases
  switch (option)
  {
  case 1:
  {
    House *h = house(houses);
    lat = h->latitude;
    lon = h->longitude;
    printf("    Found at (%.6f, %.6f)\n", lat, lon);
    break;
  }
  case 2:
    printf("Not implemented yet\n");
    break;
  case 3:
  {
    Place *p = place(places);
    lat = p->latitude;
    lon = p->longitude;
    printf("    Found at (%.6f, %.6f)\n", lat, lon);
    break;
  }
  case -1:
    printf("ERROR");
    break;
  }

  if (option == 1 || option == 3)
  {
    printf("DEBUG: Entered option 1 and 3 if\n");  
    if (streets == NULL) {
    printf("Error: streets not loaded.\n");
    } else {
    Street *closest = find_closest_street(streets, lat, lon);
    printf("DEBUG: find_closest_street done\n");  
      if (closest == NULL) {
        printf("No closest street found.\n");
      } else {
        printf("DEBUG: closest found: %s\n", closest->name);
        printf("    Closest street: %s\n", closest->name);
        printf("    Between %lld (%.6f, %.6f) and %lld (%.6f, %.6f)\n\n",
               closest->node1_id, closest->lat1, closest->lon1,
               closest->node2_id, closest->lat2, closest->lon2);
        printf("DEBUG: before find_connected\n");
        find_connected_streets(streets, closest);
        printf("DEBUG: after find_connected\n");
      }
    }
  }

  printf("DEBUG: About to free all arrays\n");  
  free_houses(houses);
  free_places(places);
  free_streets(streets);
  free(mapName);

  return 0;
}

int get_option()
{
  char *option = NULL;
  int opt_value = -1;
  do
  {
    option = get_string(16, "How do you want to input the origin position? (address/coordinate/place): ");
    // printf("%s\n",option);

    if (!strcmp(option, "1") || !strcmp(option, "address"))
      opt_value = 1;
    else if (!strcmp(option, "2") || !strcmp(option, "coordinate"))
      opt_value = 2;
    else if (!strcmp(option, "3") || !strcmp(option, "place"))
      opt_value = 3;

    if (opt_value == -1)
    {
      printf("Enter valid option.\n");
    }

  } while (opt_value == -1);

  return opt_value;
}

char *get_map_name()
{
  char *map_name = NULL;
  int is_valid = 0;
  // Asks for a map and compares it with the list of valid maps.
  do
  {

    map_name = get_string(8, "Enter map name (xs_1/xs_2/md_1/lg_1/xl_1/2xl_1): ");
    printf("%s\n", map_name);
    // Will be optimized.
    for (int i = 0; i < MAPS_SIZE; i++)
    {
      if (!strcmp(map_name, valid_maps[i]))
      {
        is_valid = 1;
        break;
      }
    }

    if (!is_valid)
    {
      printf("Enter valid map name.\n");
    }

  } while (!is_valid);

  return map_name;
}
