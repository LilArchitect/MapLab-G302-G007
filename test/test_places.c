#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/structures.h"
#include "../src/place.h"
#include "../src/utils.h"
#include "utils.h"

// Creates place
static Place *make_place(const char *id, const char *name, const char *type,
                         double lat, double lon, Place *next)
{
    Place *p = malloc(sizeof(Place));
    strncpy(p->id, id, SIZE - 1);
    p->id[SIZE - 1] = '\0';
    strncpy(p->name, name, SIZE - 1);
    p->name[SIZE - 1] = '\0';
    strncpy(p->type, type, SIZE - 1);
    p->type[SIZE - 1] = '\0';
    p->latitude = lat;
    p->longitude = lon;
    p->next = next;
    return p;
}

void test_places()
{
    running("Places");

    // Test 1: find_place exact match
    runningtest("find_place returns correct place on exact match");
    {
        Place *list = make_place("001", "Universitat Pompeu Fabra", "university", 41.403782, 2.193446, NULL);
        list = make_place("002", "Parc de la Ciutadella", "park", 41.386000, 2.186000, list);

        Place *result = find_place(list, "universitat pompeu fabra");
        if (result == NULL)
        {
            fprintf(stderr, "\033[0;31m    Expected a place but got NULL\033[0m\n\n");
            assert(0);
        }
        assertEquals(result->id, "001");
        free_places(list);
    }
    successtest();

    // Test 2: find_place case-insensitive
    runningtest("find_place is case-insensitive");
    {
        Place *list = make_place("001", "Parc de la Ciutadella", "park", 41.386000, 2.186000, NULL);

        Place *result = find_place(list, "parc de la ciutadella");
        if (result == NULL)
        {
            fprintf(stderr, "\033[0;31m    Expected a place but got NULL\033[0m\n\n");
            assert(0);
        }
        assertEquals(result->id, "001");
        free_places(list);
    }
    successtest();

    // Test 3: find_place returns correct coordinates
    runningtest("find_place returns correct latitude and longitude");
    {
        Place *list = make_place("003", "Arc de Triomf", "monument", 41.391100, 2.180400, NULL);

        Place *result = find_place(list, "arc de triomf");
        if (result == NULL)
        {
            fprintf(stderr, "\033[0;31m    Expected a place but got NULL\033[0m\n\n");
            assert(0);
        }
        if (result->latitude < 41.391 || result->latitude > 41.392)
        {
            fprintf(stderr, "\033[0;31m    Latitude out of expected range\033[0m\n\n");
            assert(0);
        }
        free_places(list);
    }
    successtest();

    // Test 4: find_place unknown name returns NULL
    runningtest("find_place returns NULL for unknown place (no suggestions)");
    {
        Place *list = make_place("001", "Parc de la Ciutadella", "park", 41.386000, 2.186000, NULL);

        Place *result = find_place(list, "lugar inexistente zzzzz");
        assertNull(result);
        free_places(list);
    }
    successtest();

    // Test 5: find_place NULL list returns NULL
    runningtest("find_place returns NULL on empty list");
    {
        Place *result = find_place(NULL, "parc de la ciutadella");
        assertNull(result);
    }
    successtest();

    // Test 6: load_places returns NULL on bad path
    runningtest("load_places returns NULL if map does not exist");
    {
        Place *result = load_places("xs_0");
        assertNull(result);
    }
    successtest();

    // Test 7: replace_em_dash funciona en el nombre
    runningtest("replace_em_dash replaces em dash in place names");
    {
        Place *p = make_place("010", "dummy", "university", 41.40, 2.19, NULL);
        p->name[0] = 'A';
        p->name[1] = '-';
        p->name[2] = 'B';
        p->name[3] = (char)0xE2;
        p->name[4] = (char)0x80;
        p->name[5] = (char)0x93;
        p->name[6] = 'C';
        p->name[7] = '\0';

        replace_em_dash(p->name);
        assertEquals(p->name, "A-B-C");
        free_places(p);
    }
    successtest();

    success();
}