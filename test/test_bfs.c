// test_bfs.c
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/structures.h"
#include "../src/hashmap.h"
#include "../src/bfs.h"
#include "utils.h"

/**
 * @brief Creates a street segment for testing purposes.
 *
 * @param n1    ID of the first node (origin intersection).
 * @param lat1  Latitude of the first node.
 * @param lon1  Longitude of the first node.
 * @param n2    ID of the second node (destination intersection).
 * @param lat2  Latitude of the second node.
 * @param lon2  Longitude of the second node.
 * @param speed Length of the segment in meters (stored in the speed field).
 * @param name  Name of the street.
 * @param next  Pointer to the next street in the linked list.
 * @return      Pointer to the newly allocated Street, or NULL on failure.
 */
static Street *make_street_bfs(long long n1, double lat1, double lon1,
                               long long n2, double lat2, double lon2,
                               double speed, const char *name, Street *next)
{
    Street *s = malloc(sizeof(Street));
    s->node1_id = n1;
    s->lat1 = lat1;
    s->lon1 = lon1;
    s->node2_id = n2;
    s->lat2 = lat2;
    s->lon2 = lon2;
    s->speed = speed;
    strncpy(s->name, name, SIZE - 1);
    s->name[SIZE - 1] = '\0';
    s->next = next;
    return s;
}

/**
 * @brief Test suite for the BFS pathfinding algorithm.
 *
 * Covers direct routes, same origin and destination, routes through
 * intermediate streets, unreachable destinations, and node sharing
 * between the last path segment and the destination.
 */
