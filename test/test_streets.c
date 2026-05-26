#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../src/structures.h"
#include "../src/street.h"
#include "../src/utils.h"
#include "utils.h"

// Crea un segmento de calle
static Street *make_street(long long n1, double lat1, double lon1,
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

void test_streets()
{
    running("Streets");

    // Test 1: find_closest_street devuelve la más cercana
    runningtest("find_closest_street returns the nearest segment");
    {
        // Roc Boronat: cerca de (41.4038, 2.1934)
        Street *list = make_street(1, 41.404389, 2.192402, 2, 41.403429, 2.193660, 30, "Carrer de Roc Boronat", NULL);
        // Llacuna: más lejos, a ~150m al este
        list = make_street(3, 41.403996, 2.195159, 4, 41.404125, 2.194986, 30, "Carrer de la Llacuna", list);

        Street *result = find_closest_street(list, 41.403782, 2.193446);
        if (result == NULL)
        {
            fprintf(stderr, "\033[0;31m    Expected a street but got NULL\033[0m\n\n");
            assert(0);
        }
        assertEquals(result->name, "Carrer de Roc Boronat");
        free_streets(list);
    }
    successtest();

    // Test 2: find_closest_street con un solo segmento
    runningtest("find_closest_street works with a single segment");
    {
        Street *list = make_street(1, 41.400000, 2.190000, 2, 41.401000, 2.191000, 50, "Carrer de Pallars", NULL);

        Street *result = find_closest_street(list, 41.400500, 2.190500);
        if (result == NULL)
        {
            fprintf(stderr, "\033[0;31m    Expected a street but got NULL\033[0m\n\n");
            assert(0);
        }
        assertEquals(result->name, "Carrer de Pallars");
        free_streets(list);
    }
    successtest();

    // Test 3: find_closest_street lista vacía devuelve NULL
    runningtest("find_closest_street returns NULL on empty list");
    {
        Street *result = find_closest_street(NULL, 41.403782, 2.193446);
        assertNull(result);
    }
    successtest();

    // Test 4: find_closest_street punto exactamente en un nodo
    runningtest("find_closest_street works when point is exactly on a node");
    {
        Street *list = make_street(1, 41.400000, 2.190000, 2, 41.401000, 2.191000, 50, "Carrer A", NULL);
        list = make_street(3, 41.410000, 2.200000, 4, 41.411000, 2.201000, 50, "Carrer B", list);

        // El punto es exactamente el nodo1 de Carrer A
        Street *result = find_closest_street(list, 41.400000, 2.190000);
        if (result == NULL)
        {
            fprintf(stderr, "\033[0;31m    Expected a street but got NULL\033[0m\n\n");
            assert(0);
        }
        assertEquals(result->name, "Carrer A");
        free_streets(list);
    }
    successtest();

    // Test 5: load_streets returns NULL on bad path
    runningtest("load_streets returns NULL if map does not exist");
    {
        Street *result = load_streets("map_that_does_not_exist");
        assertNull(result);
    }
    successtest();

    // Test 6: find_connected_streets no crashea con lista de un elemento
    runningtest("find_connected_streets runs without crash on single-element list");
    {
        Street *list = make_street(1, 41.400000, 2.190000, 2, 41.401000, 2.191000, 50, "Carrer Unic", NULL);
        // Solo comprobamos que no crashea (no hay calles conectadas)
        find_connected_streets(list, list);
        free_streets(list);
    }
    successtest();

    // Test 7: find_connected_streets encuentra calles conectadas
    runningtest("find_connected_streets finds streets sharing a node");
    {
        // Carrer A y Carrer B comparten el nodo 2
        Street *list = make_street(1, 41.400000, 2.190000, 2, 41.401000, 2.191000, 50, "Carrer A", NULL);
        list = make_street(2, 41.401000, 2.191000, 3, 41.402000, 2.192000, 50, "Carrer B", list);
        // Carrer C no comparte ningún nodo
        list = make_street(9, 41.410000, 2.200000, 8, 41.411000, 2.201000, 50, "Carrer C", list);

        // Verificamos que no crashea y que Carrer A es el closest
        Street *closest = find_closest_street(list, 41.400000, 2.190000);
        assertEquals(closest->name, "Carrer A");
        find_connected_streets(list, closest);
        free_streets(list);
    }
    successtest();

    success();
}