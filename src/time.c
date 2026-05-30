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

/** @brief Number of valid maps available in the system. */
#define MAPS_SIZE 6

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

void time_connected_streets(Street *head, Street *closest)
{
    struct timespec start, end;
    printf("Now starts time test for find connected streets.\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    find_connected_streets(head, closest);
    clock_gettime(CLOCK_MONOTONIC, &end);

    long long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL +
                           (end.tv_nsec - start.tv_nsec);
    printf("-------> LAB 4 Runtime for connected streets: %lld ns (%.3f ms)\n", elapsed_ns, elapsed_ns / 1000000.0);
}

void time_connected_streets_fast(IntersectionMap *map, Street *closest)
{
    struct timespec start, end;
    printf("Now starts time test for find connected streets fast.\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    find_connected_streets_fast(map, closest);
    clock_gettime(CLOCK_MONOTONIC, &end);

    long long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL +
                           (end.tv_nsec - start.tv_nsec);
    printf("-------> LAB 5 Runtime for connected streets fast using hashmap: %lld ns (%.3f ms)\n", elapsed_ns, elapsed_ns / 1000000.0);
}

void time_bfs(Street *all_streets, IntersectionMap *imap, Street *origin, Street *dest) {
    struct timespec start, end;
    int RUNS = 5;

    Position o = {origin->lat1, origin->lon1};
    Position d = {dest->lat1, dest->lon1};
    double dist_m = haversine(o, d) * 1000;
    printf("Distance between origin and destination: %.3f m\n", dist_m);

    long long total_linear = 0;
    for (int i = 0; i < RUNS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        Path_node *path = BFS_linear(all_streets, origin, dest);
        clock_gettime(CLOCK_MONOTONIC, &end);
        free_path(path);
        total_linear += (end.tv_sec - start.tv_sec) * 1000000000LL +
                        (end.tv_nsec - start.tv_nsec);
    }
    printf("-------> LAB 4 BFS (linear):  avg %lld ns (%.3f ms)\n",
           total_linear / RUNS, (total_linear / RUNS) / 1000000.0);

    long long total_hash = 0;
    for (int i = 0; i < RUNS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        Path_node *path = BFS(imap, origin, dest);
        clock_gettime(CLOCK_MONOTONIC, &end);
        free_path(path);
        total_hash += (end.tv_sec - start.tv_sec) * 1000000000LL +
                      (end.tv_nsec - start.tv_nsec);
    }
    printf("-------> LAB 5 BFS (hashmap): avg %lld ns (%.3f ms)\n",
           total_hash / RUNS, (total_hash / RUNS) / 1000000.0);

    
}