void bfs_test(void)
{
    running("BFS");

    /**
     * @test BFS finds a direct route between two directly connected streets.
     *
     * Graph: A(1->2) -> B(2->3)
     * Expected: path starts at Carrer A.
     */
    runningtest("BFS finds direct route between connected streets");
    {
        Street *list = make_street_bfs(1, 41.400000, 2.190000, 2, 41.401000, 2.191000, 100, "Carrer A", NULL);
        list = make_street_bfs(2, 41.401000, 2.191000, 3, 41.402000, 2.192000, 100, "Carrer B", list);

        IntersectionMap *map = build_intersection_map(list);

        Street *origin = list->next; // Carrer A
        Street *dest = list;         // Carrer B

        Path_node *path = BFS(map, origin, dest);

        if (path == NULL)
        {
            fprintf(stderr, "\033[0;31m    Expected a path but got NULL\033[0m\n\n");
            assert(0);
        }

        assertEquals(path->street->name, "Carrer A");

        free_path(path);
        hashmap_free(map);
        free_streets(list);
    }
    successtest();

    /**
     * @test BFS returns a valid path when origin and destination are the same street.
     *
     * Expected: path is non-NULL and starts at Carrer A.
     */
    runningtest("BFS returns single-node path when origin equals destination");
    {
        Street *list = make_street_bfs(1, 41.400000, 2.190000, 2, 41.401000, 2.191000, 100, "Carrer A", NULL);

        IntersectionMap *map = build_intersection_map(list);

        Path_node *path = BFS(map, list, list);

        if (path == NULL)
        {
            fprintf(stderr, "\033[0;31m    Expected a path but got NULL\033[0m\n\n");
            assert(0);
        }

        assertEquals(path->street->name, "Carrer A");

        free_path(path);
        hashmap_free(map);
        free_streets(list);
    }
    successtest();

    /**
     * @test BFS finds a route that passes through an intermediate street.
     *
     * Graph: A(1->2) -> B(2->3) -> C(3->4)
     * Origin: Carrer A, Destination: Carrer C.
     * Expected: path length >= 2.
     */
    runningtest("BFS finds route through intermediate street");
    {
        Street *list = make_street_bfs(3, 41.402000, 2.192000, 4, 41.403000, 2.193000, 100, "Carrer C", NULL);
        list = make_street_bfs(2, 41.401000, 2.191000, 3, 41.402000, 2.192000, 100, "Carrer B", list);
        list = make_street_bfs(1, 41.400000, 2.190000, 2, 41.401000, 2.191000, 100, "Carrer A", list);

        IntersectionMap *map = build_intersection_map(list);

        Street *origin = NULL, *dest = NULL;
        Street *cur = list;
        while (cur != NULL)
        {
            if (strcmp(cur->name, "Carrer A") == 0)
                origin = cur;
            if (strcmp(cur->name, "Carrer C") == 0)
                dest = cur;
            cur = cur->next;
        }

        Path_node *path = BFS(map, origin, dest);

        if (path == NULL)
        {
            fprintf(stderr, "\033[0;31m    Expected a path but got NULL\033[0m\n\n");
            assert(0);
        }

        int count = 0;
        Path_node *p = path;
        while (p != NULL)
        {
            count++;
            p = p->next;
        }

        if (count < 2)
        {
            fprintf(stderr, "\033[0;31m    Expected path length >= 2 but got %d\033[0m\n\n", count);
            assert(0);
        }

        free_path(path);
        hashmap_free(map);
        free_streets(list);
    }
    successtest();

    /**
     * @test BFS returns NULL when there is no path between origin and destination.
     *
     * Graph: A(1->2) and B(3->4) are disconnected.
     * Expected: NULL.
     */
    runningtest("BFS returns NULL when no path exists");
    {
        Street *list = make_street_bfs(1, 41.400000, 2.190000, 2, 41.401000, 2.191000, 100, "Carrer A", NULL);
        list = make_street_bfs(3, 41.410000, 2.200000, 4, 41.411000, 2.201000, 100, "Carrer B", list);

        IntersectionMap *map = build_intersection_map(list);

        Street *origin = NULL, *dest = NULL;
        Street *cur = list;
        while (cur != NULL)
        {
            if (strcmp(cur->name, "Carrer A") == 0)
                origin = cur;
            if (strcmp(cur->name, "Carrer B") == 0)
                dest = cur;
            cur = cur->next;
        }

        Path_node *path = BFS(map, origin, dest);
        assertNull(path);

        hashmap_free(map);
        free_streets(list);
    }
    successtest();

    /**
     * @test The last street in the BFS path shares at least one node with the destination.
     *
     * Graph: A(1->2) -> B(2->3)
     * Expected: last street in path shares node1 or node2 with destination.
     */
    runningtest("BFS path last street shares a node with destination");
    {
        Street *list = make_street_bfs(1, 41.400000, 2.190000, 2, 41.401000, 2.191000, 100, "Carrer A", NULL);
        list = make_street_bfs(2, 41.401000, 2.191000, 3, 41.402000, 2.192000, 100, "Carrer B", list);

        IntersectionMap *map = build_intersection_map(list);

        Street *origin = NULL, *dest = NULL;
        Street *cur = list;
        while (cur != NULL)
        {
            if (strcmp(cur->name, "Carrer A") == 0)
                origin = cur;
            if (strcmp(cur->name, "Carrer B") == 0)
                dest = cur;
            cur = cur->next;
        }

        Path_node *path = BFS(map, origin, dest);

        if (path == NULL)
        {
            fprintf(stderr, "\033[0;31m    Expected a path but got NULL\033[0m\n\n");
            assert(0);
        }

        Path_node *p = path;
        while (p->next != NULL)
            p = p->next;
        Street *last = p->street;

        int shares = (last->node1_id == dest->node1_id ||
                      last->node2_id == dest->node1_id ||
                      last->node1_id == dest->node2_id ||
                      last->node2_id == dest->node2_id);

        if (!shares)
        {
            fprintf(stderr, "\033[0;31m    Last street does not share a node with destination\033[0m\n\n");
            assert(0);
        }

        free_path(path);
        hashmap_free(map);
        free_streets(list);
    }
    successtest();

    success();
}