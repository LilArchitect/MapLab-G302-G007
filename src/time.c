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
#include <time.h>
#define MAPS_SIZE 6 // Define the size of the maps array

// question 1: runtime of initializing the intersections map

void time_intersection_map(Street *head)
{
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    IntersectionMap *map = build_intersection_map(head);
    clock_gettime(CLOCK_MONOTONIC, &end);

    long long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL +
                           (end.tv_nsec - start.tv_nsec);
    printf("Runtime building intersection map: %lld ns (%.3f ms)\n", elapsed_ns, elapsed_ns / 1000000.0);

    hashmap_free(map);
}