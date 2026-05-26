#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/structures.h"
#include "../src/address.h"
#include "../src/utils.h"
#include "utils.h"

// Creates linked list of houses
static House *make_house(const char *street, int number, double lat, double lon, House *next) {
    House *h = malloc(sizeof(House));
    strncpy(h->street, street, SIZE - 1);
    h->street[SIZE - 1] = '\0';
    h->number = number;
    h->latitude = lat;
    h->longitude = lon;
    h->next = next;
    return h;
}

void test_houses() {
    running("Houses");

    // Test 1: find_house exact match
    runningtest("find_house returns correct house on exact match");
    {
        House *list = make_house("Carrer de Roc Boronat", 138, 41.403782, 2.193446, NULL);
        list = make_house("Carrer de Pallars", 100, 41.400000, 2.190000, list);

        House *result = find_house(list, "Carrer de Roc Boronat", 138);
        if (result == NULL) {
            fprintf(stderr, "\033[0;31m    Expected a house but got NULL\033[0m\n\n");
            assert(0);
        }
        assertEqualsInt(result->number, 138);
        assertEquals(result->street, "Carrer de Roc Boronat");
        free_houses(list);
    }
    successtest();

    // Test 2: find_house case-insensitive
    runningtest("find_house is case-insensitive");
    {
        House *list = make_house("Carrer de Roc Boronat", 10, 41.40, 2.19, NULL);

        House *result = find_house(list, "carrer de roc boronat", 10);
        if (result == NULL) {
            fprintf(stderr, "\033[0;31m    Expected a house but got NULL\033[0m\n\n");
            assert(0);
        }
        assertEqualsInt(result->number, 10);
        free_houses(list);
    }
    successtest();

    // Test 3: find_house strips prefix
    runningtest("find_house matches ignoring 'Carrer'/'Calle' prefix");
    {
        House *list = make_house("Carrer de Pallars", 50, 41.40, 2.19, NULL);

        House *result = find_house(list, "Calle de Pallars", 50);
        if (result == NULL) {
            fprintf(stderr, "\033[0;31m    Expected a house but got NULL\033[0m\n\n");
            assert(0);
        }
        assertEqualsInt(result->number, 50);
        free_houses(list);
    }
    successtest();

    // Test 4: find_house wrong street returns NULL
    runningtest("find_house returns NULL for unknown street (no suggestions)");
    {
        House *list = make_house("Carrer de Roc Boronat", 138, 41.40, 2.19, NULL);

        House *result = find_house(list, "Carrer Inexistente Zzzzz", 1);
        assertNull(result);
        free_houses(list);
    }
    successtest();

    // Test 5: find_house NULL list returns NULL
    runningtest("find_house returns NULL on empty list");
    {
        House *result = find_house(NULL, "Carrer de Roc Boronat", 1);
        assertNull(result);
    }
    successtest();

    // Test 6: load_houses returns NULL on bad path
    runningtest("load_houses returns NULL if map does not exist");
    {
        House *result = load_houses("xs_0");
        assertNull(result);
    }
    successtest();

    success();
}