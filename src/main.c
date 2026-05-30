#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "structures.h"
#include "hashmap.h"
#include "bfs.h"
#include "place.h"
#include "address.h"
#include "street.h"
#include "time.h"

/** @brief Number of valid maps available in the system. */
#define MAPS_SIZE 6

/** @brief List of valid map directory names. */
const char *valid_maps[] = {
    "xs_1", "xs_2", "md_1", "lg_1", "xl_1", "2xl_1"};

int get_option(void);
char *get_map_name(void);

/**
 * @brief Entry point of the MapLab application.
 *
 * Loads all map data (houses, streets, places), prompts the user to select
 * an origin and a destination, and computes a path between them using BFS.
 * 
 */
int main()
{
  printf("*****************\nWelcome to DSA!\n*****************\n");

  char *mapName = get_map_name();

  printf("--- ORIGIN ---\n");
  double lat = 0, lon = 0;
  int opt_origin = get_option();

  House *houses = load_houses(mapName);
  Street *streets = load_streets(mapName);
  //runtime
  time_intersection_map(streets);
  IntersectionMap *imap = build_intersection_map(streets);
  Place *places = load_places(mapName);

  // Count loaded elements and warn if any list is empty
  int house_count = 0, street_count = 0, place_count = 0;

  House *hh = houses;
  while (hh != NULL)
  {
    house_count++;
    hh = hh->next;
  }

  Street *ss = streets;
  while (ss != NULL)
  {
    street_count++;
    ss = ss->next;
  }

  Place *pp = places;
  while (pp != NULL)
  {
    place_count++;
    pp = pp->next;
  }

  printf("loaded: %d houses | %d streets | %d places\n",
         house_count, street_count, place_count);

  if (houses == NULL)
    printf("Warning: no houses loaded\n");
  if (streets == NULL)
    printf("Warning: no streets loaded\n");
  if (places == NULL)
    printf("Warning: no places loaded\n");

  // Resolve origin coordinates from user's chosen input method
  switch (opt_origin)
  {
  case 1:
  {
    House *h = house(houses);
    lat = h->latitude;
    lon = h->longitude;
    printf("Street: %s\n    Found at (%.6f, %.6f)\n", h->street, lat, lon);
    break;
  }
  case 2:

    printf("Enter source coordinates:\n");
    printf("    Enter latitude: ");
    scanf("%lf", &lat);
    printf("    Enter longitude: ");
    scanf("%lf", &lon);
    break;
  case 3:
  {
    Place *p = place(places);
    lat = p->latitude;
    lon = p->longitude;
    printf("Place: %s\n    Found at (%.6f, %.6f)\n", p->name, lat, lon);
    break;
  }
  case -1:
    printf("ERROR");
    break;
  }

  // Find the street segment closest to the origin coordinates
  Street *or_street = NULL;
  if (streets == NULL)
  {
    printf("Error: streets not loaded.\n");
  }
  else
  {
    Street *closest = find_closest_street(streets, lat, lon);

    if (closest == NULL)
    {
      printf("No closest street found.\n");
    }
    else
    {
      printf("    Closest street: %s\n", closest->name);
      printf("    Between %lld (%.6f, %.6f) and %lld (%.6f, %.6f)\n\n",
             closest->node1_id, closest->lat1, closest->lon1,
             closest->node2_id, closest->lat2, closest->lon2);

      find_connected_streets_fast(imap, closest); // O(1) lookup
    }
    or_street = closest;
  }

  printf("--- DESTINATION ---\n");
  int opt_dest = get_option();

  switch (opt_dest)
  {
  case 1:
  {
    House *h = house(houses);
    lat = h->latitude;
    lon = h->longitude;
    printf("Street: %s\n    Found at (%.6f, %.6f)\n", h->street, lat, lon);
    break;
  }
  case 2:
    printf("Enter source coordinates:\n");
    printf("    Enter latitude: ");
    scanf("%lf", &lat);
    printf("    Enter longitude: ");
    scanf("%lf", &lon);
    break;
  case 3:
  {
    Place *p = place(places);
    lat = p->latitude;
    lon = p->longitude;
    printf("Place: %s\n     Found at (%.6f, %.6f)\n", p->name, lat, lon);
    break;
  }
  case -1:
    printf("ERROR");
    break;
  }

  Street *dest_street = NULL;
  if (streets == NULL)
  {
    printf("Error: streets not loaded.\n");
  }
  else
  {
    Street *closest = find_closest_street(streets, lat, lon);
    if (closest == NULL)
    {
      printf("No closest street found.\n");
    }
    else
    {
      printf("    Closest street: %s\n", closest->name);
      printf("    Between %lld (%.6f, %.6f) and %lld (%.6f, %.6f)\n\n",
             closest->node1_id, closest->lat1, closest->lon1,
             closest->node2_id, closest->lat2, closest->lon2);
    }
    dest_street = closest;
  }

  if (or_street == NULL || dest_street == NULL)
    printf("Error saving origin or destination street");
  else
  {
    Path_node *node = BFS(imap, or_street, dest_street);
    if (node == NULL)
    {
      printf("No route found.\n");
    }
    else
    {
      print_path(node);
      free_path(node);
    }
  }

  hashmap_free(imap);
  free_houses(houses);
  free_places(places);
  free_streets(streets);
  free(mapName);

  return 0;
}

/**
 * @brief Prompts the user to choose a position input method.
 *
 * Accepts either the option number ("1", "2", "3") or its name
 * ("address", "coordinate", "place"). Loops until a valid option is entered.
 *
 * @return 1 for address, 2 for coordinate, 3 for place.
 */
int get_option()
{
  char *option = NULL;
  int opt_value = -1;
  do
  {
    option = get_string(16, "How do you want to input the position? (address/coordinate/place): ");

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

/**
 * @brief Prompts the user to enter a valid map name.
 *
 * Validates the input against the valid_maps array. Loops until
 * a valid map name is entered.
 *
 * @return Heap-allocated string with the map name.
 * @note The caller is responsible for freeing the returned string.
 */
char *get_map_name()
{
  char *map_name = NULL;
  int is_valid = 0;
  // Asks for a map and compares it with the list of valid maps.
  do
  {

    map_name = get_string(8, "Enter map name (xs_1/xs_2/md_1/lg_1/xl_1/2xl_1): ");
    printf("%s\n", map_name);

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
