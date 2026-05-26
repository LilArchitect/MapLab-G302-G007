#include "../src/hashmap.h"
#include "utils.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>


void test_nodo_compartido() {
    runningtest("test_nodo_compartido");
    {
        Street s1 = {111, 41.38, 2.17, 222, 41.39, 2.18, 50.0, "Carrer de Test", NULL};
        Street s2 = {222, 41.39, 2.18, 333, 41.40, 2.19, 30.0, "Avinguda de Prova", NULL};
        s1.next = NULL;
        s2.next = NULL;

        IntersectionMap *map = hashmap_create();
        hashmap_insert(map, s1.node1_id, &s1);
        hashmap_insert(map, s1.node2_id, &s1);
        hashmap_insert(map, s2.node1_id, &s2);
        hashmap_insert(map, s2.node2_id, &s2);

        StreetNode *result = hashmap_get(map, 222);
        int count = 0;
        while (result != NULL) { count++; result = result->next; }

        assertEqualsInt(count, 2);
        hashmap_free(map);
    }
    successtest();
}

void test_nodo_exclusivo() {
    runningtest("test_nodo_exclusivo");
    {
        Street s1 = {111, 41.38, 2.17, 222, 41.39, 2.18, 50.0, "Carrer de Test", NULL};
        s1.next = NULL;

        IntersectionMap *map = hashmap_create();
        hashmap_insert(map, s1.node1_id, &s1);
        hashmap_insert(map, s1.node2_id, &s1);

        StreetNode *result = hashmap_get(map, 111);
        int count = 0;
        while (result != NULL) { count++; result = result->next; }

        assertEqualsInt(count, 1);
        hashmap_free(map);
    }
    successtest();
}

void test_nodo_inexistente() {
    runningtest("test_nodo_inexistente");
    {
        IntersectionMap *map = hashmap_create();

        StreetNode *result = hashmap_get(map, 999);
        assertEqualsInt(result == NULL, 1);

        hashmap_free(map);
    }
    successtest();
}

void test_colisiones() {
    runningtest("test_colisiones");
    {
        IntersectionMap *map = hashmap_create();
        Street streets[2048];

        for (int i = 0; i < 2048; i++) {
            streets[i].node1_id = i * 1000;
            streets[i].node2_id = i * 1000 + 1;
            streets[i].lat1  = 41.0 + i * 0.001;
            streets[i].lon1  = 2.0  + i * 0.001;
            streets[i].lat2  = 41.0 + i * 0.001 + 0.0001;
            streets[i].lon2  = 2.0  + i * 0.001 + 0.0001;
            streets[i].speed = 30.0;
            streets[i].next  = NULL;
            snprintf(streets[i].name, SIZE, "Calle %d", i);

            hashmap_insert(map, streets[i].node1_id, &streets[i]);
            hashmap_insert(map, streets[i].node2_id, &streets[i]);
        }

        int errors = 0;
        for (int i = 0; i < 2048; i++) {
            if (hashmap_get(map, streets[i].node1_id) == NULL) errors++;
            if (hashmap_get(map, streets[i].node2_id) == NULL) errors++;
        }

        assertEqualsInt(errors, 0);
        hashmap_free(map);
    }
    successtest();
}

void hashmap_test() {
    running("hashmap_test");
    {
        test_nodo_compartido();
        test_nodo_exclusivo();
        test_nodo_inexistente();
        test_colisiones();
    }
    success();
}